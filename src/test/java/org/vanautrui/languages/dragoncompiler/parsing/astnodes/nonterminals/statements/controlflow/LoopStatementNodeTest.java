package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;

public class LoopStatementNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("loop"));
        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        LoopStatementNode loop = new LoopStatementNode(list);
    }
}
