#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "twofish.h"
#include "aircrypt.h"

typedef int* bit_list; 
/* Intended use : the bit_lists only contain 0 and 1. Int type is used to make *
 * the code more portable. It doesn't have to rely on the big/little indian    *
 * and/or actual memory representation of types on a particular environment.   *
 */


/* Conversion functions from char* to bit_list :                              *
 * the idea is that {'a','Z'} corresponds to (97,90), so written on bit_lists *
 *(on bytes  : {0,1,1,0,0,0,0,1,1,0,1,1,0,1,0}                                *
 */ 

bit_list bit_list_from_char_list(unsigned char *array,int size){
/* Precondition : array is of size at least size                               * 
 * The function turns the array of chars into its binary representation seen   * 
 * as an array of bits such that every char of array is represented as         *
 * 8 consecutive bits that compose the binary representation of the char.      *
 * This implementation is not optimal but skip all issues linked to the        *
 * peculiarities of the internal representations of int                        *
 */
  int i,j;
  unsigned int n; 
  bit_list bits_array;

  bits_array=(int*) malloc(8*size*sizeof(int));
  for (i=0;i<8*size;i++)bits_array[i]=0;

  for (i=0;i<size;i++){
    n=array[i];
    for (j=0;j<8;j++){
      bits_array[8*i+(7-j)] = n % 2;
      n=n/2;
    }
 }
  return bits_array;
} 

/* Functions to produce printable char. The aim of the program is to print the*
 * bit_list in a form that can be copied by hand or by taking a picture of the*
 * screen. Coding uses 6 bits blocks and represent them with lower,upper case *
 * letters, digits (making 62 possibilities) with '@' and '?'                 *
 */

char printable_char_from_six_bits(bit_list array){
/* Precondition : array is at least of size 6.                                *
 * the function returns a printable char from  the value encoded by the bit   *
 * list. Since there are 6 bits we give an arbitrary encoding for 64 values   *
 * The encoding is as follow, depending on what integer n is represented in   *
 * the array passed as parameter:                                             *
 *    n from 0 to 9 -> encoded by digits '0' to '9'                           *
 *    n from 10 to 35 -> encoded by small case letters.                       *
 *    n from 36 to 61 -> encoded as upper case letters.                       *
 *    n == 62 -> encoded as '@'                                               *
 *    n == 63 -> encoded as '?'                                               *
 */
    char r;
    int n=array[5]+2*array[4]+4*array[3]
              +8*array[2]+16*array[1]+32*array[0];

    if ((n>=0)&&(n<=9)) 
        r='0'+n;
    else if ((n>=10)&&(n<=35)) 
         r='a'+(n-10);
     else if ((n>=36)&&(n<=61)) 
         r='A'+(n-36);
     else if (n==62) 
         r='@';
     else r='?';

   return (r);
}

char *printable_char_list_from_bit_list(bit_list array,int size){
/* Precondition : array has size elements and size is a multiple of 6.         *
 * The function applies printable_char_from_six_bits to every   consecutive    *
 * block of six bits. So it returns one char for every 6 bits of the given     *
 *argument                                                                     * 
 * Example:                                                                    *
 *  printable_char_list_from_bit_list({0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,1,0,1)    *
 *     = {'t','C','l'}                                                         *
 */
  char *result=(char*)malloc(size/6*sizeof(char));
  int i,j=0; 
  
  for(i=0;i<size/6;i++)
     result[j++]=printable_char_from_six_bits(array+(6*i));
 
  return result;
}

bit_list six_bits_from_int(int n){
/* Precondition : n is a positive integer less than 63, meaning representable  *
 * over 6 bits.                                                                *
 * The function return a block  of 6 bits representing  n written in binary    * 
 * Example six_bits_from_int(39) returns the array {1,0,0,1,1,1}               *
 */
  int i;
  bit_list result=(int*)malloc(6*sizeof(int));
  
  result[5]=n%2;     result[4]=(n/2)%2;  result[3]=(n/4)%2; 
  result[2]=(n/8)%2; result[1]=(n/16)%2; result[0]=(n/32)%2; 
     
}

bit_list six_bits_from_printable_char(char r){
/* Precondition : r is either a digit a lower case or upper case letter, or '@'*
 * or '?'                                                                      *
 * This function is the dual of printable_char_from_six_bits and shall return  *
 * the binary representation on 6 bits from which the printable char r has been* 
 * obtained                                                                    *
 * Example : six_bits_from_printable_char('D') = {1,0,0,1,1,1} representing 39 * 
 */
  bit_list result;
  int n;

  if ((r>='0')&&(r<='9')) 
    n=r-'0';
  else if  ((r>='a')&&(r<='z'))
    n=r-'a'+10;
  else if  ((r>='A')&&(r<='Z'))
    n=r-'A'+36;
  else if (r=='@')
    n=62;
  else n=63;
  
  result=six_bits_from_int(n);
  return result;
}

bit_list bit_list_from_printable_char_list(char *array,int size){
/* Precondition : paramater array only contains printable chars in the sense   *
 * of printable_char_from_six_bits coding function and size is the size of     *
 * parameter array.                                                            * 
 */
  bit_list result=(int*)malloc(size*6*sizeof(int)),local;
  int i,j,bl_index=0; 

  for (i=0;i<size;i++){
    local=six_bits_from_printable_char(array[i]);
    for(j=0;j<6;j++) result[bl_index++]=local[j];	
  }
  return(result);
}

char *char_list_from_bit_list(bit_list array,int size){
/* Precondition: array has size elements and size is a multiple of 8.          *
 * transforms each consecutive 8 bits in array into the ascii char of the code *
 * represented by the binary number represented by those 8 bits.               *
 */
  int i,j;
  char *char_list=(char*) malloc (size/8*sizeof(char));
  int ip;

  j=0; 
  for(i=0;i<(size/8);i++){
     ip=i*8;
    
     char_list[j++] = array[ip+7]+array[ip+6]*2+array[ip+5]*4+
                      array[ip+4]*8+array[ip+3]*16+array[ip+2]*32+
                      array[ip+1]*64+array[ip]*128;
   }
  return char_list;
}

void print_encrypted_message(char *pm){
/* Precondition: pm has 256 char. Of course it can be adapted to suit your *
 * way to print the message to be communicated over compromised medium     *
 * Prints the message on the screen as 4 lines of 64 chars                 *
 */

  int i,j;
  printf("\e[1;1H\e[2J"); 
  printf("\n\n\n\n================================================================\n\n");

  for (i=0;i<4;i++){
    for (j=0;j<64;j++) printf("%c",pm[i*64+j]);
    printf("\n");
  } printf("\n");

  printf("================================================================\n\n");
}

int  command_line_treatment(int argc,char *argv[],
                            char *passphrase,int* pass_size,
                            char *clear_text,int* text_size,int mode){
  int i;

  if (argc!=3){
    printf("wrong number of parameters\n");
    printf("aircrypt requires two parameters:\n\t passphrase and text to encrypt/decrypt \n");
    return 0;
  }

  *pass_size = strlen(argv[1]);
  *text_size = strlen(argv[2]);

  if (!mode)
    if (*text_size>(TSIZE*8/6)){  
      printf("the text to be decrypted must be less than");
      printf("%d char long, yours is %d long\n",TSIZE*8/6,*text_size);
      return 0;
    }
  else 
    if (*text_size>TSIZE){  
      printf("the text to be encrypted must be less than");
      printf("%d char long, yours is %d long\n",TSIZE,*text_size);
      return 0;
    }
  
  if (*pass_size>PASSIZE){  
    printf("the encryption key must be less than");
    printf("%d char long, yours is %d long\n",PASSIZE,*pass_size);
    return 0;
  }

  /*****************************************************************************
   *clear text padding : we add random chars up to make for 12 Twofish blocks  *
   *****************************************************************************/
   strncpy(clear_text,argv[2],*text_size);
   strncpy(passphrase,argv[1],*pass_size);
   for (i=*text_size;i<TSIZE;i++)
     clear_text[i]=((passphrase[i%(*pass_size)]+clear_text[i-(*text_size)])%90)+33; /* text is padded with
                                                       passphrase and itself up to TSIZE chars*/
  return 1;
  }


char *encrypt(char *passphrase,int pass_size,char *clear_text){
/* This function encrypts TSIZE/BLOCKSIZE blocks of clear_text using your      *
 * preferred encryption function and passphrase as key. We give an             *
 * implementation using Twofish, meaning that blocks are of size 128 bits. In  *
 * our implementation it makes 12 blocks. The block encryption is done using   *
 * CBC mode                                                                    * 
 */

  char *result=(char*) malloc(TSIZE*sizeof(char));
  int i,j;
  /*Twofish data structures see Twofish.h and Twofish.c */
  Twofish_key xkey;
  Twofish_Byte key_tf[PASSIZE],clear_text_tf[TSIZE],crypted_text_tf[TSIZE]; 
  Twofish_Byte crypted_block_tf[BLOCKSIZE],block_tf[BLOCKSIZE];
  Twofish_Byte initialization_vector[BLOCKSIZE];

  for (i=0;i<pass_size;i++) key_tf[i]=passphrase[i];
  for (i=0;i<TSIZE;i++) clear_text_tf[i]=clear_text[i];

  /* Initialization vector for CBC mode : this is repeated PASSPHRASE (4 times) xor first block */ 
  for (i=0;i<BLOCKSIZE;i++)
   initialization_vector[i]=passphrase[i%PASSIZE];

  Twofish_initialise(); 
  Twofish_prepare_key(key_tf,pass_size,&xkey) ;/* to convert a key to internal form.*/

  for (i=0;i<(TSIZE/BLOCKSIZE);i++){
    for(j=0;j<BLOCKSIZE;j++) block_tf[j]=clear_text_tf[j+i*BLOCKSIZE];
    for(j=0;j<BLOCKSIZE;j++) block_tf[j]=block_tf[j]^initialization_vector[j];
    Twofish_encrypt_block(&xkey,block_tf,crypted_block_tf); /* encryption of the ith block*/
    for(j=0;j<BLOCKSIZE;j++){ 
      crypted_text_tf[j+i*BLOCKSIZE] = crypted_block_tf[j];
      initialization_vector[j]=crypted_block_tf[j]; /* for cbc propagation */
    }
  }

  for (i=0;i<TSIZE;i++) result[i]=crypted_text_tf[i];
  return result;
}

char *decrypt(char *passphrase,int pass_size,char *crypted_text){
/* This function is the dual of the encrypt function.                          *
 */

  char *result=(char*) malloc(TSIZE*sizeof(char));
  int i,j;
  /*Twofish data structures see Twofish.h and Twofish.c */
  Twofish_key xkey;
  Twofish_Byte key_tf[PASSIZE],clear_text_tf[TSIZE],crypted_text_tf[TSIZE]; 
  Twofish_Byte crypted_block_tf[BLOCKSIZE],decrypted_block_tf[BLOCKSIZE];
  Twofish_Byte initialization_vector[BLOCKSIZE];



  for (i=0;i<pass_size;i++) key_tf[i]=passphrase[i];
  for (i=0;i<TSIZE;i++) crypted_text_tf[i]=crypted_text[i];

  /* Initialization vector for CBC mode : this is repeated PASSPHRASE (4 times) xor first block */ 
  for (i=0;i<BLOCKSIZE;i++)
   initialization_vector[i]=passphrase[i%PASSIZE];


  Twofish_initialise(); 
  Twofish_prepare_key(key_tf,pass_size,&xkey) ;/* to convert a key to internal form.*/

  for (i=0;i<(TSIZE/BLOCKSIZE);i++){
    for(j=0;j<BLOCKSIZE;j++) crypted_block_tf[j]=crypted_text_tf[j+i*BLOCKSIZE];
    Twofish_decrypt_block(&xkey,crypted_block_tf,decrypted_block_tf); /* decryption of the ith block*/
    for(j=0;j<BLOCKSIZE;j++){
      clear_text_tf[j+i*BLOCKSIZE] = decrypted_block_tf[j]^initialization_vector[j];
      initialization_vector[j]=crypted_block_tf[j]; /* for CBC propagation*/ 
    }  
  }

  for (i=0;i<TSIZE;i++) result[i]=clear_text_tf[i];
  return result;
}


char *printable_char_from_clear_text(char *clear_text,char *passphrase,int pass_size){
/* Precondition : clear_text is of size TSIZE and passphrase of size pass_size.*
 * This function creates the array of chars to be printed from the clear_text  *
 * and given the key passphrase						       *
 */

   int  *bl,i;
   char *printable_text,*crypted_text;

   crypted_text=encrypt(passphrase,pass_size,clear_text);
   bl=bit_list_from_char_list(crypted_text,TSIZE);
 
      printable_text=printable_char_list_from_bit_list(bl,TSIZE*8);

   return printable_text;
}

char *clear_text_from_encrypted_printable_text(char *crypted_printable_text,
                                            char *passphrase,int pass_size){
/* Precondition : passphrase is of size passe_size and crypted_printable_text  *
 */ 

  char *clear_text,*encrypted_text;
  int  *bl_from_printable_char;

  bl_from_printable_char=bit_list_from_printable_char_list(crypted_printable_text,TSIZE*8/6);
  encrypted_text=char_list_from_bit_list(bl_from_printable_char,TSIZE*8);
  clear_text=decrypt(passphrase,pass_size,encrypted_text);
  
  return clear_text;
}


