package org.vanautrui.languages.symboltables.rows;

public class LocalVarSymbolTableRow implements ISymbolTableRow {

    public final String varName;
    public final String typeName;

    public final String kind;

    public final static String KIND_LOCALVAR="LOCAL";
    public final static String KIND_ARGUMENT="ARG";

    public LocalVarSymbolTableRow(String varName, String typeName,String kind){

        this.typeName=typeName;
        this.varName=varName;
        this.kind=kind;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s | %8s |",varName,typeName,kind);
    }



    @Override
    public String getName() {
        return this.varName;
    }

    @Override
    public String getType() {
        return this.typeName;
    }
}
