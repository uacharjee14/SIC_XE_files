#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int search(FILE*, char*);
int main(){
     char label[8];
    char opcode[8];
    char operand[8];
    char c;
int loc = 0;
int start = 0;
    
    FILE* input = fopen("Assembly_code.txt", "r");
    FILE* symtab = fopen("Symtab.txt", "w+");
    FILE* optab = fopen("map.txt", "r");
    FILE* output = fopen("output.txt", "w");
    while(!feof(input)){
        strcpy(label, " ");
        strcpy(operand, " ");
        c = getc(input);
        if(c=='\t'||c==' '){
            fscanf(input,"%s", opcode);
            c = getc(input);
            if(c!='\n'){
                fscanf(input,"%s", operand);
                c = getc(input);
            }
            
        }
        else{
            ungetc(c, input);
            fscanf(input,"%s %s %s", label, opcode, operand);
            c = getc(input);
        }
        printf("%s %s %s\n", label, opcode, operand);
        //we have read the first line
        if(strcmp(opcode, "START")==0){
            sscanf(operand, "%X", &loc);
            fprintf(symtab, "%s %X\n", label, loc);
            start = loc;
        }
        //checking if end is reached
        else if(strcmp(opcode, "END")==0){
            fprintf(output, "%X %s %s %s\n", loc, label, opcode, operand);
            printf("Reached END\n");
                printf("length: %d\n", (loc-start));

            return 0;
        }
        else{
            fprintf(output, "%X %s %s %s\n", loc, label, opcode, operand);
            //search symtab for label if found set error else put in symtab
            if(search(symtab, label)==1){
                printf("duplicate symbol!!\n");
                return 1;
            }
            else if(strcmp(label," ")!=0){

              fprintf(symtab, "%s %X\n", label, loc);  
            }
            //search optab for opcode if found increment loc by 3
            if(search(optab, opcode)==1){
             loc+=3;
            }
            else if(strcmp(opcode, "WORD")==0){
                loc+=3;
            }
            else if(strcmp(opcode, "RESW")==0){
                loc= loc+ 3*atoi(operand);
            }
            else if(strcmp(opcode, "BYTE")==0){
                loc= loc+ strlen(operand);
            }
            else if(strcmp(opcode, "RESB")==0){
                loc= loc+ atoi(operand);
            }
            else{
                printf("invalid opcode!\n");
                return 1;
            }
            
        }
        
    }
    fclose(input);
    fclose(symtab);
    fclose(optab);
    fclose(output);

    printf("length: %d\n", (loc-start));
    return 0;
}

int search(FILE *fp, char *word){
    rewind(fp);
    char rword[8];
    char rcode[8];
    while(!feof(fp)){
        fscanf(fp, "%s %s", rword, rcode);
        if(strcmp(word, rword)==0){
            return 1;
        }
    }
    return 0;
}