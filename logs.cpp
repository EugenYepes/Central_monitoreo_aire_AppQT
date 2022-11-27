#include "logs.h"

void logHex(unsigned char *data, int size)
{
    unsigned char buff[255];
    int i;
    memset(buff, 0x00, sizeof(buff));
    for (i = 0; i < size; i++)
    {
        printf("%02X ", data[i]);
        if (((i + 1) % 16) == 0)
            printf("\n");
    }
    printf("\n");
}
