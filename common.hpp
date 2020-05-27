#pragma once


#include <errno.h> 		//perror

#define PERROR(sigal) {perror(sigal);exit(1);}
#define OUT(str) cout<<str<<endl;