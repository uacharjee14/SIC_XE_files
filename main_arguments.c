#include <stdio.h>

int main(int argc, char *argv[]){
if(argc != 2){
    printf("Incorrect number of arguments!\n");
    return 1;
}
FILE* fptr = fopen(argv[1], "w");
char c;
printf("Start typing and end with -1\n");
while((c=getchar())!='0'){
    fputc(c, fptr);
}
fclose(fptr);
return 0;
}