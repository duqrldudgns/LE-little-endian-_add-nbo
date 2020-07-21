#include <iostream>
#include <cstdio>           //FILE
#include <stdint.h>         //uint32_t
#include <stdlib.h>         //exit
#include <netinet/in.h>     //ntohl

using namespace std;

void usage(char *exe) {
    printf("syntax : %s <file1> <file2> \n", exe);
    printf("sample : %s a.bin c.bin \n", exe);
    exit(1);
}

uint32_t my_ntohl(char *file){
    FILE* pFile = fopen(file, "rb");
    if(pFile == NULL){
        printf("not found : %s \n", file);
        return -1;
    }

    uint32_t n;
    if( !fread(&n, 1, sizeof(uint32_t), pFile) ){  // 기존 코드 fread(&n, sizeof(uint32_t), 1, pFile); 7byte일 경우 리턴값 1, 0
        printf("read failed : %s \n", file);       // 현재 코드 fread(&n, 1, sizeof(uint32_t), pFile); 7byte일 경우 리턴값 4, 3
        return -1;
    }
    fclose(pFile);
    return ntohl(n);
}

int main(int argc, char **argv){
    if(argc!=3) usage(argv[0]);

    uint32_t n1,n2;
    n1 = my_ntohl(argv[1]);
    n2 = my_ntohl(argv[2]);

    printf("%d(0x%x) + %d(0x%x) = %d(0x%x) \n",n1,n1,n2,n2,n1+n2,n1+n2);
    return 0;
}

// $ echo -n -e \\x00\\x00\\x03\\xe8 > thousand.bin
// $ echo -n -e \\x00\\x00\\x01\\xf4 > five-hundred.bin
// $ ./add-nbo thousand.bin five-hundred.bin
// 1000(0x3e8) + 500(0x1f4) = 1500(0x5dc)

// $ echo -n -e \\x12\\x34\\x56\\x78 > a.bin
// $ echo -n -e \\x87\\x65\\x43\\x21 > b.bin
// $ ./add-nbo a.bin b.bin
// 305419896(0x12345678) + -2023406815(0x87654321) = -1717986919(0x99999999)
