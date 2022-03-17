#include <stdio.h>
#include <string.h>
int main(){
    int d = -4097;
    char *addr;
    sprintf(addr, "%X\n", d);
    d = strlen(addr);
    printf("%c%c%c\n",addr[d-4], addr[d-3], addr[d-2]);
}