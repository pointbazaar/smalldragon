#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ASTWriter.hpp"

void write(NamespaceNode nsn, ofstream* file){

	*file << nsn.srcPath;
	*file << "\t";
	*file << nsn.name;
	*file << "\t";

	*file << nsn.methods.size();
	for(MethodNode m : nsn.methods){
		write(m,file);
	}
}


void write(MethodNode m, ofstream* file){

	*file << m.isPublic;
	*file << "\t";
	*file << m.hasSideEffects;
	*file << "\t";

	write( *(m.returnType),file);

	*file << m.methodName;
	*file << "\t";

	*file << m.arguments.size();
	for(DeclaredArgumentNode* arg : m.arguments){
		write(*arg,file);
	}

	*file << m.statements.size();
	for(StatementNode* s : m.statements){
		write(*s,file);
	}
}


void write(DeclaredArgumentNode m, ofstream* file){

	write(*(m.type),file);
	
	if(m.name.has_value()){
		*file << m.name.value();
	}else{
		*file << "NULL";
	}
	*file << "\t";
}

void write(StatementNode m, ofstream* file){

	//the reader has to know which type it is,
	//we can print a small number

	if(m.m1 != NULL){*file << "1" << "\t"; write(*m.m1,file); }
	if(m.m2 != NULL){*file << "2" << "\t"; write(*m.m2,file); }
	if(m.m3 != NULL){*file << "3" << "\t"; write(*m.m3,file); }
	if(m.m4 != NULL){*file << "4" << "\t"; write(*m.m4,file); }
	if(m.m5 != NULL){*file << "5" << "\t"; write(*m.m5,file); }
}
void write(IfStatementNode m, ofstream* file){
	//TODO
}
void write(WhileStatementNode m, ofstream* file){
	//TODO
}
void write(ReturnStatementNode m, ofstream* file){
	//TODO
}
void write(AssignmentStatementNode m, ofstream* file){
	//TODO
}
void write(MethodCallNode m, ofstream* file){
	//TODO
}


void write(TypeNode m, ofstream* file){
	//TODO
}

void write_ast(string filename, NamespaceNode namespaceNode){
	ofstream file;
	file.open(filename, ios::out);

	//TODO: generate our binary AST Format
	string str = "TODO: generate our custom AST format";

	write(namespaceNode,&file);

	file.close();
}