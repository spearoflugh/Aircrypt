# Aircrypt

This is a standalone cryptographic appliction. The idea is to print the
encryption of short text on the terminal. Decryption works symmetrically.

The implementation uses Twofish encryption primitives and encrypts text of 192
chars. If less than 192 chars are given, the text is padded with random chars. 
The encrypted text produces 256 chars (6 bits by chars to make them printable) 
so 4 lines of 64 chars. The implementation of Twofish used is the one done by
Niels Ferguson, for instance available at :
[https://github.com/MacPass/KeePassKit/tree/master/TwoFish](twofish Github
Michael Starke).  

This application follows KISS guidelines: Keep It Simple Stupid.
Anyone can inspect the code before compiling it and using it. Nothing fancy, 
nothing hidden. 
 
Twofish cryptographic function is implemented by default. You can change the
cryptographic function (as well as the size of the text to be encrypted) very
easily. The 192 chars has been chosen because it corresponds to 12 blocks for
the Twofish algorithm. The 12 blocks are encrypted using CBC block cipher mode.  

## Intent and Use-Case

The intended use is : aircrypt hepls you to communicate over compromised
channels. Basically all modern equipment. The idea is to use aircrypt on a
airwall protected computer. As the application is very simple it will run on 
any hardware that support c compiling. 

Once the encryption has been done, one can either copy the output by hand or
simply take a picture of the screen. Then using a compromised messenger
application is no longer a security issue.

The receiver might also use an airwall protected computer to decrypt. One
possibility is to use an old smartphone without sim card and internet
connection: you take the picture of the message received on the compromised
messenger application and use char recognition application to get the encrypted
message.  
  
If the passprhase is sufficiently long and the encryption function is correct no
one can interfere with your private communications. Of course it requires 
manipulations but there is no free lunch. It is the price for privacy. 

The point of the application is that you are not required to have a special
hardware. Likewise you don't have to trust any operating system or internet
provider. Aircrypt was motivated by the "pager attack" that occurred on the 17th
and 18th of september 2024. The context was:

>While pagers were popular in the late twentieth century they have since largely
>been replaced by cell phones. Still, some Hezbollah members had used pagers
>for years before the 7 October attacks, but more members began using them after
>February 2024, when Hezbollah secretary-general Hassan Nasrallah called on
>members to stop using smartphones, citing Israel's capability to infiltrate
>them. Hezbollah subsequently imported the pagers to Lebanon in the
>months before the explosion.  

[https://en.wikipedia.org/wiki/2024_Lebanon_pager_explosions](Wikipedia -
Lebanon pager explosions)


Feel free to add a nice looking interface and passphrase managing system ... at
your own risks and perils. 
 
## Installation 

Following the KISS approach, all what is required are: 
   - C compiler
   - The standard "make" command-line interface software tool

which are available on almost any environment. Implementation is done to be as
cross platform as possible and assumes nothing with relation to how C types are
represented internally. Now just running:

>make

in the directory in which the aircrypt files have been copied should work.  

A successful installation is going to produce two executable programs:
*airencrypt* and *airdecrypt*

Those executables need two arguments: the passphrase and the text to be
encrypted/decrypted using the passphrase. There is no other option. Passphrase
should be long enough (but this is standard cryptographic advice) and the text
to be encrypted should be less than 192 char long. That's it.  

## Usage examples

A basic example is the following command line: 

>/airencrypt passphraseWithEnoughLetters+= "This is a test to demonstrate the capabilities of Aircrypt application. Something like a tweet long message to communicate securely"


that produce the printing on a cleared command line window : 

>================================================================
>
>TMNTxF8lg2K0DcF5tls3wOJSUpR8FHJLWKaskJ43C3jBRhY3wad4Yyhcy5m@9m52
>4701pSoILVNsc7qYWUn9@8fmOItENGDD1FpXuIaEA@UmHOVx?gQUdUxBzYRSnaw?
>qKu63pyUxEMRhyrL9bknoCq0uihMFfUn7emTjaB7Q320CNEhpkBng?27Zjzp8hAP
>oRb?up5NdIZX5@ztOA1msaM9Ecsb2w8J23j7iIWxQicuVyMavVxQAbviLZnIJsAH
>
>================================================================

>./airdecrypt passphraseWithEnoughLetters+= "TMNTxF8lg2K0DcF5tls3wOJSUpR8FHJLWKaskJ43C3jBRhY3wad4Yyhcy5m@9m524701pSoILVNsc7qYWUn9@8fmOItENGDD1FpXuIaEA@UmHOVx?gQUdUxBzYRSnaw?qKu63pyUxEMRhyrL9bknoCq0uihMFfUn7emTjaB7Q320CNEhpkBng?27Zjzp8hAPoRb?up5NdIZX5@ztOA1msaM9Ecsb2w8J23j7iIWxQicuVyMavVxQAbviLZnIJsAH"

that produces the follwing print on a cleared command line window :
>decrypted text:

>This is a test to demonstrate the capabilities of Aircrypt application. Something like a tweet long message to communicate securely/DKGO"E[BLSEexWBOZA:L=NE8HBIqUPJ9Ov3QB4HL[xF3SZL;YiID'HZE&U=R

Notice that characters have been added to the original text. You can ignore
them. Feel free to develop your own communication protocol if you need to be
sure that the receiver knows exactly where the original text ends. 

If you enter a text longer than 192 char the program won't do anythig : 

>./airencrypt passphraseWithEnoughLetters++ "This is a test to demonstrate the capabilities of Aircrypt application. Something like a tweet long message to communicate securely. But when the message is too long it won't work. You will have to split the message into as many blocks as required"
>the text to be encrypted must be less than192 char long, yours is 247 long

If you give an erroneous passphrase Aircrypt won't tell you anything and decrypt
using the given passphrase, for instance if we replace the "=" at the end of the
passphrase with "+":

>./airdecrypt passphraseWithEnoughLetters+= "TMNTxF8lg2K0DcF5tls3wOJSUpR8FHJLWKaskJ43C3jBRhY3wad4Yyhcy5m@9m524701pSoILVNsc7qYWUn9@8fmOItENGDD1FpXuIaEA@UmHOVx?gQUdUxBzYRSnaw?qKu63pyUxEMRhyrL9bknoCq0uihMFfUn7emTjaB7Q320CNEhpkBng?27Zjzp8hAPoRb?up5NdIZX5@ztOA1msaM9Ecsb2w8J23j7iIWxQicuVyMavVxQAbviLZnIJsAH"

is going to produce garbage, here including non printable chars: 


>decrypted text:
>
>e����h��*���z@�Aa"�B�������ty�B��%Ԥ�#ڊ�N^D^g�kq�����&</�h��e0W~���MɅ1����V?�?,ta�y:D�mq����
>                                                                                           xii���*������Mw2aM���x��*��q�kI�;u����Pvd�5X\���������J�e������zo
>
>
 
