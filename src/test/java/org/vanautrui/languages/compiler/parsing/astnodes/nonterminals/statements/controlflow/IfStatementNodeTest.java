package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;

public class IfStatementNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("if"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new OperatorToken("<"));
        list.add(new IntegerNonNegativeConstantToken(3));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        IfStatementNode ifStatement = new IfStatementNode(list);
    }
}