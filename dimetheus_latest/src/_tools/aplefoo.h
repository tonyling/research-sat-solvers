/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef APLEFOO_H_
#define APLEFOO_H_

#include "tools.h"

//This file contains all the definitions for the APLEFOO format.
#define APLEFOO_OP_SYM_NOT '!'	//NOT symbol
#define APLEFOO_OP_SYM_AND '&'	//AND symbol
#define APLEFOO_OP_SYM_XOR '^'	//XOR symbol
#define APLEFOO_OP_SYM_OR '|'	//OR  symbol
#define APLEFOO_OP_SYM_RIM '>'	//RIGHT IMPLICATION symbol
#define APLEFOO_OP_SYM_LIM '<'	//LEFT IMPLICATION  symbol
#define APLEFOO_OP_SYM_EQU '='	//EQUIVALENCE symbol

char aplefoo_op_sym_not;		//These lines denote the operator symbols. They have the above default values in case the
char aplefoo_op_sym_and;		//file makes no statement about them.
char aplefoo_op_sym_xor;
char aplefoo_op_sym_or;
char aplefoo_op_sym_rim;
char aplefoo_op_sym_lim;
char aplefoo_op_sym_equ;

typedef enum {
	APLEFOO_OP_VAL_NULL,
	APLEFOO_OP_VAL_NOT,
	APLEFOO_OP_VAL_AND,
	APLEFOO_OP_VAL_XOR,
	APLEFOO_OP_VAL_OR,
	APLEFOO_OP_VAL_RIM,
	APLEFOO_OP_VAL_LIM,
	APLEFOO_OP_VAL_EQU
} aplefoo_token_op;

typedef enum {
	APLEFOO_TOKEN_TYPE_NULL,	//If the token has no type.
	APLEFOO_TOKEN_TYPE_VAR,		//If the token is a variable.
	APLEFOO_TOKEN_TYPE_OBR, 	//If the token is an opening bracket.
	APLEFOO_TOKEN_TYPE_CBR, 	//If the token is a closing bracket.
	APLEFOO_TOKEN_TYPE_UOP,		//If the token is an unary operator symbol.
	APLEFOO_TOKEN_TYPE_BOP		//If the token is a binary operator symbol.
} aplefoo_token_type;

typedef struct {		//This structure defines what we understand as a token that is provided by the token reader.
	aplefoo_token_type type;//The token type.
	aplefoo_token_op op;	//The operator, in case the token type is APLEFOO_TOKEN_TYPE_OPS (or APLEFOO_OP_VAL_NULL).
	int32_t var;			//The variable ID if the token type is APLEFOO_TOKEN_TYPE_VAR (or 0 otherwise).
} aplefoo_token;

#define APLEFOO_IDENTIFIER_MAXLEN 256	//The maximum size of an identifier we allow.
char* aplefoo_currentIdentifier;		//In case we read a variable identifier, we write it into this string.

aplefoo_token* aplefoo_currentToken;	//The last token read by the token reader.

//The following macro creates a new unary operator token from the given character.
#define APLEFOO_NEWTOKEN_UOP( __char ) {\
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_UOP; \
	aplefoo_currentToken->var = 0;\
	if (__char == aplefoo_op_sym_not) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_NOT; \
	} \
}

//The following macro creates a new binary operator token from the given character.
#define APLEFOO_NEWTOKEN_BOP( __char ) {\
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_BOP; \
	aplefoo_currentToken->var = 0; \
	if (__char == aplefoo_op_sym_and) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_AND; \
	} else if (__char == aplefoo_op_sym_xor) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_XOR; \
	} else if (__char == aplefoo_op_sym_or) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_OR; \
	} else if (__char == aplefoo_op_sym_rim) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_RIM; \
	} else if (__char == aplefoo_op_sym_lim) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_LIM; \
	} else if (__char == aplefoo_op_sym_equ) { \
		aplefoo_currentToken->op = APLEFOO_OP_VAL_EQU; \
	} \
}

//The following macro creates a new opening bracket token.
#define APLEFOO_NEWTOKEN_OBR() { \
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_OBR; \
	aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL; \
	aplefoo_currentToken->var = 0; \
}

//The following macro creates a new closing bracket token.
#define APLEFOO_NEWTOKEN_CBR() { \
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_CBR; \
	aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL; \
	aplefoo_currentToken->var = 0; \
}

//The following macro creates a new variable token.
#define APLEFOO_NEWTOKEN_VAR( __var ) { \
	aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_VAR; \
	aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL; \
	aplefoo_currentToken->var = __var; \
}

//The following macro checks if a given character denotes a unary operator.
#define APLEFOO_ISCHAR_UOP( __char ) (\
	 __char == aplefoo_op_sym_not \
	 ? 1 : 0 \
)

//The following macro checks if a given character denotes a binary operator.
#define APLEFOO_ISCHAR_BOP( __char ) (\
	 __char == aplefoo_op_sym_and || \
	 __char == aplefoo_op_sym_xor || \
	 __char == aplefoo_op_sym_or  || \
	 __char == aplefoo_op_sym_rim || \
	 __char == aplefoo_op_sym_lim || \
	 __char == aplefoo_op_sym_equ \
	 ? 1 : 0 \
)

//The following macro checks if a given character is one of the variable identification symbols in [a-z,A-Z,0-9,_].
#define APLEFOO_ISCHAR_VAR( __char ) ( \
	(__char > 96 && __char < 123) || \
	(__char > 64 && __char < 91) || \
	(__char > 47 && __char < 58) || \
	(__char == 95) \
	? 1 : 0 \
)

//The following macros are used to check if a character is an opening or closing bracket.
#define APLEFOO_ISCHAR_OBR( __char ) ( \
	__char == 40 || \
	__char == 91 || \
	__char == 123 \
	? 1 : 0 \
)

#define APLEFOO_ISCHAR_CBR( __char ) ( \
	__char == 41 || \
	__char == 93 || \
	__char == 125 \
	? 1 : 0 \
)

#define APLEFOO_ISCHAR_BRACKET( __char ) ( APLEFOO_ISCHAR_OBR(__char) || APLEFOO_ISCHAR_CBR(__char) )

//The following macros are used to check if operator A binds stronger or weaker than operator B.
#define APLEFOO_OP_BINDS_STRONGER( __opA, __opB ) ( __opA < __opB ? 1 : 0 )
#define APLEFOO_OP_BINDS_STRONGEROREQUAL( __opA, __opB ) ( __opA <= __opB ? 1 : 0 )
#define APLEFOO_OP_BINDS_WEAKER( __opA, __opB ) ( __opA > __opB ? 1 : 0 )
#define APLEFOO_OP_BINDS_WEAKEROREQUAL( __opA, __opB ) ( __opA >= __opB ? 1 : 0 )


//The following macro is used to reset the APLEFOO current token.
#define APLEFOO_CURRENTTOKEN_RESET() { aplefoo_currentToken->op = APLEFOO_OP_VAL_NULL; aplefoo_currentToken->type = APLEFOO_TOKEN_TYPE_NULL;  aplefoo_currentToken->var = 0; }

//The following macro is used to print a token in-line.
#define APLEFOO_TOKEN_PRINT( __token ) { \
	if (__token->type == APLEFOO_TOKEN_TYPE_VAR) { \
		printf("VAR:%d,%s ", __token->var, main_varNames[__token->var]);\
	} else if (__token->type == APLEFOO_TOKEN_TYPE_OBR) { \
		printf("OBR "); \
	} else if (__token->type == APLEFOO_TOKEN_TYPE_CBR) { \
		printf("CBR "); \
	} else if (__token->type == APLEFOO_TOKEN_TYPE_UOP){ \
		printf("UOP:"); \
		if (__token->op == APLEFOO_OP_VAL_NULL){ \
			printf("NULL "); \
		} if (__token->op == APLEFOO_OP_VAL_NOT){ \
			printf("NOT "); \
		} \
	} else if (__token->type == APLEFOO_TOKEN_TYPE_BOP){ \
		printf("BOP:"); \
		if (__token->op == APLEFOO_OP_VAL_NULL){ \
			printf("NULL "); \
		} else if (__token->op == APLEFOO_OP_VAL_AND){ \
			printf("AND "); \
		} else if (__token->op == APLEFOO_OP_VAL_XOR){ \
			printf("XOR "); \
		} else if (__token->op == APLEFOO_OP_VAL_OR){ \
			printf("OR "); \
		} else if (__token->op == APLEFOO_OP_VAL_RIM){ \
			printf("RIM "); \
		} else if (__token->op == APLEFOO_OP_VAL_LIM){ \
			printf("LIM "); \
		} else if (__token->op == APLEFOO_OP_VAL_EQU){ \
			printf("EQU "); \
		} \
	} else { \
		printf("UNKNOWN "); \
	} \
}

typedef struct aplefoo_treeNode aplefoo_treeNode;

struct aplefoo_treeNode {
	aplefoo_treeNode *parent;		//Denotes the parent node in the tree.
	aplefoo_treeNode **children;	//Contains all the children for this node.
	uint32_t parentPos;				//Denotes where in the parent->children array the pointer to this node is found.
	uint32_t numChildrenUsed;		//Denotes how many children this node has.
	uint32_t numChildrenAvail;		//Denotes for how many children the memory was allocated in the first dimension of children.
	uint32_t var;					//In case this node is of type APLEFOO_TOKEN_TYPE_VAR, this states the variable number.
	aplefoo_token_type type;		//This states the node type.
	aplefoo_token_op op;			//In case this node is of type APLEFOO_TOKEN_TYPE_UOP/BOP, it states the operator.
};

aplefoo_treeNode* aplefoo_tree_root;
aplefoo_treeNode* aplefoo_tree_current;
int32_t aplefoo_numAuxVars;

void aplefoo_extren_disposeTree(aplefoo_treeNode*);	//Completely disposes a node tree recursively starting with the given node.
aplefoo_treeNode* aplefoo_extern_createNode(aplefoo_treeNode*,uint32_t,aplefoo_token_type,aplefoo_token_op,uint32_t,uint32_t);
													//Creates a new tree node and returns the pointer to it.
void aplefoo_extern_deleteNode(aplefoo_treeNode*);	//Deletes the given node.
uint32_t aplefoo_extern_alterTree();				//Alters the tree based on the current node and the current token. It returns
													//1 in case of success and 0 otherwise.
aplefoo_treeNode* aplefoo_extern_copyTree(aplefoo_treeNode*, aplefoo_treeNode*, uint32_t);//Copies the given tree into a new
													//tree and returns the pointer to the root of the new tree.
uint32_t aplefoo_extern_translateToStdBasis(aplefoo_treeNode*);	//Translates the given tree such that it contains only AND,
													//OR, and NOT. It returns 1 in case of success, and 0 otherwise.
uint32_t aplefoo_extern_pushNegations(aplefoo_treeNode*, uint32_t);	//This pushes all negations down to the variable tokens
													//beginning from the given node.

int32_t aplefoo_extern_tseitinTransformFromNNF(aplefoo_treeNode*);//This method translates the binary NNF tree into a CNF and
													//puts each clause into the startup CNF clauses array. It uses an optimized
													//Tseitin encoding for monotone circuits.
													//While at it, this method also deletes all nodes from the tree.

int32_t aplefoo_extern_tseitinTransformFromArbitrary(aplefoo_treeNode*);//This method directly derives the CNF from the given
													//tree and puts them into the startup CNF clauses array. It uses the full
													//Tseitin encoding without optimizations.
													//While at it, this method also deletes all nodes from the tree.

void aplefoo_extern_printTree(aplefoo_treeNode*, uint32_t); //Prints the tree from the given node.
void aplefoo_extern_printFormulaFromTree(aplefoo_treeNode*, uint32_t);//Prints the formula using the current tree.
#endif /* APLEFOO_H_ */
