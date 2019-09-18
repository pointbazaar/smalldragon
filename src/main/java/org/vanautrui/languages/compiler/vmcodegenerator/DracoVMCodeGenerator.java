package org.vanautrui.languages.compiler.vmcodegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;
import java.util.Random;
import java.util.Set;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.SubroutineDracoVMCodeGenerator.generateDracoVMCodeForMethod;

public class DracoVMCodeGenerator {

    public static List<String> generateDracoVMCode(Set<AST> asts, SubroutineSymbolTable subTable) throws Exception{

        DracoVMCodeWriter sb = new DracoVMCodeWriter();
        for(AST ast :asts){
            for(ClassNode classNode : ast.classNodeList){
                for(MethodNode methodNode : classNode.methodNodeList){
                    generateDracoVMCodeForMethod(methodNode,sb,subTable);
                }
                if(classNode.fieldNodeList.size()>0){
                    throw new Exception("unhandled case in dracovm code generation");
                }
            }
        }
        return sb.getDracoVMCodeInstructions();
    }

    /**
     * @param assignStmt the AssignmentStatementNode being compiled
     * @param sb         the VM Code Writer class
     * @param varTable   the Local Variable Symbol Table
     * @throws Exception if the variable is not in the symbol table
     */
    public static void genVMCodeForAssignmentStatement(AssignmentStatementNode assignStmt, DracoVMCodeWriter sb,SubroutineSymbolTable subTable,LocalVarSymbolTable varTable) throws Exception {

        //the variable being assigned to would be a local variable or argument.
        //the expression that is being assigned, there can be code generated to put it on the stack
        final String varName = assignStmt.variableNode.name;
        final String segment = varTable.getSegment(varName);
        final int index = varTable.getIndexOfVariable(varName);

        if(assignStmt.variableNode.indexOptional.isPresent()){
            //push the array address on the stack
            sb.push(segment,index);

            //push the index
            genDracoVMCodeForExpression(assignStmt.variableNode.indexOptional.get(),sb,subTable,varTable);

            //push the value we want to store
            genDracoVMCodeForExpression(assignStmt.expressionNode,sb,subTable,varTable);

            sb.arraystore();
        }else {
            genDracoVMCodeForExpression(assignStmt.expressionNode,sb,subTable,varTable);
            //then we just pop that value into the appropriate segment with the specified index
            sb.pop(segment, index);
        }
    }


    public static void genVMCodeForFloatConst(FloatConstNode fconst,DracoVMCodeWriter sb){
        sb.fconst(fconst.value);
    }

    public static void genVMCodeForIntConst(int iconst,DracoVMCodeWriter sb){
        sb.iconst(iconst);
    }

    public static void genVMCodeForBoolConst(BoolConstNode bconst,DracoVMCodeWriter sb){
        sb.iconst((bconst.value)?1:0);
    }

    public static void genDracoVMCodeForTerm(TermNode tNode,DracoVMCodeWriter sb,SubroutineSymbolTable subTable,LocalVarSymbolTable varTable)throws Exception{
        ITermNode t = tNode.termNode;
        if(t instanceof FloatConstNode){
            genVMCodeForFloatConst((FloatConstNode)t,sb);
        }else if(t instanceof IntConstNode){
            genVMCodeForIntConst(((IntConstNode)t).value,sb);
        }else if(t instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode)t;
            genDracoVMCodeForExpression(expressionNode,sb,subTable,varTable);
        }else if(t instanceof VariableNode) {
            //find the local variable index
            // and push the variable onto the stack
            VariableNode variableNode = (VariableNode) t;
            genDracoVMCodeForVariable(variableNode,sb,subTable,varTable);
        }else if(t instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode)t;
            genVMCodeForMethodCall(methodCallNode,sb,subTable,varTable);

        }else if(t instanceof BoolConstNode) {
            genVMCodeForBoolConst((BoolConstNode)t,sb);
        }else if(t instanceof ArrayConstantNode) {
            ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
            genVMCodeForArrayConstant(arrayConstantNode,sb,subTable,varTable);
        }else if (t instanceof CharConstNode) {
            CharConstNode t1 = (CharConstNode) t;
            genVMCodeForIntConst((int)t1.content,sb);
        }else{
            throw new Exception("unhandled case");
        }
    }

    /**
     * after this subroutine, the address of the array with the specified elements inside is on the stack
     * @param arrayConstantNode
     * @param sb
     * @param subTable
     * @param varTable
     * @throws Exception
     */
    private static void genVMCodeForArrayConstant(ArrayConstantNode arrayConstantNode, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{

        //allocate space for the new array.
        //this leaves the address of the new array (the new array resides on the heap) on the stack
        sb.malloc(arrayConstantNode.elements.size(),"malloc some space for a new array");

        //put the individual elements into the array
        for(int i=0;i<arrayConstantNode.elements.size();i++) {
            sb.dup(); //duplicate the array address as it is consumed with  'arraystore'

            sb.iconst(i);//index to store into

            //value we want to store
            genDracoVMCodeForExpression(arrayConstantNode.elements.get(i),sb,subTable,varTable);

            sb.arraystore();
        }
    }

    public static void genDracoVMCodeForVariable(VariableNode variableNode, DracoVMCodeWriter sb,SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //push the variable on the stack

        //if the variable is local,
        //or argument, that would be different segments
        //we have to consider its index

        String name = variableNode.name;

        int index=varTable.getIndexOfVariable(name);
        String segment = varTable.getSegment(name);

        sb.push(segment,index);

        if(variableNode.indexOptional.isPresent()){
            //it is an array and we should read from the index
            genDracoVMCodeForExpression(variableNode.indexOptional.get(),sb,subTable,varTable);
            sb.arrayread();
        }
    }


    public static void genDracoVMCodeForOp(OperatorNode opNode,DracoVMCodeWriter sb)throws Exception{
        switch (opNode.operator){
            case "+":
                sb.add();
                break;
            case "-":
                sb.sub();
                break;
            case ">":
                sb.gt();
                break;
            case "<":
                sb.lt();
                break;
            case "*":
                sb.mul();
                break;
            case "/":
                sb.div();
                break;
            default:
                throw new Exception("currently unsupported op "+opNode.operator);
        }
    }



    public static long unique(){
        //uniqueness for jump labels
        Random r = new Random();
        return Math.abs(r.nextInt(100000));
    }
}