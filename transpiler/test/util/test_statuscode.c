#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "test_statuscode.h"

#include "flags/flags.h"
#include "transpiler.h"

void clean();

char* FNAME_DEFAULT = "test.dg";

int sourceToStatus(char* src, bool debug){
	
	clean();
	
	FILE* file = fopen(FNAME_DEFAULT,"w");
	
	if(file == NULL){
		printf("could not open output file %s\n", FNAME_DEFAULT);
		return -1;
	}
	
	fprintf(file, "%s", src);
	fclose(file);
	
	struct Flags* flags = makeFlags2();
	flags->debug = debug;
	flags->count_filenames = 1;
	flags->filenames[0] = FNAME_DEFAULT;
	
	transpileAndCompile(flags);
	
	//#define WEXITSTATUS(x)	(_W_INT(x) >> 8)
	//#define	_W_INT(i)	(i)
	//so we can simply avoid WEXITSTATUS(system(...))	
	int status = system("./a.out") >> 8;
	
	if(!debug){
		//in debug mode, leave the artifacts
		//so we can look at them
		clean();
	}
	
	freeFlags(flags);
	
	return status;
}

void clean(){
	
	// '-f ' option so that it will not give a warning
	system("rm -f test.dg");
	system("rm -f .test.dg.tokens");
	system("rm -f .test.dg.ast");
	system("rm -f test.c");
	system("rm -f test.h");
	system("rm -f a.out");
}
