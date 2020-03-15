#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "IntConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/smalloc.h"

struct IntConst* makeIntConst(struct TokenList* tokens, bool debug) {

	struct IntConst* res = smalloc(sizeof(struct IntConst));

	if(debug){
		printf("IntConst(...) from %s\n", list_code(tokens));
	}

	struct TokenList* copy = makeTokenList(tokens);

	struct Token* tk = list_get(copy, 0);
	if(tk == NULL){return NULL;}

	switch (tk->kind){

		case OPKEY: 
			;
			if (
				strcmp(tk->value,"-") == 0 
				&& (list_get(copy, 1)->kind == INTEGER)

			) {
				struct Token* mytk = list_get(copy,1);
				if(mytk == NULL){return NULL;}
				
				res->value = - atoi( mytk->value );
				if(!list_consume(copy, 2)){return NULL;}
			} else {
				// "cannot parse integer constant node with such operator:" + tk->value;
				return NULL;
			}
			break;

		case INTEGER: 
			;
			res->value = atoi(tk->value);
			if(!list_consume(copy, 1)){return NULL;}
			break;

		default:
			;
			// "could not read IntConst node";
			return NULL;
	}

	list_set(tokens, copy);

	return res;
}
