
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeNode.hpp"
#include "ITypeNode.hpp"

class TypeNode {
public:
	ITypeNode typeNode;

	TypeNode(ITypeNode typeNode) {
		this.typeNode = typeNode;
	}

	TypeNode(TokenList tokens){

		TokenList copy = tokens.copy();

		try {
			this.typeNode = new ArrayTypeNode(copy);
		} catch (string e) {
			try {
				this.typeNode = new TypeParameterNode(copy);
			} catch (Exception e2) {
				this.typeNode = new BasicTypeWrappedNode(copy);
			}
		}

		tokens.set(copy);
	}

};