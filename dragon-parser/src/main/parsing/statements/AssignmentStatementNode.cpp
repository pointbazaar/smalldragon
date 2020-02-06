
//standard headers
#include <vector>
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../typenodes/TypeNode.hpp"
#include "../VariableNode.hpp"
#include "../ExpressionNode.hpp"

AssignmentStatementNode::AssignmentStatementNode(TokenList tokens) {
	optional<TypeNode*> optTypeNode1; 

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = optional<TypeNode*>(new TypeNode(copy2));
		copy.set(copy2);
	} catch (string e) {
		optTypeNode1 = nullopt;
		//pass
	}

	this->optTypeNode = optTypeNode1;
	this->variableNode = new VariableNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( BaseToken(OPERATOR,"="));

	this->expressionNode = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( BaseToken(SEMICOLON));

	tokens.set(copy);
}

