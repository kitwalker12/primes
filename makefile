hw7: hw7.o primes.o
	g++ -Wall -g primes.o hw7.o -o hw7 -L/home/scf-22/csci551b/openssl/lib -lcrypto
primes.o: primes.cpp primes.h
	g++ -Wall -g -c primes.cpp -I/home/scf-22/csci551b/openssl/include
hw7.o: hw7.cpp
	g++ -Wall -g -c hw7.cpp
clean:
	rm -rf *.o hw7
