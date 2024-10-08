aircrypt: airdecrypt.o airencrypt.o twofish.o aircrypt.o
	gcc -o airencrypt airencrypt.o aircrypt.o twofish.o  
	gcc -o airdecrypt airdecrypt.o aircrypt.o twofish.o

airencrypti.o: airencrypt.c 
	gcc -c airencrypt.c

airdecrypt.o: airdecrypt.c
	gcc -c airdecrypt.c

aircrypt.o: aircrypt.c
	gcc -c aircrypt.c 

twofish.o: twofish.c
	gcc -c twofish.c

clean: 
	rm *.o
