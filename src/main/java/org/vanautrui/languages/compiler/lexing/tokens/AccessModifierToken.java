package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

public class AccessModifierToken implements IToken {

    public boolean is_public;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public AccessModifierToken(CharacterList list) throws Exception {

        this.lineNumber=list.getCurrentLineNumber();
        if (list.startsWith("public")) {
            this.is_public = true;
            list.consumeTokens("public".length());
        } else if (list.startsWith("private")) {
            this.is_public = false;
            list.consumeTokens("private".length());
        } else {
            throw new Exception("could not recognize access modifier token");
        }
    }

    public AccessModifierToken(String newcontents) throws Exception {
        this.lineNumber=0;
        switch (newcontents) {
            case "public":
                is_public = true;
                break;
            case "private":
                is_public = false;
                break;
            default:
                throw new Exception("invalid acces modifier token: \"" + newcontents + "\"");
        }
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return (this.is_public) ? "public" : "private";
    }



}