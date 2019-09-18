package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;

public class AccessModifierNode implements IASTNode {

    public final boolean is_public;

    public AccessModifierNode(TokenList tokens) throws Exception {
        IToken token1 = tokens.get(0);
        if (token1 instanceof AccessModifierToken) {
            this.is_public = ((AccessModifierToken) token1).is_public;
            tokens.consume(1);
        } else {
            //otherwise, it is just public. no access modifier is also an access modifier
            this.is_public = true;
        }
    }

    @Override
    public String toSourceCode() {
        return (is_public) ? "public" : "private";
    }
}