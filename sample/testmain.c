#include <stdio.h>
#include "testsrc1.h"

//! mode: exe
//! flag: -O3
//! link: m
//! out: testmain
//! src: testmain.c, testsrc1.c
int main(void)
{
	foo();
	return 0;
}

