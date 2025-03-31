/*
** 在 BL0 阶段，iROM 内固化的代码读取 nandflash 或 SD 卡前面最大16K 的内容（即
*BL1）到 iRAM，
** 并比对前 16 字节中的校验和是否正确，正确则继续，错误则尝试启动下一个设备。**
*BL1 的头信息规定如下
** 0x0：BL1 的大小（最大 16K，包括 BL1 头信息的大小）
** 0x4: 0（规定）
** 0x8：校验和
** 0xC：0（规定）
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMG_SIZE    (16 * 1024)
#define HEADER_SIZE 16
#define BLKSIZE     512


int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned char *Buf;
    int BufLen;
    int nbytes, fileLen;
    unsigned int checksum, count;
    int i;
    if (argc != 3)
    {
        printf("Usage: %s <source file> <destination file>\n", argv[0]);
        return -1;
    }
    /* 分配 16K 的 buffer */
    BufLen = IMG_SIZE;
    Buf = malloc(BufLen);
    if (!Buf)
    {
        perror("Alloc buffer failed!");
        return -1;
    }
    memset(Buf, 0x00, BufLen);
    /* 读源 bin 到 buffer */
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        perror("source file open error");
        free(Buf);
        return -1;
    }
    /* 获取源 bin 长度 */
    fseek(fp, 0L, SEEK_END);
    fileLen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    /* 源 bin 长度不得超过 16K-16byte */
    fileLen =
        (fileLen < (IMG_SIZE - HEADER_SIZE)) ? fileLen : (IMG_SIZE - HEADER_SIZE);
    /* 读源 bin 到 buffer[16] */
    nbytes = fread(Buf + HEADER_SIZE, 1, fileLen, fp);
    if (nbytes != fileLen)
    {
        perror("source file read error\n");
        free(Buf);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    /* 计算校验和 */
    for (i = 0, checksum = 0; i < fileLen; i++)
        checksum += Buf[HEADER_SIZE + i];
    /* 计算 BL1 的大小:
    ** BL1 的大小包括 BL1 的头信息
    ** 另外 iROM 从 SD 卡拷贝是按块拷贝的，因此这里需要调整大小为512 字节的整数倍
    */
    fileLen += HEADER_SIZE;
    count = fileLen / BLKSIZE * BLKSIZE;
    if (count < fileLen)
        count += BLKSIZE;
    memcpy(Buf, &count, 4); // 保存 BL1 的大小到Buf[0-3]
    // 将校验和保存在 buffer[8~15]
    memcpy(Buf + 8, &checksum, 4);
    fp = fopen(argv[2], "wb");
    if (fp == NULL)
    {
        perror("destination file open error");
        free(Buf);
        return -1;
    }
    // 将 count + HEADER_SIZE 字节的 buffer 拷贝到目的bin 中nbytes = fwrite(Buf,
    // 1, count, fp);
    if (nbytes != count)
    {
        perror("destination file write error");
        free(Buf);
        fclose(fp);
        return -1;
    }
    free(Buf);
    fclose(fp);
    return 0;
}