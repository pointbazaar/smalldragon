
//standard headers
#include <vector>

//project headers
#include "MethodCallNode.hpp"
#include "IASTNode.hpp"
#include "IStatementNode.hpp"
#include "ITermNode.hpp"

class MethodCallNode : IASTNode, IStatementNode, ITermNode {

public:
	string methodName;

	vector<ExpressionNode> arguments = new ArrayList<>();

	MethodCallNode(TokenList tokens) throws Exception {

		TokenList copy = tokens.copy();

		this.methodName = new IdentifierNode(copy).identifier;

		copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

		//while there is no ')' up, continue parsing arguments
		IToken next = copy.get(0);
		while (!(next instanceof RParensToken)) {
			if (arguments.size() > 0) {
				copy.expectAndConsumeOtherWiseThrowException(new CommaToken());
			}
			this.arguments.add(new ExpressionNode(copy));
			next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

		tokens.set(copy);
	}

};