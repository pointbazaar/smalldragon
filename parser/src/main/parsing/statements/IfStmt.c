#include <stdio.h>
#include <stdlib.h>

#include "../StmtBlock.h"
#include "IfStmt.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../Expr.h"
#include "Stmt.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct IfStmt* initIfStmt();
void freeIncomplete(struct IfStmt* ifstmt);
//--------------------------

struct IfStmt* makeIfStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("IfStmt(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 3){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, IF)){
		return NULL;
	}
	
	struct IfStmt* res = initIfStmt();

	if((res->condition = makeExpr(copy,debug)) == NULL){
		freeIncomplete(res);
		return NULL;
	}

	if((res->block = makeStmtBlock(copy, debug)) == NULL){
		freeIncomplete(res);
		return NULL;
	}

	if (list_expect(copy, ELSE)) {

		if((res->elseBlock = makeStmtBlock(copy, debug)) == NULL){
			freeIncomplete(res);
			return NULL;
		}
	}
	
	if(debug){
		printf("sucess parsing IfStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct IfStmt* initIfStmt(){
	
	struct IfStmt* res = smalloc(sizeof(struct IfStmt));
	
	res->condition = NULL;
	res->block     = NULL;
	res->elseBlock = NULL;
	
	return res;
}

void freeIncomplete(struct IfStmt* is){
	//free an IfStmt, even if it has not been
	//completely initialized
	if(is->condition != NULL){
		freeExpr(is->condition);
	}
	if(is->block != NULL){
		freeStmtBlock(is->block);
	}
	if(is->elseBlock != NULL){
		freeStmtBlock(is->elseBlock);
	}
	free(is);
}
