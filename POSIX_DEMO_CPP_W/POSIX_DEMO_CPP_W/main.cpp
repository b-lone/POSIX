//
//  main.cpp
//  POSIX_DEMO_CPP_W
//
//  Created by 尤坤 on 2023/3/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main() {
    int fd;
    char *addr;

    // 创建共享内存对象
    fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    // 设置共享内存大小
    if (ftruncate(fd, 1024) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // 映射共享内存
    addr = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // 使用共享内存
    strcpy(addr, "Hello, world!");
    
    char buf[1024];
    bool ifContinue = true;
    while (ifContinue) {
        printf("请输入内容：");
        fflush(stdout);  // 刷新缓冲区，确保提示信息能够及时输出
        if (fgets(buf, 1024, stdin) != NULL) {
            printf("输入的内容是：%s\n", buf);
            if (buf[0] == 'e') {
                ifContinue = false;
            } else {
                strcpy(addr, buf);
            }
        } else {
            printf("读取输入失败！\n");
            break;
        }
    }

    // 解除共享内存映射关系
    if (munmap(addr, 1024) == -1) {
        perror("munmap");
        exit(1);
    }

    // 删除共享内存对象
    if (shm_unlink("/myshm") == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
