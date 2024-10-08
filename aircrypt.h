#define TSIZE 192 
#define PASSIZE 32
#define BLOCKSIZE 16
#define NBBLOCKS 12
/* those contants are chosen with Twofish cryptographic function in mind
 * that uses a 256 bits, hence 32 chars, passphrase and blocks of size 128 bits.
 * We encode  192 chars text meaning 12 blocks. The idea is to print the 
 * result to be able to take a picture of it, hence 6 bits of information per 
 * printed char.
 */



char *printable_char_from_clear_text(char *clear_text,
                                     char *passphrase,
                                     int pass_size);

char *clear_text_from_encrypted_printable_text(char *crypted_printable_text,
                                               char *passphrase,
                                               int pass_size);

int  command_line_treatment(int argc,char *argv[],
                            char *passphrase,int* pass_size,
                            char *clear_text,int* text_size,int mode);

void print_encrypted_message(char *pm);
