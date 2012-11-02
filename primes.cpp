#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <openssl/bn.h>
#include "primes.h"

using namespace std;

void setbitn(unsigned char *cp,int bitpos,int value){
        unsigned char temp=(unsigned char)1;
        temp<<=bitpos;
        if(value) 
          *cp=*cp | temp;
        else
          
          *cp=*cp & ( ~ temp);
}

int getbitn(unsigned char c, int bitpos){
 
        unsigned char temp=(unsigned char)1;
        temp<<=bitpos;
        c&=temp; 
        if(c)
                return 1;
        else
                return 0;
}

void setbit(unsigned char *sp, size_t bitpos, int value, unsigned int MAXBYTES){
  
        size_t byteno;
        int bit07; 
        byteno=bitpos/8; 
        if(byteno >= MAXBYTES){
                fprintf(stderr,"setbit: buffer overflow trapped - %d:%d\n",byteno,MAXBYTES);
                exit(1); 
        }
        bit07=(int)bitpos%8; 
        setbitn(sp+byteno,bit07,value);
}

int getbit(unsigned char *sp, size_t bitpos, unsigned int MAXBYTES){
  
        size_t byteno;
        int bit07;
        byteno=bitpos/8; 
        if(byteno >= MAXBYTES){
                fprintf(stderr,"getbit: attempt to read beyond allocated buffer\n");
                fprintf(stderr,"bitpos: %d\n",bitpos);
                exit(1);
        }
        bit07=(int)bitpos%8; 
        return getbitn(*(sp+byteno),bit07);
}

unsigned long int readLong(unsigned long int temp) {
	unsigned char a[4];
	a[3] = (temp>>24) & 0xFF;
	a[2] = (temp>>16) & 0xFF;
	a[1] = (temp>>8) & 0xFF;
	a[0] = temp & 0xFF;
	return ((a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0]);
}

void sieve(unsigned int maxval) {
	int i,k;
	unsigned int nextprime=2;
	unsigned int MAXBYTES=ceil((double)maxval/8)+1;
	//unsigned int MAXBYTES=2;
	unsigned char a[MAXBYTES];
    for(i=0;i<(int)MAXBYTES;i++) a[i]=0x00; 
    setbit(a,0,1,MAXBYTES); setbit(a,1,1,MAXBYTES); 
    while(nextprime+1<MAXBYTES*8){
            k=nextprime;
            while(nextprime*k<MAXBYTES*8){
					//printf("-%d-",nextprime*k);
                    setbit(a,nextprime*k,1,MAXBYTES);
                    k++;
            }
            while(nextprime+1<MAXBYTES*8 && getbit(a,++nextprime,MAXBYTES));
            //printprime(nextprime,MAXBYTES,maxval);
    }
	
	uint32_t mv=readLong((unsigned long int)maxval);
	mv=htonl(mv);
//	printf("%lu",(unsigned long int)htonl(mv));
	fwrite(&mv, sizeof(uint32_t), 1, stdout); 
//	unsigned char mv[4];
//	mv[3] = (num>>24) & 0xFF;
//	mv[2] = (num>>16) & 0xFF;
//	mv[1] = (num>>8) & 0xFF;
//	mv[0] = num & 0xFF;
//	printf("%0x%0x%0x%0x",mv[3],mv[2],mv[1],mv[0]);
	for(unsigned long int i=0;i<=maxval;i++) {
		if(getbit(a,i,MAXBYTES)==0) {
			uint32_t num = readLong(i);
			num=htonl(num);
//			printf("%lu",(unsigned long int)htonl(num));
			fwrite(&num, sizeof(uint32_t), 1, stdout); 
//			unsigned char a[4];
//			a[3] = (num>>24) & 0xFF;
//			a[2] = (num>>16) & 0xFF;
//			a[1] = (num>>8) & 0xFF;
//			a[0] = num & 0xFF;
//			printf("%0x%0x%0x%0x",a[3],a[2],a[1],a[0]);
		}
	}
	//printf("\n");
}

void read(FILE *pf) {
	while(true) {
		int result=0;
		unsigned long int *buff;
		buff=new unsigned long int;
		result=fread(buff,sizeof(unsigned long int),1,pf);
		if(result==0) {
			break;
		}
		unsigned long int val;
		val = readLong(*buff);
		printf("%lu",val);
	}
}

int trialdiv(unsigned long long int n, FILE *f,bool show) {
	if(show) {
		printf("  n = %llu\n",n);
	}
	int result=0;
	unsigned long int *buff;
	buff=new unsigned long int;
	result=fread(buff,sizeof(unsigned long int),1,f);
	if(result==0) {
		fputs("Error in reading primesfile\n",stderr);
		exit(1);
	}
	unsigned long int maxval;
	maxval = readLong(*buff);
	if(maxval<floor(sqrt(n))) {
		if(show) {
			printf("  ");
		}
		printf("n passes trial division test (not enough primes)\n");
		return 2;
	}
	while(true) {
		result=fread(buff,sizeof(unsigned long int),1,f);
		if(result==0) {
			break;
		}
		unsigned long int x;
		x = readLong(*buff); 
		//printf("%lu\n",x);
		if(x>=n) {
			break;
		}
		if(n%x==0) {
			if(show) {
				printf("  ");
			}
			printf("n is composite by trial division (mod %lu = 0)\n",x);
			return 0;
		}
	}
	if(show) {
		printf("  ");
	}
	printf("n passes trial division test\n");
	return 1;
}

//typedef unsigned int uint128_t __attribute__((mode(TI)));

static uint64_t pow_mod(uint64_t a, uint64_t x, uint64_t n)
{
  uint64_t r=1;

  while ( x ) {
    if ( (x & 1) == 1 )
      //r = (uint128_t)a*r % n; // Slow
      r = a*r % n;

    x >>= 1;
    //a = (uint128_t)a*a % n; // SLow
    a = a*a % n;
  }

  return r;
}

int millerrabin(unsigned long long int n, int t, FILE *f,bool show) {
	int result=0;
	unsigned long int *buff;
	buff=new unsigned long int;
	result=fread(buff,sizeof(unsigned long int),1,f);
	if(result==0) {
		fputs("Error in reading primesfile\n",stderr);
		exit(1);
	}
	//unsigned long int maxval;
	//maxval = readLong(*buff);
	
	uint64_t N=n;
	int s=0;
	for ( uint64_t m = N-1; !(m & 1); ++s, m >>= 1 );
	uint64_t R = (N-1) / (1<<s);
	if(show) {
		printf("    n-1 = %llu\n    s = %d\n    r = %llu\n",N-1,s,R);
	} else {
		printf("n = %llu\n  n-1 = %llu\n  s = %d\n  r = %llu\n",N,N-1,s,R);
	}
	
	for(int i=1;i<=t;i++) {
		result=fread(buff,sizeof(unsigned long int),1,f);
		if(result==0) {
			fputs("Error in reading primesfile\n",stderr);
			exit(1);
		}
		unsigned long int a;
		a = readLong(*buff);
		if(a>(N-1)) {
			printf("maxitr is too large or not enough primes\n");
			return 2;
		}
		uint64_t A = (unsigned long long int)a;
		uint64_t y = pow_mod(A,R,N);
		if(show) {
			printf("  ");
		}
		printf("  Itr %d of %d, a = %llu, y = %llu",i,t,A,y);
		if(y==N-1) {
			printf(" (which is n-1)\n");
		} else {
			printf("\n");
		}
		if(y!=1 && y!=N-1) {
			for(int j=1;(j<=s-1 && y!=N-1);j++) {
				y = pow_mod(y,2,N);
				if(show) {
					printf("  ");
				}
				printf("    j = %d of %d, y = %llu",j,s-1,y);
				if(y==N-1) {
					printf(" (which is n-1)\n");
				} else {
					printf("\n");
				}
				if(y==1) {
					if(show) {
						printf("  ");
					}
					printf("Miller-Rabin found a strong witness %llu\n",A);
					return 0;
				}
			}
			if(y!=N-1) {
				if(show) {
					printf("  ");
				}
				printf("Miller-Rabin found a strong witness %llu\n",A);
				return 0;
			}
		}
	}
	if(show) {
		printf("  ");
	}
	printf("Miller-Rabin declares n to be a prime number\n");
	return 1;
}

BIGNUM* RndOddNum(int k, FILE *rf) {
	int x= ceil((double)k/8);
	unsigned char *buff;
	buff=new unsigned char[x];
	int result=0;
	result=fread(buff,x,1,rf);
	if(result==0) {
		//fputs("Error in reading rndfile\n",stderr);
		return NULL;
	}
	//printf("Read:%x",*buff);
	BIGNUM *bn_n=NULL;
	bn_n = BN_new();
	BN_zero(bn_n);
	BN_bin2bn(buff, x, bn_n);
	BN_set_bit(bn_n, 0);
	BN_set_bit(bn_n, k-1);
	for(int i=k;i<x*8;i++) {
		BN_clear_bit(bn_n, i);
	}
	return bn_n;
	//printf("The value of %u is %s.\n",(unsigned int)*buff,BN_bn2dec(bn_n));
}

void rndsearch(int k, int t, FILE *pf, FILE *rf) {
	int i=1;
	while(true) {
		printf("RANDOM-SEARCH: iteration %d\n",i);
		BIGNUM *bn_n=NULL;
		//bn_n = BN_new();
		bn_n = RndOddNum(k,rf);
		if(bn_n==NULL) {
			break;
		}
		//unsigned long int n1=(unsigned long int)strtoul(BN_bn2dec(bn_n),NULL,0);	
		unsigned long long int n=(unsigned long long int)strtoull(BN_bn2dec(bn_n),NULL,0);	
		//printf("Ret:%llu\n",n);
		int res=trialdiv(n,pf,true);
		if(res==2) {
			break;
		}
		if(res==0) {
			i++;
			rewind(pf);
			continue;
		}
		rewind(pf);
		if(millerrabin(n,t,pf,true)==1) {
			return;
		}
		i++;
	}
}

void maurer(int k, FILE *pf, FILE *rf) {
	while(k<=20) {
		BIGNUM *bn_n=NULL;
		//bn_n = BN_new();
		bn_n = RndOddNum(k,rf);
		unsigned long int n=(unsigned long int)strtoul(BN_bn2dec(bn_n),NULL,0);
		if(trialdiv(n,pf,false)==1) {
			return;
		}
		rewind(pf);
	}	
}

