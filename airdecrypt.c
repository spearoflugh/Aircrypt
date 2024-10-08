#include<stdio.h>
#include "aircrypt.h"

int main(int argc,char *argv[]){
  char *clear_text,passphrase[PASSIZE],printable_encrypted_text[TSIZE*8/6];
  int  pass_size,text_size,i,mode=1; 


  if (!(command_line_treatment(argc,argv,
                             passphrase,&pass_size,
                             printable_encrypted_text,&text_size,mode)))
   return 0; 
 

  clear_text=clear_text_from_encrypted_printable_text(printable_encrypted_text,passphrase,pass_size);


  printf("\e[1;1H\e[2J"); 
  printf("decrypted text:\n\n");
  for (i=0;i<TSIZE;i++) printf("%c",clear_text[i]);
  printf("\n\n");

  return 0;
}


