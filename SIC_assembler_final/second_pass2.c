//modifying the second pass function to understand indexing as well
//whenever the operand is read we check for a , and perform address calculation apprpriately
//basically adding 8000(hex)->32768(dec) to address if it is indexed else 0
//FIX THE ERRORS: INDEXING, BYTE FOR CHAR AND HEXADECIMAL
//Indexing has been added
//now adding hex and char array for byte identification
//ascii is giving problems.. unneceesary trailing zeros.. otherwise values are correct
//problem sorted.. minor issue only
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int str_check(char *);//returns 1 if string is pure number else returns zero
int search(FILE *, char *); //search the table and return corresponding value, -1 if not found
void ascii(char*); //to convert string to ascii values for each character
void comma_sep(char *, int *);
int read_byte(char *);
int main(){
    //Open file for input
    FILE* input = fopen("Assembly_code.txt", "r");
    FILE* optab = fopen("map.txt", "r");
    FILE* symtab = fopen("Symtab.txt", "r");
    FILE* output = fopen("output2.txt", "w");
    char opcode[8];
    char label[8];
    char operand[30];//changed operand size
    char c;
    int op;
    int addr;
    int length = 0;
    int x = 0; //will be incremented to 32768 if X is found else will remain 0
    //check opcode
    while(!feof(input)){
        strcpy(label, " ");
        strcpy(operand, " ");
        x = 0;
        c = getc(input);
        if(c=='\t'||c==' '){
            fscanf(input,"%s", opcode);
            c = getc(input);
            if(c!='\n'){
                fscanf(input,"%[^\n]s", operand);
                c = getc(input);
            }
            
        }
        else{
            ungetc(c, input);
            fscanf(input,"%s %s %[^\n]s", label, opcode, operand);
            c = getc(input);
        }
        //read operand
        //now checking for indexing
        //our function comma_sep will return a new operand value cutting out the ,X
        //and increment x if X found
        
        comma_sep(operand, &x);
       
        printf("%s %s %s\n", label, opcode, operand);
        op = search(optab, opcode);
        if(op!=-1){
            //if operand exists  and is non numeric then look it up in symtab
            if(str_check(operand)==0){
                addr = search(symtab, operand);
                if(addr == -1){
                    printf("operand not declared!\n");
                    addr =0;
                    //return 1;
                }
             addr = search(symtab, operand);
             //sometimes operand may not exist in that case operand is " "
             if(strcmp(operand, " ")==0)
             addr = 0;
            fprintf(output,"%02X%04X\n", op, (addr+x));
            }
           
        }
        else if(strcmp(opcode,"BYTE")==0){
            //put the operand as it is ASCII in hex
            //reads the operand and modifies it to the correct format to be passed into ascii
            int hex = read_byte(operand);// 2 for hex, 1 for char array
            if (hex == 1){
            ascii(operand);
            fprintf(output,"%s\n", operand);
            }
            else
            fprintf(output, "%s\n", operand);

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
void ascii(char *string){
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
    l = strlen(a);
    a[l]= '\0';
      strcpy(string, a);
}

void comma_sep(char *string, int *x){
   
    char before[30];
    char after[5];
    for(int i=0; i<strlen(string); i++){
        if(string[i]==','){
            if(i==0 || i==(strlen(string)-1)){
                printf("Improper placement of comma\n");
            }
            (string)[i] = ' ';
            sscanf(string, "%s %s", before, after);
            printf("Before , : %s\n", before);
            printf("After , : %s\n", after);
            *x = 32768;//incrementing the x flag
            strcpy(string,before);
            break;
            
            //Error handing not done yet!
        }
        
    }
    
}

int read_byte(char *string){
    char temp[30];
    int j =0;
    for(int i= 0; i<strlen(string); i++){
        if(string[i]!='\''&&(i!=0)){
            temp[j]= string[i];
            j++;
        }
    }
    temp[j] = '\0';
    char c = string[0];
    strcpy(string, temp);
     //1 for char array and 2 for hex which will be divided from length
    if(c=='X')
    return 2;
    return 1;
}