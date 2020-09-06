#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "RangeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/parsing/Range.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

void range_test1(bool debug){
	
	if(debug){
		printf("TEST: range_test1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"a"));
	
	list_add(list, makeToken2(RANGEOP,".."));

	list_add(list, makeToken2(INTEGER,"0"));


	list_add(list, makeToken(SEMICOLON));

	struct Range* r = makeRange(list, debug);
	
	assert(r != NULL);
	assert(r->start != NULL);
	assert(r->end != NULL);
	
	freeTokenList(list);
	freeRange(r);
}