//modifying the second pass function to understand indexing as well
//whenever the operand is read we check for a , and perform address calculation apprpriately
//basically adding 8000(hex)->32768(dec) to address if it is indexed else 0
//FIX THE ERRORS: INDEXING, BYTE FOR CHAR AND HEXADECIMAL
//Indexing has been added
//now adding hex and char array for byte identification
//ascii is giving problems.. unneceesary trailing zeros.. otherwise values are correct
//problem sorted.. minor issue only
//ADD SOMETHING TO IDENTIFY COMMA SEPARATED REGISTER VALUES
//ALSO RESOLVE IMMEDIATE ADDRESSING PROBLEM
//HAVENT RUN THIS  YET
//HANDLE OTHER FORMATS ALSO
//finish verification tomorrow!
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int str_check(char *);//returns 1 if string is pure number else returns zero
int search(FILE *, char *); //search the table and return corresponding value, -1 if not found
int search_opcode(FILE*, char*);//search the optable for opcode and return it
int search2(FILE *, char *);//searches opcode and returns format else 0
void ascii(char*); //to convert string to ascii values for each character
void comma_sep(char *, int *);
int read_byte(char *);
int comma_sep_reg(char *string);
int reg_num(char reg);

int main(){
    //Open file for input
    FILE* input = fopen("output.txt", "r");
    FILE* optab = fopen("map_sic_xe.txt", "r");
    FILE* symtab = fopen("Symtab.txt", "r");
    FILE* output = fopen("output2.txt", "w");
    //ALSO OPEN OUTPUT IN READ FORMAT TO READ PC
    char opcode[8];
    char label[8];
    char operand[30];//changed operand size
    char c;
    int pc;
    int base = -1;
    int op;
    int addr;
    int x = 0;
    while(!feof(input)){
        strcpy(label, " ");
        strcpy(operand, " ");
        fscanf(input, "%X", &pc);//updating pc;
        c = getc(input);
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

        //Having read label, opcode, operand
        //What we do next depends on the opcode
    int format = search2(optab, opcode);
    if(opcode[0]=='+'){
        strcpy(opcode, &opcode[1]);
        format = 4;//Ok for now..
    }
    printf("%X %s %s= format %d %s\n", pc, label, opcode, format, operand);

    switch(format){
        case 1: 
         op = search_opcode(optab, opcode);
         fprintf(output,"%02X\n", op);
         break;
         case 2:
         op = search_opcode(optab, opcode);
         addr = comma_sep_reg(operand);
         fprintf(output,"%02X%02X\n", op, addr);
         break;
         case 3:
         op = search_opcode(optab, opcode);
         //printf("operand is = %s, ascii= %d\n", operand, operand[0]);
         if(operand[0]=='#'){
             strcpy(operand, &operand[1]);
             if(str_check(operand)==0){
                 //do pc and base relative here

                 int a = search(symtab, operand);
                
                 addr = a-pc-3;
             //printf("%X\n", addr);
             char code[8];
             if(addr<2048 && addr>=-2048){
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+1, 2, addr);
                 }
                 else{
                     //printf("Entered else\n");
                 sprintf(code, "%X", addr);
                 //printf("code=%s\n", code);
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+1, 2, code[l-3], code[l-2], code[l-1] );
                 }
             }
             else if((a-base)<2048 && (a-base)>=-2048){
                 addr = a-base;
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+1, 4, addr);
                 }
                 else{
                 sprintf(code, "%X", addr);
                 
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+1, 4, code[l-4], code[l-3], code[l-2] );
                 }
             }

                 
             }
             else{
                 fprintf(output,"%02X%04X\n", op+1, atoi(operand));
             }
         }
         else if(operand[0]=='@'){
             strcpy(operand, &operand[1]);
             if(str_check(operand)==0){
                 int a = search(symtab, operand);
                  addr = a-pc-3;
             //printf("%X\n", addr);
             char code[8];
             if(addr<2048 && addr>=-2048){
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+2, 2, addr);
                 }
                 else{
                     //printf("Entered else\n");
                 sprintf(code, "%X", addr);
                 //printf("code=%s\n", code);
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+2, 2, code[l-3], code[l-2], code[l-1] );
                 }
             }
             else if((a-base)<2048 && (a-base)>=-2048){
                 addr = a-base;
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+2, 4, addr);
                 }
                 else{
                 sprintf(code, "%X", addr);
                 
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+2, 4, code[l-4], code[l-3], code[l-2] );
                 }
             }
                 
             }
             else{
                 if(strcmp(operand, " ")==0){
                     fprintf(output,"%02X0000\n", op);
                 }
                 else{
                 fprintf(output,"%02X%04X\n", op+2, atoi(operand));
                 }
             }
         }
         else if(operand[0]!=' '){
             comma_sep(operand, &x);
             int a = search(symtab, operand);
             addr = a-pc-3;
             //printf("%X\n", addr);
             char code[8];
             if(addr<2048 && addr>=-2048){
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+3, 2+x, addr);
                 }
                 else{
                     //printf("Entered else\n");
                 sprintf(code, "%X", addr);
                 //printf("code=%s\n", code);
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+3, 2+x, code[l-3], code[l-2], code[l-1] );
                 }
             }
             else if((a-base)<2048 && (a-base)>=-2048){
                 addr = a-base;
                 if(addr>=0){
                     fprintf(output,"%02X%X%03X\n", op+3, 4+x, addr);
                 }
                 else{
                 sprintf(code, "%X", addr);
                 
                 int l = strlen(code);
                 fprintf(output,"%02X%X%c%c%c\n", op+3, 4+x, code[l-4], code[l-3], code[l-2] );
                 }
             }

         }
         else{
             fprintf(output,"%02X0000\n", op+3 );
         }
         break;
         case 4:
         op = search_opcode(optab, opcode);
         if(operand[0]=='#'){
             strcpy(operand, &operand[1]);
             if(str_check(operand)==0){
                 addr = search(symtab, operand);
                 fprintf(output,"%X1%05X\n", op+1, addr );
             }
             else{
                 //printf("operand = %s\n", operand);
                 fprintf(output,"%02X1%05X\n", op+1, atoi(operand));
             }
         }
         else if(operand[0]=='@'){
             strcpy(operand, &operand[1]);
             if(str_check(operand)==0){
                 addr = search(symtab, operand);
                 fprintf(output,"%02X1%05X\n", op+2, addr );
             }
             else{
                 fprintf(output,"%02X1%05X\n", op+2, atoi(operand));
             }
         }
         else{
             comma_sep(operand, &x);
             addr = search(symtab, operand);
             fprintf(output,"%02X%X%05X\n", op+3, 1+x, addr);
         }
         break;
         default:
         if(strcmp(opcode,"BYTE")==0){
            //put the operand as it is ASCII in hex
            //reads the operand and modifies it to the correct format to be passed into ascii
            int hex = read_byte(operand);// 2 for hex, 1 for char array
            if (hex == 1){
            ascii(operand);
            fprintf(output,"%s\n", operand);
            }
            else
            fprintf(output,"%s\n", operand);

        }
        else if(strcmp(opcode,"WORD")==0){
            //put the operand as it is 6digit hexadecimal padded with zeros if required
        fprintf(output,"%06X\n", atoi(operand));
        }
        else if(strcmp(opcode,"START")==0){
            //put program name (label), length of code, address
            fprintf(output, "%s %s\n",label, operand);
        }
        else if(strcmp(opcode,"END")==0){
            //put an appropriate message and end the program
            printf("Program ended!\n");
            return 0;
        }
        else if(strcmp(opcode, "BASE")==0){
            printf("BASE found!\n");
         base = search(symtab, operand);
         if(base == -1){
             printf("Invalid operand for base\n");
             return -1;
         }
         fprintf(output,"\n");
        }//ADDED BASE
        else if((strcmp(opcode,"RESW")==0) || (strcmp(opcode,"RESB")==0) ){
            //return error as opcode not found
            fprintf(output, "\n");
        }
        else{
            printf("Opcode not found...\n");
            return 2;
            }//ADD BASE HERE- SO THAT THE VARIABLE BASE HAS SOMETHING
        //SEARCH SYMTAB FOR BASE OPERAND AND FIX THE VALUE FOR LATER USE



         //need to comma separate operands if multiple, i.e, read r1, r2 values

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
int search_opcode(FILE *fp, char *word){
    rewind(fp);
    char rword[8];
    int rcode;
    int format;
    while(!feof(fp)){
        fscanf(fp, "%s %d %X", rword, &format, &rcode);
        if(strcmp(word, rword)==0){
            return rcode;
        }
    }
    return -1;
}
int search2(FILE *fp, char *word){
    rewind(fp);
    char rword[8];
    int rcode;
    int format;
    while(!feof(fp)){
        fscanf(fp, "%s %d %X", rword, &format, &rcode);
        if(strcmp(word, rword)==0){
            return format;
        }
    }
    return 0;
}

int str_check(char *string){
   int i =0;
  for(i=0; i<strlen(string); i++){
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
    *x = 0;
    for(int i=0; i<strlen(string); i++){
        if(string[i]==','){
            if(i==0 || i==(strlen(string)-1)){
                printf("Improper placement of comma\n");
            }
            (string)[i] = ' ';
            sscanf(string, "%s %s", before, after);
            printf("Before , : %s\n", before);
            printf("After , : %s\n", after);
            if(strcmp(after, "X")==0)//specifically for x bit
            *x = 8;//incrementing the x flag
            strcpy(string,before);
            break;
            
            //Error handing not done yet!
        }
        
    }
    
}

int comma_sep_reg(char *string){
   
    char before = string[0];
    char after = ' ';
    for(int i=0; i<strlen(string); i++){
        if(string[i]==','){
            if(i==0 || i==(strlen(string)-1)){
                printf("Improper placement of comma\n");
            }
            (string)[i] = ' ';
            sscanf(string, "%c %c", &before, &after);
            printf("Before , : %c\n", before);
            printf("After , : %c\n", after);
            break;
            
            //Error handing not done yet!
        }
        
    }
    return reg_num(before)*16+reg_num(after);
    
}

int reg_num(char reg){
   switch(reg){
       case 'A': return 0;
       case 'X': return 1;
       case 'L': return 2;
       case 'B': return 3;
       case 'S': return 4;
       case 'T': return 5;
       case 'F': return 6;
       default: return 0;
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