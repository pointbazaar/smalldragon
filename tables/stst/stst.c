#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "token/TokenKeys.h"

#include "stst.h"

#define STST_INITIAL_CAPACITY 10;

#define ERR_NOT_FOUND "[STST] could not find struct \n"

struct STST {
	//STruct Symbol Table (STST)
	
	//struct STST should be opaque,
	//except in this file, for better encapsulation
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

struct STST* makeSTST(){
	
	struct STST* stst = make(STST);
	
	stst->capacity = STST_INITIAL_CAPACITY;
	stst->lines    = malloc(sizeof(struct STSTLine*) * stst->capacity);
	stst->count    = 0;
	
	return stst;
}

void stst_clear(struct STST* stst){

	for(int i=0;i < stst->count; i++){

		free(stst->lines[i]);
	}
	free(stst->lines);
	
	stst->capacity = STST_INITIAL_CAPACITY;
	stst->lines    = malloc(sizeof(struct STSTLine*) * stst->capacity);
	stst->count    = 0;
}

void stst_fill(struct STST* stst, struct Namespace* ns){
	
	for(int i=0;i < ns->count_structs; i++){

		struct StructDecl* mystruct = ns->structs[i];

		struct STSTLine* line = makeSTSTLine(mystruct, ns->name);

		stst_add(stst, line);
	}
}

void stst_print(struct STST* stst){
	
	char* fmt = "%16s|%16s|\n";
	
	printf("[STST] Struct Symbol Table\n");
	printf(fmt, "struct name", "member name");
	printf("-----------------\n");
	for(int i = 0; i < stst->count; i++){
		struct STSTLine* line = stst->lines[i];
		
		for(int j = 0; j < line->decl->count_members; j++){
			
			struct StructMember* member = line->decl->members[j];
			
			
			printf(fmt, line->name, member->name);
		}
	}
}

struct STSTLine* stst_get(struct STST* stst, char* name){
	
	for(int i = 0; i < stst->count; i++){
		
		struct STSTLine* line = stst->lines[i];
		
		if(strcmp(line->name, name) == 0)
			{ return line; }
	}
	
	printf(ERR_NOT_FOUND);
	printf("\t%s\n", name);
	exit(1);
	return NULL;
}

void freeSTST(struct STST* stst){
	
	for(int i=0;i < stst->count; i++)
		{ free(stst->lines[i]); }
	
	free(stst->lines);
	
	free(stst);
}

struct STSTLine* makeSTSTLine(struct StructDecl* s, char* _namespace){
	
	struct STSTLine* line = make(STSTLine);

	line->decl       = s;
	line->is_private = has_annotation(s->super.annotations, ANNOT_PRIVATE);
	
	strncpy(line->_namespace, _namespace,                    DEFAULT_STR_SIZE);
	strncpy(line->name,       s->type->structType->typeName, DEFAULT_STR_SIZE);
	
	return line;
}

void stst_add(struct STST* stst, struct STSTLine* line){
	
	if(stst->capacity <= stst->count){
		
		stst->capacity *= 2;
		stst->lines = realloc(stst->lines, sizeof(struct STSTLine*) * stst->capacity);
	}

	stst->lines[stst->count] = line;
	stst->count += 1;
}

struct Type* stst_get_member_type(struct STST* stst, char* struct_name, char* member_name){
		
	struct STSTLine* line = stst_get(stst, struct_name);

	for(int j=0; j < line->decl->count_members; j++){

		struct StructMember* member = line->decl->members[j];

		if(strcmp(member->name, member_name) == 0){

			return member->type;
		}
	}

	char* msg = "[STST] could not find type of member '%s' of '%s'\n";
	printf(msg, member_name, struct_name);
	
	stst_print(stst);
	exit(1);
}
