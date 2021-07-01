#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "free_ast.h"

//this file also contains static asserts
//about the sizes of AST structures.
//this is because any program which uses these
//AST structs has to also free them.

void freeAST(struct AST* ast) {

	for(int i=0; i < ast->count_namespaces; i++) {
		freeNamespace(ast->namespaces[i]);
	}
	free(ast->namespaces);
	free(ast);
}

void freeBoolConst(struct BoolConst* bc) {
	
	free(bc);
}

void freeCharConst(struct CharConst* cc) {
	
	free(cc);
}

void freeDeclArg(struct DeclArg* da) {
	
	freeType(da->type);
	free(da);
}


void freeExpr(struct Expr* expr) {
	
	freeUnOpTerm(expr->term1);
	if(expr->op != NULL) {
		freeOp(expr->op);
		freeUnOpTerm(expr->term2);
	}
	
	free(expr);
}


void freeFloatConst(struct FloatConst* fc) { free(fc); }
void freeIdentifier(struct Identifier* id) { free(id); }
void freeIntConst(struct IntConst* ic) { free(ic); }
void freeHexConst(struct HexConst* hc){ free(hc); }
void freeBinConst(struct BinConst* hc){ free(hc); }

void freeMethod(struct Method* m) {

	freeType(m->returnType);
	for(int i=0; i < m->count_args; i++) {
		freeDeclArg(m->args[i]);
	}
	free(m->args);

	freeStmtBlock(m->block);
	free(m);
}

void freeNamespace(struct Namespace* ns) {

	for(int i=0; i < ns->count_methods; i++) {
		freeMethod(ns->methods[i]);
	}

	for(int i=0; i < ns->count_structs; i++) {
		freeStructDecl(ns->structs[i]);
	}

	free(ns->methods);
	free(ns->structs);
	
	free(ns->src_path);
	free(ns->ast_path);

	free(ns);
}

void freeSimpleVar(struct SimpleVar* sv) {

	for(int i=0;i < sv->count_indices; i++){
		freeExpr(sv->indices[i]);
	}
	free(sv->indices);
	free(sv);
}

void freeStmtBlock(struct StmtBlock* block) {

	for(int i=0; i < block->count; i++) {
		freeStmt(block->stmts[i]);
	}
	free(block->stmts);
	free(block);
}

void freeStructDecl(struct StructDecl* sd) {

	freeSimpleType(sd->type);
	for(int i=0; i < sd->count_members; i++) {
		freeStructMember(sd->members[i]);
	}
	free(sd);
}

void freeStructMember(struct StructMember* sm) {
	freeType(sm->type);
	free(sm);
}
void freeTerm(struct Term* t) {

	switch(t->kind){
		case  1: freeBoolConst  (t->ptr.m1); 	break;
		case  2: freeIntConst   (t->ptr.m2); 	break;
		case  3: freeCharConst  (t->ptr.m3); 	break;
		case  4: freeCall       (t->ptr.m4); 	break;
		case  5: freeExpr       (t->ptr.m5); 	break;
		case  6: freeVariable   (t->ptr.m6); 	break;
		case  7: freeFloatConst (t->ptr.m7); 	break;
		case  8: freeStringConst(t->ptr.m8); 	break;
		case  9: freeHexConst   (t->ptr.m9); 	break;
		case 10: freeBinConst   (t->ptr.m10); 	break;
		case 11: freeLambda     (t->ptr.m11);   break;
		default:
			printf("Error in freeTerm(...)\n");
			free(t);
			exit(1);
	}
	free(t);
}
void freeUnOpTerm(struct UnOpTerm* t) {

	freeOp(t->op);
	freeTerm(t->term);
	free(t);
}

void freeVariable(struct Variable* var) {

	freeSimpleVar(var->simpleVar);

	if(var->memberAccess != NULL){

		freeVariable(var->memberAccess);
	}

	free(var);
}

void freeAssignStmt(struct AssignStmt* as) {

	if(as->optType != NULL) {
		freeType(as->optType);
	}
	freeVariable(as->var);
	freeExpr(as->expr);

	free(as);
}


void freeIfStmt(struct IfStmt* is) {

	freeExpr(is->condition);

	freeStmtBlock(is->block);
	if(is->elseBlock != NULL) {
		freeStmtBlock(is->elseBlock);
	}

	free(is);
}

void freeLoopStmt(struct LoopStmt* ls) {

	freeExpr(ls->count);
	freeStmtBlock(ls->block);
	free(ls);
}

void freeCall(struct Call* mc) {

	for(int i=0; i < mc->count_args; i++) {
		freeExpr(mc->args[i]);
	}
	free(mc->args);

	free(mc);
}

void freeRetStmt(struct RetStmt* rs) {

	if(rs->returnValue != NULL) {
		freeExpr(rs->returnValue);
	}
	free(rs);
}

void freeStmt(struct Stmt* s) {

	switch(s->kind){
		
		case 99: /* nothing to do here */  break;
		case 0: freeLoopStmt(s->ptr.m0);   break;
		case 1: freeCall(s->ptr.m1);       break;
		case 2: freeWhileStmt(s->ptr.m2);  break;
		case 3: freeIfStmt(s->ptr.m3);     break;
		case 4: freeRetStmt(s->ptr.m4);    break;
		case 5: freeAssignStmt(s->ptr.m5); break;
		case 7: freeForStmt(s->ptr.m7);    break;
		case 8: freeSwitchStmt(s->ptr.m8); break;
		default:
			printf("Error in freeStmt\n");
			free(s);
			exit(1);
	}
	free(s);
}


void freeWhileStmt(struct WhileStmt* ws) {
	freeExpr(ws->condition);
	freeStmtBlock(ws->block);
	free(ws);
}

void freeArrayType(struct ArrayType* at) {
	freeType(at->element_type);
	free(at);
}

void freeBasicTypeWrapped(struct BasicTypeWrapped* btw) {

	if(btw->simpleType != NULL) {
		freeSimpleType(btw->simpleType);

	} else if(btw->subrType != NULL) {
		freeSubrType(btw->subrType);
	}

	free(btw);
}

void freeSimpleType(struct SimpleType* st) {
	
	if(st->primitiveType != NULL)
		{ freePrimitiveType(st->primitiveType); }
		
	if(st->structType != NULL)
		{ freeStructType(st->structType); }
	
	free(st);
}

void freeSubrType(struct SubrType* st) {
	
	freeType(st->returnType);
	for(int i=0; i < st->count_argTypes; i++) {
		freeType(st->argTypes[i]);
	}
	free(st->argTypes);
	free(st);
}

void freeType(struct Type* t) {

	if(t->m1 != NULL) {
		freeBasicTypeWrapped(t->m1);
	} else if(t->m2 != NULL) {
		freeTypeParam(t->m2);
	} else if(t->m3 != NULL) {
		freeArrayType(t->m3);
	}
	free(t);
}

void freeTypeParam(struct TypeParam* tp) { free(tp); }

void freePrimitiveType(struct PrimitiveType* p){ free(p); }

void freeStructType(struct StructType* s){

	if(s->typeParamCount != 0)
		{ free(s->typeParams); }
		
	free(s);
}

void freeOp(struct Op* op){ free(op); }

void freeStringConst(struct StringConst* s){
	free(s->value);
	free(s);
}

void freeRange(struct Range* range){
	freeExpr(range->start);
	freeExpr(range->end);
	free(range);
}

void freeLambda(struct Lambda* l){
	for(uint8_t i = 0; i < l->count_params; i++){
		freeIdentifier(l->params[i]);
	}
	freeExpr(l->expr);
	free(l);
}

void freeForStmt(struct ForStmt* fstmt){
	freeRange(fstmt->range);
	freeStmtBlock(fstmt->block);
	free(fstmt);
}

void freeSwitchStmt(struct SwitchStmt* s){
	
	freeExpr(s->expr);
	for(int i=0; i < s->count_cases; i++){
		freeCaseStmt(s->cases[i]);
	}
	free(s->cases);
	free(s);
}

void freeCaseStmt(struct CaseStmt* c){
	if(c->block != NULL){
		freeStmtBlock(c->block);
	}
	switch(c->kind){
		case 0: freeBoolConst(c->ptr.m1); break;
		case 1: freeCharConst(c->ptr.m2); break;
		case 2: freeIntConst(c->ptr.m3); break;
		default:
			printf("Error in freeCaseStmt\n");
			exit(1);
	}
	free(c);
}
