#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../../util/util.h"

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: %s\n", list_code(tokens, debug));
	}

	struct SimpleType* res = smalloc(sizeof(struct SimpleType));
	
	strcpy(res->typeName, "");

	if(list_size(tokens) == 0){
		free(res);
		return NULL;
	}

	struct Token* token = list_head(tokens);

	if(debug){
		printf("\tinspect token kind\n");
	}
	if (token->kind == TYPEIDENTIFIER) {
		strcpy(res->typeName, token->value);
	} else if (token->kind == ANYTYPE) {
		strcpy(res->typeName, "#");
	} else {
		if(debug){
			printf("Error: could not read simple type identifier\n");
		}
		free(res);
		return NULL;
	}

	list_consume(tokens, 1);

	if(debug){
		printf("\tsuccess parsing SimpleType\n");
	}

	return res;
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = smalloc(sizeof(struct SimpleType));

	strcpy(res->typeName, typeName);

	return res;
}

void freeSimpleType(struct SimpleType* st){
	printf("DEBUG: freeSimpleType\n");
	//TODO:
	//free(st);
	printf("DEBUG: freeSimpleType done\n");
}
