
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int str_check(char *);//returns 1 if string is pure number else returns zero
int search(FILE *, char *); //search the table and return corresponding value, -1 if not found
int ascii(char*); //to convert string to ascii values for each character
int main(){
    //Open file for input
    FILE* input = fopen("Assembly_code.txt", "r");
    FILE* optab = fopen("map.txt", "r");
    FILE* symtab = fopen("Symtab.txt", "r");
    FILE* output = fopen("output2.txt", "w");
    char opcode[8];
    char label[8];
    char operand[8];
    char c;
    int op;
    int addr;
    int length = 0;
    //check opcode
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
        //read operand
        printf("%s %s %s\n", label, opcode, operand);
        op = search(optab, opcode);
        if(op!=-1){
            //if operand exists (format 3 operand always exists) and is non numeric then look it up in symtab
            if(str_check(operand)==0){
                addr = search(symtab, operand);
                if(addr == -1){
                    printf("operand not declared!\n");
                    addr =0;
                    //return 1;
                }
             addr = search(symtab, operand);
            fprintf(output,"%02X%04X\n", op, addr);
            }
           
        }
        else if(strcmp(opcode,"BYTE")==0){
            //put the operand as it is ASCII in hex
            fprintf(output,"%06X", ascii(operand));

        }
        else if(strcmp(opcode,"WORD")==0){
            //put the operand as it is 6digit hexadecimal padded with zeros if required
        fprintf(output,"%06X\n", atoi(operand));
        }
        else if(strcmp(opcode,"START")==0){
            //put program name (label), length of code, address
            fprintf(output, "%s %d %s\n",label, length, operand);
        }
        else if(strcmp(opcode,"END")==0){
            //put an appropriate message and end the program
            printf("Program ended!\n");
            return 0;
        }
        else if((strcmp(opcode,"RESW")!=0) && (strcmp(opcode,"RESB")!=0) ){
            //return error as opcode not found
        printf("Error: opcode not found\n");
        return 2;
        }

    }
    
     fclose(input);
     fclose(optab);
     fclose(symtab);
     fclose(output);
     return 0;
}
int search(FILE *fp, char *word){
    rewind(fp);
    char rword[8];
    int rcode;
    while(!feof(fp)){
        fscanf(fp, "%s %X", rword, &rcode);
        if(strcmp(word, rword)==0){
            return rcode;
        }
    }
    return -1;
}

int str_check(char *string){
   int i =0;
  for(i=0; i<10; i++){
        if(isalpha(string[i])){
         return 0;
        }
        
    }
   
    return 1;
}
int ascii(char *string){
    char *temp;
    char *a;
    int l = strlen(string);
    temp = malloc(l*sizeof(char));
    a = malloc(l*sizeof(char));
    strcpy(a,"");
    for(int i=0; i<l; i++){
        sprintf(temp, "%02X", (int)string[i]);
        strcat(a, temp);
    }
       int asc;
       sscanf(a, "%X", &asc);
       free(string);
       free(a);
       return asc; 
}