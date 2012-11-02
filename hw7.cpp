#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdint.h>
#include "primes.h"

using namespace std;

int main(int argc, char *argv[]) {
	if(argc==3) {
		if(strcmp(argv[1],"primes")==0) {
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-n=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())==0) {
					fputs("Invalid Parameter\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())>16777216) {
					fputs("maxval is out of range\n",stderr);
					exit(1);
				}
				sieve((unsigned int)atoi(param.c_str()));
				return 0;
			}	//-n
		}	//primes
	}	//argc==3
	if(argc==4) {
		if(strcmp(argv[1],"trialdiv")==0) {
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-n=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())==0) {
					fputs("Invalid Parameter\n",stderr);
					exit(1);
				}
				unsigned long int n=(unsigned long int)atoi(param.c_str());
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-p=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					FILE *f;
					f=fopen(param.c_str(),"rb");
					if(f==NULL) {
						fputs("Error in opening primesfile\n",stderr);
						exit(1);
					}
					trialdiv(n,f,false);
					return 0;
				}
			}
		}	//trialdiv
	}	//argc==4
	if(argc==5) {
		if(strcmp(argv[1],"millerrabin")==0) {
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-n=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())==0) {
					fputs("Invalid Parameter\n",stderr);
					exit(1);
				}
				unsigned long long int n=(unsigned long long int)strtoull(param.c_str(),NULL,0);
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-t=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					if(atoi(param.c_str())==0) {
						fputs("Invalid Parameter\n",stderr);
						exit(1);
					}
					int t=atoi(param.c_str());
					temp=argv[4];
					option=temp.substr(0,3);
					if(option.compare("-p=")==0) {
						param=temp.substr(3,temp.length());
						if(param.compare("")==0) {
							fputs("Malformed Command\n",stderr);
							exit(1);
						}
						FILE *f;
						f=fopen(param.c_str(),"rb");
						if(f==NULL) {
							fputs("Error in opening primesfile\n",stderr);
							exit(1);
						}
						millerrabin(n,t,f,false);
						return 0;
					}
				}
			}
		}	//millerrabin
		if(strcmp(argv[1],"maurer")==0) {
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())==0) {
					fputs("Invalid Parameter\n",stderr);
					exit(1);
				}
				int k=atoi(param.c_str());
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-p=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					FILE *pf;
					pf=fopen(param.c_str(),"rb");
					if(pf==NULL) {
						fputs("Error in opening primesfile\n",stderr);
						exit(1);
					}
					temp=argv[4];
					option=temp.substr(0,3);
					if(option.compare("-r=")==0) {
						param=temp.substr(3,temp.length());
						if(param.compare("")==0) {
							fputs("Malformed Command\n",stderr);
							exit(1);
						}
						FILE *rf;
						rf=fopen(param.c_str(),"rb");
						if(rf==NULL) {
							fputs("Error in opening rndfile\n",stderr);
							exit(1);
						}
						maurer(k,pf,rf);
						return 0;
					}
				}
			}
		}	//maurer
	}	//argc==5
	if(argc==6) {
		if(strcmp(argv[1],"rndsearch")==0) {
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(atoi(param.c_str())==0) {
					fputs("Invalid Parameter\n",stderr);
					exit(1);
				}
				int k=atoi(param.c_str());
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-t=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					if(atoi(param.c_str())==0) {
						fputs("Invalid Parameter\n",stderr);
						exit(1);
					}
					int t=atoi(param.c_str());
					temp=argv[4];
					option=temp.substr(0,3);
					if(option.compare("-p=")==0) {
						param=temp.substr(3,temp.length());
						if(param.compare("")==0) {
							fputs("Malformed Command\n",stderr);
							exit(1);
						}
						FILE *pf;
						pf=fopen(param.c_str(),"rb");
						if(pf==NULL) {
							fputs("Error in opening primesfile\n",stderr);
							exit(1);
						}
						temp=argv[5];
						option=temp.substr(0,3);
						if(option.compare("-r=")==0) {
							param=temp.substr(3,temp.length());
							if(param.compare("")==0) {
								fputs("Malformed Command\n",stderr);
								exit(1);
							}
							FILE *rf;
							rf=fopen(param.c_str(),"rb");
							if(rf==NULL) {
								fputs("Error in opening rndfile\n",stderr);
								exit(1);
							}
							rndsearch(k,t,pf,rf);
							return 0;
						}
					}
				}
			}
		}	//rndsearch
	}	//argc==6

	fputs("Malformed Command\n",stderr);
	return 1;
}

