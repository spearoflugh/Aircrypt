
#include<stdio.h>
#include "aircrypt.h"

int main(int argc,char *argv[]){
  char clear_text[TSIZE],passphrase[PASSIZE],*printable_encrypted_text;
  int  pass_size,text_size,cl,mode=0; 


  if (!(command_line_treatment(argc,argv,
                             passphrase,&pass_size,
                             clear_text,&text_size,mode)))
   return 0; 
 
  printable_encrypted_text=printable_char_from_clear_text(clear_text,
                                                         passphrase,pass_size);

  print_encrypted_message(printable_encrypted_text);
  return 0;
}
