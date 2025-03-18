#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMG_SIZE    (16 * 1024)  // BL1 最大大小为 16K
#define HEADER_SIZE 16           // BL1 头信息大小为 16 字节
#define BLKSIZE     512          // 块大小为 512 字节

int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned char *Buf;
    int BufLen;
    int nbytes, fileLen;
    unsigned int checksum, count;
    int i;

    // 检查命令行参数
    if (argc != 3)
    {
        printf("Usage: %s <source file> <destination file>\n", argv[0]);
        return -1;
    }

    // 分配 16K 的 buffer
    BufLen = IMG_SIZE;
    Buf = malloc(BufLen);
    if (!Buf)
    {
        perror("Alloc buffer failed!");
        return -1;
    }
    memset(Buf, 0x00, BufLen);

    // 打开源文件
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        perror("source file open error");
        free(Buf);
        return -1;
    }

    // 获取源文件长度
    fseek(fp, 0L, SEEK_END);
    fileLen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // 源文件长度不得超过 16K - 16 字节
    fileLen = (fileLen < (IMG_SIZE - HEADER_SIZE)) ? fileLen : (IMG_SIZE - HEADER_SIZE);

    // 读取源文件到 buffer[16] 开始的位置
    nbytes = fread(Buf + HEADER_SIZE, 1, fileLen, fp);
    if (nbytes != fileLen)
    {
        perror("source file read error\n");
        free(Buf);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    // 计算校验和
    for (i = 0, checksum = 0; i < fileLen; i++)
        checksum += Buf[HEADER_SIZE + i];

    // 计算 BL1 的大小
    // BL1 的大小包括头信息，并且需要调整为 512 字节的整数倍
    fileLen += HEADER_SIZE;
    count = fileLen / BLKSIZE * BLKSIZE;
    if (count < fileLen)
        count += BLKSIZE;

    // 保存 BL1 的大小到 Buf[0-3]
    memcpy(Buf, &count, 4);

    // 保存校验和到 Buf[8-11]
    memcpy(Buf + 8, &checksum, 4);

    // 打开目标文件
    fp = fopen(argv[2], "wb");
    if (fp == NULL)
    {
        perror("destination file open error");
        free(Buf);
        return -1;
    }

    // 将 count 字节的 buffer 写入目标文件
    nbytes = fwrite(Buf, 1, count, fp);
    if (nbytes != count)
    {
        perror("destination file write error");
        free(Buf);
        fclose(fp);
        return -1;
    }

    // 释放资源
    free(Buf);
    fclose(fp);

    printf("BL1 image generated successfully: %s\n", argv[2]);
    return 0;
}