#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "VariableTest.h"

#include "var/Variable.h"
#include "var/SimpleVar.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int variable_test_parse_struct_member_access(bool debug) {

	if(debug){
		printf("TEST: variable_test_parse_struct_member_access(...)\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,"."));
	list_add(tokens, makeToken2(ID,"a"));

	struct Variable* v = makeVariable(tokens,debug);
	if(v==NULL){
		return 0;
	}

	assert(NULL != v->memberAccess);
	
	assert(strcmp("x", v->simpleVar->name) == 0);

	assert(strcmp("a", v->memberAccess->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(v);

	return 1;
}

int variable_test_parse_index_access(bool debug) {
	
	if(debug){
		printf("TEST: variable_test_parse_index_access(...)\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken(LBRACKET));
	list_add(tokens, makeToken2(INTEGER,"0"));
	list_add(tokens, makeToken(RBRACKET));

	struct Variable* node = makeVariable(tokens,debug);
	assert(node != NULL);

	assert(NULL == node->memberAccess);
	assert(strcmp("x", node->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(node);

	return 1;
}

int variable_test_parse_struct_member_access_and_index_access(bool debug) {

	if(debug){
		printf("TEST: variable_test_parse_struct_member_access_and_index_access(...)\n");
	}

	// x.a[0].b
	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,".") );
	list_add(tokens, makeToken2(ID,"a") );

	list_add(tokens, makeToken2(LBRACKET,"[") );
	list_add(tokens, makeToken2(INTEGER,"0") );
	list_add(tokens, makeToken2(RBRACKET,"]") );

	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,".") );
	list_add(tokens, makeToken2(ID,"b") );

	struct Variable* node = makeVariable(tokens,debug);
	assert(node != NULL);

	assert(NULL != node->memberAccess);
	assert(strcmp("x", node->simpleVar->name) == 0);
	assert(strcmp("a", node->memberAccess->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(node);

	return 1;
}
