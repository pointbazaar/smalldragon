#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "../cg.h"
#include "cg_subr.h"
#include "code_gen/c_code_gen/types/cg_types.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/stst/stst.h"

#include "analyzer/lv/lv_analyzer.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

void transpileMethod(struct Method* m, struct Ctx* ctx){
	
	//create the local variable symbol table
	lvst_clear(ctx->tables->lvst);
	lvst_fill(m, ctx->tables);
	if (ctx->flags->debug){ lvst_print(ctx->tables->lvst); }

	transpileMethodSignature(m, ctx);
	
	ctx->index_try_stmt = 0;

	transpileStmtBlock(m->block, ctx);
}

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
	transpileType(m->decl->return_type, ctx);

	fprintf(ctx->file, " %s(", m->decl->name);

	for(int i=0; i < m->decl->count_args; i++){
		transpileDeclArg(m->decl->args[i], ctx);
		if(i < (m->decl->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}
	
	
	//maybe sneak in a 
	//jmp_buf* _jb
	//argument if the relevant function throws
	if(m->decl->throws){
		if(m->decl->count_args > 0){
			fprintf(ctx->file, ", ");
		}
		fprintf(ctx->file, "jmp_buf* _jb");
	}

	fprintf(ctx->file, ")");
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){
		
	assert(da != NULL);
		
	bool isSubrType = false;
	//is it a function pointer?
	if(da->type->m1 != NULL){
		if(da->type->m1->subr_type != NULL){
			isSubrType = true;
			strncpy(
				ctx->current_function_pointer_var_or_arg_name,
				da->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(da->type, ctx);

	if(da->has_name && !isSubrType){
		//if it has a name, and is a subroutine type
		//(function pointer), then the name
		//is transpiled by transpileSubrType
		fprintf(ctx->file, " %s", da->name);
	}
}
