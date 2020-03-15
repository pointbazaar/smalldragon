#include "RetStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/statements/RetStmt.hpp"

#include <stdio.h>

int retstmt_test1(bool debug) {

	printf("retstmt_test1\n");

	try {
		TokenList list = TokenList();
		list.add(RETURN);

		list.add(LPARENS);

		list.add(Token(OPKEY,"-"));
		list.add(Token(INTEGER,"5"));

		list.add(RPARENS);

		list.add(SEMICOLON);

		struct RetStmt* r = makeRetStmt(&list,debug);

		return 1;
	}catch (string e){
		return 0;
	}
}

int retstmt_test2(bool debug){

	printf("retstmt_test2\n");

	try {
		TokenList list = TokenList();
		list.add(RETURN);

		list.add(LPARENS);

		list.add(Token(OPKEY,"-"));

		list.add(Token(INTEGER,"5"));

		list.add(RPARENS);

		list.add(Token(OPKEY,"*"));

		list.add(Token(ID,"n"));

		list.add(SEMICOLON);

		struct RetStmt* r = makeRetStmt(&list,debug);

		return 1;
	}catch (string e){
		return 0;
	}
}

int retstmt_test3(bool debug) {

	printf("retstmt_test3\n");

	try {
		TokenList list = TokenList();

		list.add(RETURN);

		list.add(ID,"arr");

		list.add(LBRACKET);
		list.add(INTEGER,"0");

		list.add(RBRACKET);

		list.add(SEMICOLON);

		struct RetStmt* r = makeRetStmt(&list,debug);
		return 1;
	}catch (string e){
		return 0;
	}
}