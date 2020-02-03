
//standard headers
#include <vector>
#include <set>
#include <string>

//project headers
#include "IdentifierNode.hpp"
#include "../lexing/TokenList.hpp"
#include "../lexing/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

IdentifierNode::IdentifierNode(TokenList tokens) {

	BaseToken token = tokens.get(0);

	if (token.kind == IDENTIFIER) {
		this.identifier = token.getContents();
		tokens.consume(1);

	} else {
		String msg = "could not read identifier from token: " + token.getContents();
		msg += " in " + tokens.relPath + ":" + token.getLineNumber();
		msg += " in context '" + tokens.toSourceCodeFragment().substr(0, Math.min(20, tokens.toSourceCodeFragment().size())) + "'";

		throw (msg);
	}

}
