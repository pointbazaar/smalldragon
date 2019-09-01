package org.vanautrui.languages.codegeneration.dracovmbackend;

import org.vanautrui.languages.parsing.astnodes.ITermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class DracoVMCodeGenerator {

    public static List<String> generateDracoVMCode(Set<AST> asts, SubroutineSymbolTable subTable) throws Exception{

        List<String> sb =new ArrayList<>();
        //TODO:
        for(AST ast :asts){
            for(ClassNode classNode : ast.classNodeList){
                for(MethodNode methodNode : classNode.methodNodeList){
                    generateDracoVMCodeForMethod(methodNode,sb);
                }
                if(classNode.fieldNodeList.size()>0){
                    throw new Exception("unhandled case in dracovm code generation");
                }
            }
        }

        return sb;
    }

    private static void generateDracoVMCodeForMethod(MethodNode m, List<String> sb)throws Exception{
        //TODO
        sb.add("subroutine "+m.methodName.methodName.name+" "+m.arguments.size());
        //not sure if it is number of arguments or number of local vars
        for(StatementNode stmt : m.statements){
            generateDracoVMCodeForStatement(stmt,sb);
        }
    }

    private static void generateDracoVMCodeForStatement(StatementNode stmt,List<String> sb)throws Exception{
        IStatementNode snode = stmt.statementNode;
        if(snode instanceof MethodCallNode){
            MethodCallNode call = (MethodCallNode)snode;
            genVMCodeForMethodCall(call,sb);
        }else if(snode instanceof LoopStatementNode) {
            LoopStatementNode loop = (LoopStatementNode) snode;
            //TODO
        }else if(snode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
            //TODO
        }else if(snode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode =(WhileStatementNode)snode;
            //TODO
        }else if(snode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) snode;
            //TODO
        }else if(snode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)snode;
            genDracoVMCodeForReturn(returnStatementNode,sb);
        }else{
            throw new Exception("unconsidered statement type: "+stmt.statementNode.getClass().getName());
        }
    }

    private static void genVMCodeForFloatConst(FloatConstNode fconst,List<String> sb){
        sb.add("fpush "+fconst.value);
    }

    private static void genVMCodeForIntConst(int iconst,List<String> sb){
        sb.add("ipush "+iconst);
    }

    private static void genVMCodeForBoolConst(BoolConstNode bconst,List<String> sb){
        sb.add("ipush "+((bconst.value)?1:0));
    }

    private static void genDracoVMCodeForTerm(TermNode tNode,List<String> sb)throws Exception{
        //TODO
        ITermNode t = tNode.termNode;
        if(t instanceof FloatConstNode){
            genVMCodeForFloatConst((FloatConstNode)t,sb);
        }else if(t instanceof IntConstNode){
            genVMCodeForIntConst(((IntConstNode)t).value,sb);
        }else if(t instanceof StringConstNode){
            StringConstNode stringConstNode = (StringConstNode)t;
            //push the string on the stack
            //TODO
            throw new Exception("unhandled case");
        }else if(t instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode)t;
            genDracoVMCodeForExpression(expressionNode,sb);
            throw new Exception("currently unhandled case ");
        }else if(t instanceof VariableNode) {
            //find the local variable index
            // and push the variable onto the stack
            VariableNode variableNode = (VariableNode) t;
            //TODO
            throw new Exception("unhandled case");
        }else if(t instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode)t;
            genVMCodeForMethodCall(methodCallNode,sb);
            throw new Exception("unhandled case");
        }else if(t instanceof BoolConstNode) {
            genVMCodeForBoolConst((BoolConstNode)t,sb);
        }else if(t instanceof ArrayConstantNode) {
            ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
            //TODO
            throw new Exception("currently unhandled case");
        }else if (t instanceof CharConstNode) {
            CharConstNode t1 = (CharConstNode) t;
            genVMCodeForIntConst((int)t1.content,sb);
        }else{
            throw new Exception("unhandled case");
        }
    }

    private static void genVMCodeForMethodCall(MethodCallNode methodCallNode, List<String> sb) {
        sb.add("call "+methodCallNode.identifierMethodName);
    }

    private static void genDracoVMCodeForOp(OperatorNode opNode,List<String> sb)throws Exception{
        switch (opNode.operator){
            case "+":
                sb.add("add");
                break;
            case "-":
                sb.add("sub");
                break;
            default:
                throw new Exception("currently unsupported op "+opNode.operator);
        }
    }

    private static void genDracoVMCodeForExpression(ExpressionNode expr,List<String> sb)throws Exception{
        genDracoVMCodeForTerm(expr.term,sb);
        for(int i=0;i<expr.termNodes.size();i++){
            genDracoVMCodeForTerm(expr.termNodes.get(i),sb);
            genDracoVMCodeForOp(expr.operatorNodes.get(i),sb);
        }
    }

    private static void genDracoVMCodeForReturn(ReturnStatementNode retStmt,List<String> sb)throws Exception {
        genDracoVMCodeForExpression(retStmt.returnValue,sb);
        sb.add("return");
    }
}
