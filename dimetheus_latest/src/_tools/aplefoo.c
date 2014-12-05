/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "aplefoo.h"

void aplefoo_extren_disposeTree(aplefoo_treeNode* theNode){
	//This method removes recursively the complete tree starting with the given tree node.

	//First of all, we return if the given node is NULL.
	if (theNode == NULL) return;

	//We now call the cleaning method for all children this node has.
	uint32_t childNum;
	for (childNum = 0; childNum < theNode->numChildrenUsed; ++childNum){
		aplefoo_extren_disposeTree(theNode->children[childNum]);
		theNode->children[childNum] = NULL;
	}
	//We can now free the children array, since all children have been cleared.
	free(theNode->children);

	//We can now free the node itself and set its pointer to NULL.
	free(theNode);
}

aplefoo_treeNode* aplefoo_extern_createNode(aplefoo_treeNode* parent, uint32_t parentPos, aplefoo_token_type type,
		aplefoo_token_op op, uint32_t var, uint32_t newChildNum){

	//The new node that is to be created.
	aplefoo_treeNode* resultingNode = NULL;
	//Allocate memory for it.
	resultingNode = malloc(sizeof(aplefoo_treeNode));
	//In case the allocation fails, we return NULL.
	if (resultingNode == NULL) return NULL;

	//Otherwise, we now initialize the new node data based on what was provided.
	resultingNode->parent = parent;
	resultingNode->parentPos = parentPos;
	resultingNode->type = type;
	resultingNode->op = op;
	resultingNode->var = var;
	resultingNode->numChildrenUsed = 0;
	resultingNode->numChildrenAvail = newChildNum;
	resultingNode->children = NULL;
	resultingNode->children = malloc(sizeof(aplefoo_treeNode*) * resultingNode->numChildrenAvail);

	return resultingNode;
}

void aplefoo_extern_deleteNode(aplefoo_treeNode* toDelete){
	//This method deletes the current node (with freeing its memory). It will make the child pointer of the parent at which
	//toDelete is found point to the last children of toDelete. It will tell this child its new parent and new position.
	aplefoo_treeNode* newParent;
	uint32_t i,j,k;

	if (toDelete->parent == NULL){
		//We delete the root node.
		newParent = NULL;
	} else {
		//The toDelete node is not the global root. We pick its parent as the new parent.
		newParent = toDelete->parent;
	}

	//Depending on the collected data, we must now act.
	if (newParent == NULL){
		//The node toDelete has no parent.
		if (toDelete->numChildrenUsed == 0){
			//The node toDelete has no children. We just free the node itself and do not have to update anything.

		} else {
			//The node toDelete has children. We must now set all parent pointers of the children to NULL.
			for (i = 0; i < toDelete->numChildrenUsed; ++i){
				toDelete->children[i]->parent = NULL;
				toDelete->children[i]->parentPos = 0;
			}
		}
	} else {
		//The node toDelete has a parent.
		if (toDelete->numChildrenUsed == 0){
			//The node toDelete has no children. We just remove toDelete from the parents children array. In order to make sure
			//that the children ordering is kept, we rewrite the children array of the parent, telling all its children their
			//new positions.
			j=0;
			for (i=0; i < newParent->numChildrenUsed; ++i){
				if (newParent->children[i] == toDelete){
					//We ignore the node we are about to delete.
					continue;
				}

				//The child at position i will be written to position j and its parent position is updated to j.
				if (i != j){
					//We have to do nothing as long as i == j. However, if they are not equal, child i becomes child j < i.
					newParent->children[j] = newParent->children[i];
					newParent->children[j]->parentPos = j;
				}

				//We increase the write pointer.
				++j;
			}
			//Afterwards, we tell the parent that it has less children now.
			--newParent->numChildrenUsed;
		} else {
			//The node toDelete has children. We create a new children array for the parent that has sufficient size to store
			//all its current children -1 (the node we delete) plus all the children of the toDelete + 1 for less memory hassle.
			newParent->numChildrenAvail = newParent->numChildrenUsed + toDelete->numChildrenUsed;
			aplefoo_treeNode** newChildArray = malloc(sizeof(aplefoo_treeNode*) * newParent->numChildrenAvail);
			//We copy all the old children of newParent into the new array but skip toDelete. In case of to delete, we write
			//to deletes children and update their new parent and parent position.
			j=0;
			for (i=0; i < newParent->numChildrenUsed; ++i){
				if (newParent->children[i] != toDelete){
					newChildArray[j] = newParent->children[i];
					newChildArray[j]->parentPos = j;
					++j;
				} else {
					for (k = 0; k < toDelete->numChildrenUsed; ++k){
						newChildArray[j] = toDelete->children[k];
						newChildArray[j]->parentPos = j;
						newChildArray[j]->parent = newParent;
						++j;
					}
				}
			}

			//We now free the old children array and link in the newly constructed array.
			free(newParent->children);
			newParent->children = newChildArray;
			newParent->numChildrenUsed = j;
		}
	}

	//Finally, we free the node itself.
	if (toDelete->children != NULL){
		free(toDelete->children);
	}
	free(toDelete);
}


uint32_t aplefoo_extern_alterTree(){
	//Alters the tree based on the current node and the current token. We assume here, that the aplefoo_tree_current pointer
	//points to the correct node on which we currently wait for a new token. We also assume, that aplefoo_currentToken
	//contains the new token that we use to alter the tree accordingly. Finally, we assume that the token is allowed at this
	//position -- the token generator function must check this before calling this method. The table of allowed tokens
	//following each other is as follows. The line denotes the previous token, the column the current token.
	//     VAR OBR CBR UOP BOP
	//VAR   X   X   o   X   o
	//OBR   o   o   X   o   X
	//CBR   X   X   o   X   o
	//UOP   o   o   X   o   X
	//BOP   o   o   X   o   X
	//The current situations given the current tree node and the current token are as follows.
	//		CBR OBR VAR NOT AND XOR OR RIM LIM EQU
	//OBR	1.1 1.2 1.3 1.4 -------- 1.5 ---------
	//VAR
	//NOT
	//AND
	//XOR
	//OR
	//RIM
	//LIM
	//EQU

	//This method returns 1 if success and 0 otherwise.

	aplefoo_treeNode *newNode = NULL, *newCurrent = NULL;

	//We need to distinct the following cases.
	if (aplefoo_tree_current->type == APLEFOO_TOKEN_TYPE_OBR){
		//Case 1.
		//The current position in the tree is an opening bracket. The following cases for the current token can occur.
		if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_CBR){
			//Case 1.1. The new token is a closing bracket. The action to perform is delete the bracket and have the current
			//pointer point to the last child of the former bracket node. Then, we move backwards until we hit the next
			//opening bracket.
			if (aplefoo_tree_current->numChildrenUsed == 0){
				printf("c ERROR. APLEFOO alter tree case 1.1. Trying to delete OBR with no children.\n");
				return 0;
			}
			newCurrent = aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed-1];
			aplefoo_extern_deleteNode(aplefoo_tree_current);
			//The new current node is the last child of the former current node.
			aplefoo_tree_current = newCurrent;
			//Move back to the next opening bracket.
			while (aplefoo_tree_current->type != APLEFOO_TOKEN_TYPE_OBR){
				if (aplefoo_tree_current->parent == NULL){
					printf("c ERROR. APLEFOO move up tree case 1.1 on node with no parent.\n");
					return 0;
				}
				aplefoo_tree_current = aplefoo_tree_current->parent;
			}
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
			//Case 2.2. The new token is another opening bracket. We add it.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_OBR,					//The type of the new node is an opening bracket.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					0,										//The variable ID is 0.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 1.2. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the opening bracket to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_VAR){
			//Case 1.3. The new token is a variable. We just add the variable and make it the new current node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_VAR,					//The type of the new node is a variable.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					aplefoo_currentToken->var,				//The variable ID is the one given in the token.
					1										//We allocate memory for 1 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 1.3. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the variable to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_UOP && aplefoo_currentToken->op == APLEFOO_OP_VAL_NOT){
			//Case 1.4. The new token is the unary operator NOT. We simply append the NOT node as a new child and make it
			//the new current node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_UOP,					//The type of the new node is a unary operator.
					APLEFOO_OP_VAL_NOT,						//The operator of the new node is NOT.
					0,										//The variable ID is 0.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 1.4. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the UOP to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_BOP){
			//Case 1.5. The new token is a binary operator. The following to major situations might occur.
			//1.5.1. The current opening bracket node has no parent. We must then change the current node to be the given
			//binary operator but leave everything else as it is.
			//1.5.2. The current opening bracket node has a parent. We must determine what type of parent we have.
			if (aplefoo_tree_current->parent == NULL){
				//The current node has no parent. We just change the node type of the current node to the operator given.
				aplefoo_tree_current->type = APLEFOO_TOKEN_TYPE_BOP;
				aplefoo_tree_current->op = aplefoo_currentToken->op;
			} else {
				//The current node has a parent. We must now determine which of the following cases occurs.
				if (aplefoo_tree_current->parent->type == APLEFOO_TOKEN_TYPE_OBR){
					//1.5.2.1. The parent node is an opening bracket. We replace the current opening bracket with the operator
					//and everything else stays as it is.
					aplefoo_tree_current->type = APLEFOO_TOKEN_TYPE_BOP;
					aplefoo_tree_current->op = aplefoo_currentToken->op;
				} else if (aplefoo_tree_current->parent->type == APLEFOO_TOKEN_TYPE_UOP
						|| aplefoo_tree_current->parent->type == APLEFOO_TOKEN_TYPE_BOP){
					//1.5.2.2. The parent node is an operator.
					if (APLEFOO_OP_BINDS_WEAKER(aplefoo_tree_current->parent->op, aplefoo_currentToken->op)){
						//The parent operator binds weaker than the token operator. We just change the current bracket node
						//into the token operator and leave everything unchanged.
						aplefoo_tree_current->type = APLEFOO_TOKEN_TYPE_BOP;
						aplefoo_tree_current->op = aplefoo_currentToken->op;
					} else {
						//The parent operator binds at least as strong as the token operator. We will now delete the current
						//bracket node. Then we will go backwards unless we either find a bracket as new parent or an operator
						//that binds strictly weaker.
						newCurrent = aplefoo_tree_current->parent;
						aplefoo_extern_deleteNode(aplefoo_tree_current);
						//The new current node is the last child of the former current node.
						aplefoo_tree_current = newCurrent;
						while (aplefoo_tree_current->parent->type != APLEFOO_TOKEN_TYPE_OBR
							&& APLEFOO_OP_BINDS_STRONGEROREQUAL(aplefoo_tree_current->op, aplefoo_currentToken->op)) {
							if (aplefoo_tree_current->parent == NULL){
								printf("c ERROR. APLEFOO hit the root in case 1.5.2.2.\n");
								return 0;
							}
							aplefoo_tree_current = aplefoo_tree_current->parent;
						}
						//We now reached a current node that has either an opening bracket as parent, or, has as weaker
						//binding than the token operator. In any case, we just prepend the operation in front of the
						//current node.
						newNode = aplefoo_extern_createNode(
								aplefoo_tree_current->parent,			//The parent is the parent of the current node.
								aplefoo_tree_current->parentPos,		//The position in the children array of the parent.
								APLEFOO_TOKEN_TYPE_BOP,					//The type of the new node is a binary operator.
								aplefoo_currentToken->op,				//The operator of the new node is that of the token.
								0,										//The variable ID is 0.
								3										//We allocate memory for 3 children within the new node.
								);
						//Check if the allocation worked.
						if (newNode == NULL){
							printf("c ERROR. APLEFOO allocate new node case 1.5.2.2. failed. Out of memory?\n");
							return 0;
						}
						//Make the parent point to the new node.
						aplefoo_tree_current->parent->children[aplefoo_tree_current->parentPos] = newNode;
						//Make the new node first child point to the current node, an tell the current node that it is in
						//position 0 of the new parent. Furthermore, tell the child that it has a new parent.
						newNode->children[0] = aplefoo_tree_current;
						++newNode->numChildrenUsed;
						newNode->children[0]->parentPos = 0;
						newNode->children[0]->parent = newNode;

						//The new node is now the current node we keep on working with.
						aplefoo_tree_current = newNode;
					}
				}
			}
		} else {
			printf("c ERROR. APLEFOO unknown sub-case for case 1 [NODE OBR].\n");
			printf("c ERROR. Current token: "); APLEFOO_TOKEN_PRINT( aplefoo_currentToken ); printf("\n");
			return 0;
		}
	} else if (aplefoo_tree_current->type == APLEFOO_TOKEN_TYPE_VAR){
		//Case 2.
		//The current position in the tree is a variable node.
		if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_CBR){
			//Case 2.1. The new token is a closing bracket. The action to perform is to move backwards until we hit the next
			//opening bracket is found.
			while (aplefoo_tree_current->type != APLEFOO_TOKEN_TYPE_OBR){
				if (aplefoo_tree_current->parent == NULL){
					printf("c ERROR. APLEFOO hit the root in case 2.1.\n");
					return 0;
				}
				aplefoo_tree_current = aplefoo_tree_current->parent;
			}
			//The new current pointer is an opening bracket node.
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_BOP){
			//Case 2.2. The new token is a binary operator. We move back until the parent is either an opening bracket or a
			//weaker operator. As soon as we found this, we insert an new BOP node before the current.
			if (aplefoo_tree_current->parent == NULL){
				printf("c ERROR. APLEFOO hit the root in case 2.2.\n");
				return 0;
			}
			while (aplefoo_tree_current->parent->type != APLEFOO_TOKEN_TYPE_OBR
				&& APLEFOO_OP_BINDS_STRONGEROREQUAL(aplefoo_tree_current->parent->op, aplefoo_currentToken->op)) {
				aplefoo_tree_current = aplefoo_tree_current->parent;
			}
			//The parent now is an operator that is either a bracket or binds strictly weaker. We insert a new BOP node in
			//between the current and its parent and make the new node the current.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current->parent,			//The parent is the parent of the current node.
					aplefoo_tree_current->parentPos,		//The position in the children array of the parent.
					APLEFOO_TOKEN_TYPE_BOP,					//The type of the new node is a binary operator.
					aplefoo_currentToken->op,				//The operator of the new node is that of the token.
					0,										//The variable ID is 0.
					3										//We allocate memory for 3 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 1.5.2.2. failed. Out of memory?\n");
				return 0;
			}
			//Make the parent point to the new node.
			aplefoo_tree_current->parent->children[aplefoo_tree_current->parentPos] = newNode;
			//Make the new node first child point to the current node, an tell the current node that it is in
			//position 0 of the new parent. Furthermore, tell the child that it has a new parent.
			newNode->children[0] = aplefoo_tree_current;
			++newNode->numChildrenUsed;
			newNode->children[0]->parentPos = 0;
			newNode->children[0]->parent = newNode;
			//The new node is now the current node we keep on working with.
			aplefoo_tree_current = newNode;
		} else {
			printf("c ERROR. APLEFOO unknown sub-case for case 2 [NODE VAR].\n");
			printf("c ERROR. Current token: "); APLEFOO_TOKEN_PRINT( aplefoo_currentToken ); printf("\n");
			return 0;
		}
	} else if (aplefoo_tree_current->type == APLEFOO_TOKEN_TYPE_UOP && aplefoo_tree_current->op == APLEFOO_OP_VAL_NOT){
		//Case 3.
		//The current position in the tree is the unary operator NOT.
		if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
			//Case 3.1. The new token is an opening bracket. We add the new child node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_OBR,					//The type of the new node is an opening bracket.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					0,										//The variable ID is 0.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 3.1. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the opening bracket to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_VAR){
			//Case 3.2. The new token is a variable. We add the new child node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_VAR,					//The type of the new node is an opening bracket.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					aplefoo_currentToken->var,				//The variable ID of the token.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 3.1. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the variable to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_UOP && aplefoo_currentToken->op == APLEFOO_OP_VAL_NOT){
			//Case 3.3. The new token is another NOT operator. We will delete the current node (cancellation of two NOT
			//operators) and revert the current node to the parent of the current node.
			newCurrent = aplefoo_tree_current->parent;
			aplefoo_extern_deleteNode(aplefoo_tree_current);
			aplefoo_tree_current = newCurrent;
		} else {
			printf("c ERROR. APLEFOO unknown sub-case for case 3 [NODE NOT].\n");
			printf("c ERROR. Current token: "); APLEFOO_TOKEN_PRINT( aplefoo_currentToken ); printf("\n");
			return 0;
		}
	} else if (aplefoo_tree_current->type == APLEFOO_TOKEN_TYPE_BOP){
		//Case 4.
		//The current position in the tree is a binary operator.
		if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_OBR){
			//Case 4.1. The new token is an opening bracket. We add the new child node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_OBR,					//The type of the new node is an opening bracket.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					0,										//The variable ID is 0.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 4.1. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the opening bracket to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_VAR){
			//Case 4.2. The new token is a variable. We add the new child node.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_VAR,					//The type of the new node is an opening bracket.
					APLEFOO_OP_VAL_NULL,					//The operator of the new node is NULL.
					aplefoo_currentToken->var,				//The variable ID of the token.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 4.1. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the variable to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else if (aplefoo_currentToken->type == APLEFOO_TOKEN_TYPE_UOP && aplefoo_currentToken->op == APLEFOO_OP_VAL_NOT){
			//Case 4.3. The new token is the NOT operator. We append it as new child.
			newNode = aplefoo_extern_createNode(
					aplefoo_tree_current,					//The parent of the new node is the current node we point at.
					aplefoo_tree_current->numChildrenUsed,	//The position in the children array of the new node.
					APLEFOO_TOKEN_TYPE_UOP,					//The type of the new node is a unary operator.
					APLEFOO_OP_VAL_NOT,						//The operator of the new node is NOT.
					0,										//The variable ID is 0.
					2										//We allocate memory for 2 children within the new node.
					);
			//Check if the allocation worked.
			if (newNode == NULL){
				printf("c ERROR. APLEFOO allocate new node case 1.4. failed. Out of memory?\n");
				return 0;
			}
			//It worked, so we now append the new node to the current one.
			aplefoo_tree_current->children[aplefoo_tree_current->numChildrenUsed++] = newNode;
			//Check if the current node needs more memory for children.
			if (aplefoo_tree_current->numChildrenUsed == aplefoo_tree_current->numChildrenAvail){
				//Yes, it does. We will however, only add one new children.
				++aplefoo_tree_current->numChildrenAvail;
				aplefoo_tree_current->children = realloc(aplefoo_tree_current->children, sizeof(aplefoo_treeNode*)
						* aplefoo_tree_current->numChildrenAvail);
			}
			//This concludes adding the UOP to the current node. The new current node will be the newly created.
			aplefoo_tree_current = newNode;
		} else {
			printf("c ERROR. APLEFOO unknown sub-case for case 2 [NODE BOP].\n");
			printf("c ERROR. Current token: "); APLEFOO_TOKEN_PRINT( aplefoo_currentToken ); printf("\n");
			return 0;
		}
	} else {
		printf("c ERROR. Unknown case.\n");
		return 0;
	}

	return 1;
}

aplefoo_treeNode* aplefoo_extern_copyTree(aplefoo_treeNode* source, aplefoo_treeNode* parent, uint32_t parentPos){
	//Copies the given tree below source into a new tree and returns the pointer to the root of the new tree. The parent
	//of the new root is the given parent at the given position. First, we allocate the new node.
	aplefoo_treeNode* result = aplefoo_extern_createNode(
			parent,						//The parent of the new node is the given parent.
			parentPos,					//The position in the parents child array is given.
			source->type,				//The type of the new node is the same as the one of the source.
			source->op,					//The operator of the new node is the same as the one of the source.
			source->var,				//The variable ID is the same as the one fo the source.
			source->numChildrenAvail	//We allocate the same amount of memory as the source has.
	);

	//In case the source node has children, we recursively copy them all.
	for (result->numChildrenUsed = 0; result->numChildrenUsed < source->numChildrenUsed; ++result->numChildrenUsed){
		result->children[result->numChildrenUsed] = aplefoo_extern_copyTree(source->children[result->numChildrenUsed], result, result->numChildrenUsed);
		//Check if this worked out.
		if (result->children[result->numChildrenUsed] == NULL){
			return NULL;
		}
	}

	//The sub-tree has been copied. It is time to return the new resulting node.
	return result;
}

uint32_t aplefoo_extern_translateToStdBasis(aplefoo_treeNode* theNode){
	//Translates the given tree such that it contains only AND, OR, and NOT. It returns 1 in case of success, and 0 otherwise.

	//First thing is to check if the current node is a terminal symbol: a variable. If so we just return with success.
	if (theNode->type == APLEFOO_TOKEN_TYPE_VAR){
		//The current node is a variable, there is nothing we can do here.
		return 1;
	}

	//Otherwise, the current node is not a terminal symbol. We recursively translate all children.
	uint32_t i;
	for (i=0; i < theNode->numChildrenUsed; ++i){
		if (!aplefoo_extern_translateToStdBasis(theNode->children[i])){
			//The translation of the Child node somehow failed. We must return the error.
			return 0;
		}
	}

	//In case no error occurred, we know that all the children of the current node have been correctly translated to use only
	//the standard basis. We must now translate the current node in case it is an operator that is not in the standard basis.
	if (theNode->type == APLEFOO_TOKEN_TYPE_BOP){
		//Yes, the current node is a binary operator that we may have to translate.
		if (theNode->op == APLEFOO_OP_VAL_EQU){
			//The node is an equivalence: a = b is translated to (!a | b) & (a | !b). The left occurrences of a and b are
			//the original copies. The right occurrences of a and b will be copies of the original ones.
			//Step 1. Modify the current node into an AND node.
			theNode->op = APLEFOO_OP_VAL_AND;
			//Step 2. Create a new OR node for the left child.
			aplefoo_treeNode* leftOrNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new OR is the current node we work at.
					0,							//The left child has position 0 in the AND node.
					APLEFOO_TOKEN_TYPE_BOP,		//The type of the new node is BOP.
					APLEFOO_OP_VAL_OR,			//The operator of the new node is OR.
					0,							//The variable ID is 0.
					3							//We allocate memory for 3 children within the new node.
			);
			//Check if the allocation worked.
			if (leftOrNode == NULL){
				printf("c ERROR. APLEFOO allocate new left OR node in translation of EQU failed. Out of memory?\n");
				return 0;
			}

			//Step 3. Create a new NOT node for the left OR.
			aplefoo_treeNode* leftNotNode = aplefoo_extern_createNode(
					leftOrNode,					//The parent of the new NOT is the left OR node.
					0,							//The NOT has position 0 in the OR node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (leftNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new left NOT node in translation of EQU failed. Out of memory?\n");
				return 0;
			}

			//We can now append the NOT node as first child of the left OR node.
			leftOrNode->children[0] = leftNotNode;
			++leftOrNode->numChildrenUsed;

			//Step 4. We can now re-link the original a and b nodes to the new OR node.
			//First, we re-link the a node below the NOT node, and tell the AND node that its new first children is the left OR.
			aplefoo_treeNode* toRelink = theNode->children[0];
			leftNotNode->children[0] = toRelink;
			++leftNotNode->numChildrenUsed;
			toRelink->parent = leftNotNode;
			toRelink->parentPos = 0;
			theNode->children[0] = leftOrNode;
			//Second, we re-link the b node below the OR node as second child.
			toRelink = theNode->children[1];
			leftOrNode->children[1] = toRelink;
			++leftOrNode->numChildrenUsed;
			toRelink->parent = leftOrNode;
			toRelink->parentPos = 1;

			//Step 5. We create the right or node and append it to the current node at position 1.
			aplefoo_treeNode* rightOrNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new OR is the current node we work at.
					1,							//The right child has position 1 in the AND node.
					APLEFOO_TOKEN_TYPE_BOP,		//The type of the new node is BOP.
					APLEFOO_OP_VAL_OR,			//The operator of the new node is OR.
					0,							//The variable ID is 0.
					3							//We allocate memory for 3 children within the new node.
			);
			//Check if the allocation worked.
			if (rightOrNode == NULL){
				printf("c ERROR. APLEFOO allocate new right OR node in translation of EQU failed. Out of memory?\n");
				return 0;
			}
			theNode->children[1] = rightOrNode;

			//Step 6. Create a new NOT node for the right OR.
			aplefoo_treeNode* rightNotNode = aplefoo_extern_createNode(
					rightOrNode,				//The parent of the new NOT is the left OR node.
					1,							//The NOT has position 1 in the OR node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (rightNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new right NOT node in translation of EQU failed. Out of memory?\n");
				return 0;
			}

			//We can now append the NOT node as second child of the right OR node. We skip the first position as this will
			//later contain the copy of the a node.
			rightOrNode->numChildrenUsed = 2;
			rightOrNode->children[1] = rightNotNode;

			//Step 7. We must now copy the a node tree into the first child of the right OR node. The tree we must copy is
			//found below the left NOT node.
			rightOrNode->children[0] = aplefoo_extern_copyTree(leftNotNode->children[0], rightOrNode, 0);
			//Check if the allocation worked.
			if (rightOrNode->children[0] == NULL){
				printf("c ERROR. APLEFOO copy A node in translation of EQU failed. Out of memory?\n");
				return 0;
			}

			//Step 8. We must now copy the b node tree into the first child of the right NOT node. The tree we must copy is
			//found below the left OR node, second child. The right NOT node has therefore one child.
			rightNotNode->numChildrenUsed = 1;
			rightNotNode->children[0] = aplefoo_extern_copyTree(leftOrNode->children[1], rightNotNode, 0);
			if (rightNotNode->children[0] == NULL){
				printf("c ERROR. APLEFOO copy B node in translation of EQU failed. Out of memory?\n");
				return 0;
			}
			//This concludes the translation of the EQU node.
		} else if (theNode->op == APLEFOO_OP_VAL_RIM){
			//The node is a right implication: a > b is translated to !a | b. This is a lucky case in which we just have to
			//change the RIM node into an OR node and prepend a NOT before its left child.
			//Step 1. Change the node type to OR.
			theNode->op = APLEFOO_OP_VAL_OR;
			//Step 2. Create the left NOT node.
			aplefoo_treeNode* leftNotNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new NOT is the node we translate.
					0,							//The NOT has position 0 in the parent node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (leftNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new left NOT node in translation of RIM failed. Out of memory?\n");
				return 0;
			}
			//Step 3. We now re-link the first child of the current node below the left NOT node. The not node will be the new
			//parent and the position will be 0. Afterwards, we override the left child of the current node to be the NOT node.
			aplefoo_treeNode* toRelink = theNode->children[0];
			leftNotNode->children[0] = toRelink;
			leftNotNode->numChildrenUsed = 1;
			toRelink->parent = leftNotNode;
			toRelink->parentPos = 0;
			theNode->children[0] = leftNotNode;
			//The right child of the current node stays unchanged for the right implication.
		} else if (theNode->op == APLEFOO_OP_VAL_LIM){
			//The node is a left implication: a < b is translated to a | !b. This is a lucky case in which we just have to
			//change the LIM node into an OR node and prepend a NOT before its right child.
			//Step 1. Change the node type to OR.
			theNode->op = APLEFOO_OP_VAL_OR;
			//Step 2. Create the right NOT node.
			aplefoo_treeNode* rightNotNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new NOT is the node we translate.
					1,							//The NOT has position 1 in the parent node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (rightNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new left NOT node in translation of LIM failed. Out of memory?\n");
				return 0;
			}
			//Step 3. We now re-link the second child of the current node below the right NOT node. The not node will be the
			//new parent and the position will be 0. Afterwards, we override the right child of the current node to be the NOT.
			aplefoo_treeNode* toRelink = theNode->children[1];
			rightNotNode->children[0] = toRelink;
			rightNotNode->numChildrenUsed = 1;
			toRelink->parent = rightNotNode;
			toRelink->parentPos = 0;
			theNode->children[1] = rightNotNode;
			//The left child of the current node stays unchanged for the left implication.
		} else if (theNode->op == APLEFOO_OP_VAL_XOR){
			//The node is an exclusive OR: a ^ b is translated to (a | b) & (!a | !b). The left occurrences of a and b are
			//the original copies. The right occurrences of a and b will be copies of the original ones.
			//Step 1. Change the current node to AND.
			theNode->op = APLEFOO_OP_VAL_AND;
			//Step 2. Create the left OR node.
			aplefoo_treeNode* leftOrNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new OR is the current node we work at.
					0,							//The left child has position 0 in the AND node.
					APLEFOO_TOKEN_TYPE_BOP,		//The type of the new node is BOP.
					APLEFOO_OP_VAL_OR,			//The operator of the new node is OR.
					0,							//The variable ID is 0.
					3							//We allocate memory for 3 children within the new node.
			);
			//Check if the allocation worked.
			if (leftOrNode == NULL){
				printf("c ERROR. APLEFOO allocate new left OR node in translation of XOR failed. Out of memory?\n");
				return 0;
			}
			//Step 3. Re-link the a and b node to be the first and second child of the new OR node. Make the current node the
			//parent of the left OR node and the position will be 0.
			aplefoo_treeNode* toRelink = theNode->children[0];
			leftOrNode->children[0] = toRelink;
			toRelink->parent = leftOrNode;
			toRelink->parentPos = 0;
			toRelink = theNode->children[1];
			leftOrNode->children[1] = toRelink;
			toRelink->parent = leftOrNode;
			toRelink->parentPos = 1;
			leftOrNode->numChildrenUsed = 2;
			theNode->children[0] = leftOrNode;

			//Step 4. Create the right OR node and make it the child of the current node.
			aplefoo_treeNode* rightOrNode = aplefoo_extern_createNode(
					theNode,					//The parent of the new OR is the current node we work at.
					1,							//The left child has position 1 in the AND node.
					APLEFOO_TOKEN_TYPE_BOP,		//The type of the new node is BOP.
					APLEFOO_OP_VAL_OR,			//The operator of the new node is OR.
					0,							//The variable ID is 0.
					3							//We allocate memory for 3 children within the new node.
			);
			//Check if the allocation worked.
			if (rightOrNode == NULL){
				printf("c ERROR. APLEFOO allocate new right OR node in translation of XOR failed. Out of memory?\n");
				return 0;
			}
			theNode->children[1] = rightOrNode;

			//Step 5. Create the left and right NOT nodes as children of the right OR node.
			aplefoo_treeNode* leftNotNode = aplefoo_extern_createNode(
					rightOrNode,				//The parent of the new NOT is the right OR node.
					0,							//The left child has position 0 in the OR node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (leftNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new left NOT node in translation of XOR failed. Out of memory?\n");
				return 0;
			}
			//Append the new child to the OR node.
			rightOrNode->children[0] = leftNotNode;
			++rightOrNode->numChildrenUsed;

			aplefoo_treeNode* rightNotNode = aplefoo_extern_createNode(
					rightOrNode,				//The parent of the new NOT is the right OR node.
					1,							//The left right has position 1 in the OR node.
					APLEFOO_TOKEN_TYPE_UOP,		//The type of the new node is UOP.
					APLEFOO_OP_VAL_NOT,			//The operator of the new node is NOT.
					0,							//The variable ID is 0.
					2							//We allocate memory for 2 children within the new node.
			);
			//Check if the allocation worked.
			if (rightNotNode == NULL){
				printf("c ERROR. APLEFOO allocate new left OR node in translation of EQU failed. Out of memory?\n");
				return 0;
			}
			//Append the new child to the OR node.
			rightOrNode->children[1] = rightNotNode;
			++rightOrNode->numChildrenUsed;

			//Step 6. We must now copy the original a and b nodes below the new NOT nodes. The original a node is the first
			//child of the left OR node, and the original b node is the second child of the left OR node.
			leftNotNode->children[0] = aplefoo_extern_copyTree(leftOrNode->children[0], leftNotNode, 0);
			//Check if the allocation worked.
			if (leftNotNode->children[0] == NULL){
				printf("c ERROR. APLEFOO copy A node in translation of XOR failed. Out of memory?\n");
				return 0;
			}
			leftNotNode->numChildrenUsed = 1;
			rightNotNode->children[0] = aplefoo_extern_copyTree(leftOrNode->children[1], rightNotNode, 0);
			//Check if the allocation worked.
			if (rightNotNode->children[0] == NULL){
				printf("c ERROR. APLEFOO copy B node in translation of XOR failed. Out of memory?\n");
				return 0;
			}
			rightNotNode->numChildrenUsed = 1;
			//This concludes the translation of the XOR node.
		}
		//In case we reach this position, we either translated the node or no translation was necessary.
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_UOP){
		//The current node is a unary operator.
		if (theNode->op == APLEFOO_OP_VAL_NOT){
			//The current unary operator is the NOT. We do not have to do anything.
		} else {
			//In case we reach this position, we encountered an unknown unary operator and must return with an error.
			printf("c ERROR. APLEFOO translate to standard basis failed -- unknown unary operator.\n");
			return 0;
		}
	} else {
		//In case we reach this position, we encountered an unknown node type and must return with an error.
		printf("c ERROR. APLEFOO translate to standard basis failed -- unknown node type.\n");
		return 0;
	}

	//Finally, we can return the success.
	return 1;
}

uint32_t aplefoo_extern_pushNegations(aplefoo_treeNode* theNode, uint32_t isNeg){
	//This pushes all negations down to the variable tokens beginning from the given node theNode. The integer isNeg tells
	//us whether we are currently working in the "negation" mode, i.e. if there was a negation before that. This method
	//returns 1 in case of success and 0 otherwise. This method assumes that the tree is binary using the standard basis only.

	//We check the following cases.
	//0. The current node is a variable.
	//	0.1. The isNeg is true.
	//		Invert the sign of the variable and return with success.
	//	0.2. The isNeg is false.
	//		Leave the variable as it is and return with success.
	//1. The current operator is an AND.
	//	1.1. The isNeg is true.
	//		Alter the current node to OR and recursively push negations on all its children, isNeg stays true.
	//	1.2. The isNeg is false.
	//		Leave the node as it is and recursively push negations on all its children, isNeg stays false.
	//2. The current operator is an OR.
	//	2.1. The isNeg is true.
	//		Alter the current node to AND and recursively push negations on all its children, isNeg stays true.
	//	2.2. The isNeg is false.
	//		Leave the node as it is and recursively push negations on all its children, isNeg stays false.
	//3. The current operator is NOT.
	//	3.1. The isNeg is true.
	//		Delete the current node and re-link parent and child. Recursively call on the child, toggle isNeg to false.
	//	3.2. The isNeg is false.
	//		Delete the current node and re-link parent and child. Recursively call on the child, toggle isNeg to true.

	if (theNode->type == APLEFOO_TOKEN_TYPE_VAR){
		//Case 0.
		if (isNeg){
			theNode->var = (-theNode->var);//The literal in the NNF will be negative.
		}
		return 1;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_BOP && theNode->op == APLEFOO_OP_VAL_AND){
		//Case 1.
		if (isNeg){
			//Case 1.1.
			//Change the operator from AND to OR.
			theNode->op = APLEFOO_OP_VAL_OR;
		}//else it is Case 1.2.
		//Make sure that the operator has exactly two children and push the negation recursively.
		if (theNode->numChildrenUsed != 2){
			printf("c ERROR. APLEFOO push down negations found an AND with not exactly two children.\n");
			return 0;
		}
		//Push down on the children.
		if (!aplefoo_extern_pushNegations(theNode->children[0], isNeg)) return 0;
		if (!aplefoo_extern_pushNegations(theNode->children[1], isNeg)) return 0;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_BOP && theNode->op == APLEFOO_OP_VAL_OR){
		//Case 2.
		if (isNeg){
			//Case 2.1.
			//Change the operator from OR to AND.
			theNode->op = APLEFOO_OP_VAL_AND;
		}//else it is Case 2.2.
		//Make sure that the operator has exactly two children and push the negation recursively.
		if (theNode->numChildrenUsed != 2){
			printf("c ERROR. APLEFOO push down negations found an OR with not exactly two children.\n");
			return 0;
		}
		//Push down on the children.
		if (!aplefoo_extern_pushNegations(theNode->children[0], isNeg)) return 0;
		if (!aplefoo_extern_pushNegations(theNode->children[1], isNeg)) return 0;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_UOP && theNode->op == APLEFOO_OP_VAL_NOT){
		//Case 3.
		//First, we make sure that the current NOT operation has exactly one child. If not, there is something wrong.
		if (theNode->numChildrenUsed != 1){
			printf("c ERROR. APLEFOO push down negations found a negation with not exactly one child.\n");
			return 0;
		}
		aplefoo_treeNode *child = theNode->children[0];
		//Second, we check whether the current NOT is actually the root node. If so, we re-link the root and current to the
		//first child that will be the new root.
		if (theNode->parent == NULL){
			//The current node has no parent. Check that this node is actually the root. Otherwise we have a situation that
			//should never occur and we return an error.
			if (theNode != aplefoo_tree_root){
				printf("c ERROR. APLEFOO push down negations found a node without a parent that is not the root.\n");
				return 0;
			}
			//The current node is the root node. The new root (and current) will be the single child.
			aplefoo_tree_current = child;
			aplefoo_tree_root = child;
		} else {
			//The current node has a parent. We make the parent in the position of the current node point to the first child.
			//Thereby we effectively skip the current node that we are about to delete.
			theNode->parent->children[theNode->parentPos] = child;
		}
		//We now tell the child that it has a new parent and a new parent position (this might be NULL in case it became the
		//new root node, but it will work anyway).
		child->parent = theNode->parent;
		child->parentPos = theNode->parentPos;
		//We can now delete the current node.
		free(theNode->children);
		free(theNode);
		//Recursively push down the negation, but TOGGLE it before the call as we just removed a NOT operation.
		isNeg = 1 - isNeg;
		if (!aplefoo_extern_pushNegations(child, isNeg)) return 0;
	} else {
		//This case should not happen in case we are working on a binary tree with the standard basis.
		printf("c ERROR. APLEFOO push down negations encountered an unknown case.\n");
		return 0;
	}

	return 1;
}

int32_t aplefoo_addNewAuxiliaryVariable(){
	//This method adds a new auxiliary variable and returns its ID. The new auxiliary variable is appended to the set of
	//variables we already have.
	++f.n_initial;
	++f.n_afterPrep;
	//Allocate memory to store the prefix and the ID integer.
	main_varNames[f.n_initial] = malloc(sizeof(char)*(31));
	if (main_varNames[f.n_initial] == NULL){
		printf("c ERROR. APLEFOO was unable to allocate memory for a new auxiliary variable name. Out of memory?\n");
		return 0;
	}
	//Increase the counter for auxiliary variables.
	++aplefoo_numAuxVars;
	//Write the integer into the buffer. The buffer is large enough to hold the 19 characters auxiliary variable prefix as well
	//as an integer consisting of 11 digits as well as the NULL byte.
	sprintf(main_varNames[f.n_initial], "__dimetheus_auxvar_%d", aplefoo_numAuxVars);

	//We must now increase the names array in the first dimension to allow us to store more stuff. Allocate memory using
	//f.n_initial + 2. The +2 makes sure, that we have main_varNames[0] and main_varNames[f.n_initial+1], the latter being the
	//next memory position for a variable.
	main_varNames = realloc(main_varNames, sizeof(char*) * (f.n_initial+2));
	main_varNames[f.n_initial+1] = NULL;
	//Return the ID of the new auxiliary variable.
	return f.n_initial;
}

int32_t aplefoo_createUnitClause(int32_t lit1){
	//This method creates a unit clause and appends it to the startup_cnfClauses array. The method returns 1 in case of
	//success and 0 otherwise.

	//First we allocate memory for the new binary clause. We need 2 memory positions (do not forget the terminating zero).
	startup_cnfClauses[startup_cnfClausesNumUsed] = NULL;
	startup_cnfClauses[startup_cnfClausesNumUsed] = malloc(sizeof(int32_t) * 2);
	if (startup_cnfClauses[startup_cnfClausesNumUsed] == NULL){
		printf("c ERROR. APLEFOO was unable to add a new CNF clause into the startup array. Out of memory?\n");
		return 0;
	}
	startup_cnfClauses[startup_cnfClausesNumUsed][0] = lit1;
	startup_cnfClauses[startup_cnfClausesNumUsed][1] = 0;

	//Increase the number of used clauses.
	++startup_cnfClausesNumUsed;
	//Increase the amount of memory for CNF clauses, if necessary.
	if (startup_cnfClausesNumUsed == startup_cnfClausesNumAvail){
		startup_cnfClausesNumAvail += BLOCKSIZE;
		startup_cnfClauses = realloc(startup_cnfClauses, sizeof(int32_t*) * startup_cnfClausesNumAvail);
	}

	return 1;
}

int32_t aplefoo_createBinaryClause(int32_t lit1, int32_t lit2){
	//This method creates a binary clause and appends it to the startup_cnfClauses array. The method returns 1 in case of
	//success and 0 otherwise.

	//First we allocate memory for the new binary clause. We need 3 memory positions (do not forget the terminating zero).
	startup_cnfClauses[startup_cnfClausesNumUsed] = NULL;
	startup_cnfClauses[startup_cnfClausesNumUsed] = malloc(sizeof(int32_t) * 3);
	if (startup_cnfClauses[startup_cnfClausesNumUsed] == NULL){
		printf("c ERROR. APLEFOO was unable to add a new CNF clause into the startup array. Out of memory?\n");
		return 0;
	}
	startup_cnfClauses[startup_cnfClausesNumUsed][0] = lit1;
	startup_cnfClauses[startup_cnfClausesNumUsed][1] = lit2;
	startup_cnfClauses[startup_cnfClausesNumUsed][2] = 0;

	//Increase the number of used clauses.
	++startup_cnfClausesNumUsed;
	//Increase the amount of memory for CNF clauses, if necessary.
	if (startup_cnfClausesNumUsed == startup_cnfClausesNumAvail){
		startup_cnfClausesNumAvail += BLOCKSIZE;
		startup_cnfClauses = realloc(startup_cnfClauses, sizeof(int32_t*) * startup_cnfClausesNumAvail);
	}

	return 1;
}

int32_t aplefoo_createTernaryClause(int32_t lit1, int32_t lit2, int32_t lit3){
	//This method creates a ternary clause and appends it to the startup_cnfClauses array. The method returns 1 in case of
	//success and 0 otherwise.

	//First we allocate memory for the new binary clause. We need 4 memory positions (do not forget the terminating zero).
	startup_cnfClauses[startup_cnfClausesNumUsed] = NULL;
	startup_cnfClauses[startup_cnfClausesNumUsed] = malloc(sizeof(int32_t) * 4);
	if (startup_cnfClauses[startup_cnfClausesNumUsed] == NULL){
		printf("c ERROR. APLEFOO was unable to add a new CNF clause into the startup array. Out of memory?\n");
		return 0;
	}
	startup_cnfClauses[startup_cnfClausesNumUsed][0] = lit1;
	startup_cnfClauses[startup_cnfClausesNumUsed][1] = lit2;
	startup_cnfClauses[startup_cnfClausesNumUsed][2] = lit3;
	startup_cnfClauses[startup_cnfClausesNumUsed][3] = 0;

	//Increase the number of used clauses.
	++startup_cnfClausesNumUsed;
	//Increase the amount of memory for CNF clauses, if necessary.
	if (startup_cnfClausesNumUsed == startup_cnfClausesNumAvail){
		startup_cnfClausesNumAvail += BLOCKSIZE;
		startup_cnfClauses = realloc(startup_cnfClauses, sizeof(int32_t*) * startup_cnfClausesNumAvail);
	}

	return 1;
}

int32_t aplefoo_extern_tseitinTransformFromNNF(aplefoo_treeNode* theNode){
	//This method translates the operator tree into a CNF and puts each clause into the startup CNF clauses array. While
	//at it, this method also deletes all nodes from the tree. This method assumes the tree to be binary, in standard basis,
	//and most important to reflect a formula in NNF. The transformation is based on an optimized Tseitin's encoding for
	//monotone circuits.
	int32_t theLit, leftLit, rightLit;

	if (theNode->type == APLEFOO_TOKEN_TYPE_VAR){
		//Case 1. The current node is a literal node. We just return the literal.
		theLit = theNode->var;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_BOP){
		//Case 2. The current node is a binary operator. We first recurse on its children to get the left and right literal.
		leftLit = aplefoo_extern_tseitinTransformFromNNF(theNode->children[0]); if (leftLit == 0) return 0;
		rightLit = aplefoo_extern_tseitinTransformFromNNF(theNode->children[1]); if (rightLit == 0) return 0;

		//We need a new auxiliary variable for the current gate.
		theLit = aplefoo_addNewAuxiliaryVariable(); if (theLit == 0) return 0;

		//We now check what operator we are currently working on.
		if (theNode->op == APLEFOO_OP_VAL_AND){
			//The AND gate is represented as follows in the Tseitin transformation (the two binaries are added).
			//theLit -> (leftLit & rightLit) == (-theLit | leftLit) & (-theLit | rightLit)
			if (!aplefoo_createBinaryClause(-theLit, leftLit)) return 0;
			if (!aplefoo_createBinaryClause(-theLit, rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_OR){
			//The OR gate is represented as follows in the Tseitin transformation (the ternary is added).
			//theLit -> (leftLit | rightLit) == (-theLit | leftLit | rightLit)
			if (!aplefoo_createTernaryClause(-theLit, leftLit, rightLit)) return 0;
		}
	} else {
		printf("c ERROR. APLEFOO Tseitin transformation encountered an unknown token in the tree.\n");
		return 0;
	}

	//If the current node is the root node, we will also append the global unit clause for it.
	if (theNode == aplefoo_tree_root){
		if (!aplefoo_createUnitClause(theLit)) return 0;
		//We can also reset the root and current pointer, because the tree does not exist anymore at this position.
		aplefoo_tree_root = NULL;
		aplefoo_tree_current = NULL;
	}

	//We finally delete the current node because we do not need it anymore.
	if (theNode->children != NULL){
		free(theNode->children);
	}
	free(theNode);

	return theLit;
}

int32_t aplefoo_extern_tseitinTransformFromArbitrary(aplefoo_treeNode* theNode){
	//This method translates an arbitrary operator tree into a CNF and puts each clause into the startup CNF clauses array.
	//While at it, this method also deletes all nodes from the tree. This method assumes the tree to be binary, but not
	//necessarily in standard basis. The transformation is based on the full Tseitin encoding for arbitrary circuits.
	int32_t theLit, leftLit, rightLit;

	if (theNode->type == APLEFOO_TOKEN_TYPE_VAR){
		//Case 1. The current node is a literal node. We just return the literal.
		theLit = theNode->var;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_UOP && theNode->op == APLEFOO_OP_VAL_NOT){
		//Case 2. The current node is a NOT operator. We recurse on the child and then invert the answer.
		leftLit = aplefoo_extern_tseitinTransformFromArbitrary(theNode->children[0]); if (leftLit == 0) return 0;

		//We do not need any auxiliary variable here, we just invert the sign of leftLit.
		theLit = -leftLit;
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_BOP){
		//Case 3. The current node is a binary operator. We recurse on the children to get the left and right literal.
		leftLit = aplefoo_extern_tseitinTransformFromArbitrary(theNode->children[0]); if (leftLit == 0) return 0;
		rightLit = aplefoo_extern_tseitinTransformFromArbitrary(theNode->children[1]); if (leftLit == 0) return 0;

		//We need a new auxiliary variable for the current gate.
		theLit = aplefoo_addNewAuxiliaryVariable(); if (theLit == 0) return 0;

		//We now check what operator we are currently working on.
		if (theNode->op == APLEFOO_OP_VAL_AND){
			//The AND gate is represented as follows in the full Tseitin transformation (the three clauses are added).
			//theLit <-> (leftLit & rightLit)
			// == (-theLit | leftLit)
			//    & (-theLit | rightLit)
			//    & (theLit | -leftLit | -rightLit)
			if (!aplefoo_createBinaryClause(-theLit, leftLit)) return 0;
			if (!aplefoo_createBinaryClause(-theLit, rightLit)) return 0;
			if (!aplefoo_createTernaryClause(theLit, -leftLit, -rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_XOR){
			//The XOR gate is represented as follows in the full Tseitin transformation (the clauses are added).
			//theLit <-> (leftLit ^ rightLit)
			// == (-theLit | leftLit | rightLit)
			//    & (-theLit | -leftLit | -rightLit)
			//    & (theLit | -leftLit | rightLit)
			//    & (theLit | leftLit | -rightLit)
			if (!aplefoo_createTernaryClause(-theLit, leftLit, rightLit)) return 0;
			if (!aplefoo_createTernaryClause(-theLit, -leftLit, -rightLit)) return 0;
			if (!aplefoo_createTernaryClause(theLit, -leftLit, rightLit)) return 0;
			if (!aplefoo_createTernaryClause(theLit, leftLit, -rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_OR){
			//The OR gate is represented as follows in the full Tseitin transformation (the clauses are added).
			//theLit <-> (leftLit | rightLit)
			// == (-theLit | leftLit | rightLit)
			//    & (theLit | -leftLit)
			//    & (theLit | -rightLit)
			if (!aplefoo_createTernaryClause(-theLit, leftLit, rightLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, -leftLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, -rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_RIM){
			//The RIM gate is represented as follows in the full Tseitin transformation (the clauses are added).
			//theLit <-> (leftLit > rightLit)
			// == (-theLit | -leftLit | rightLit)
			//    & (theLit | leftLit)
			//    & (theLit | -rightLit)
			if (!aplefoo_createTernaryClause(-theLit, -leftLit, rightLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, leftLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, -rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_LIM){
			//The LIM gate is represented as follows in the full Tseitin transformation (the two binaries are added).
			//theLit <-> (leftLit < rightLit)
			// == (-theLit | leftLit | -rightLit)
			//    & (theLit | -leftLit)
			//    & (theLit | rightLit)
			if (!aplefoo_createTernaryClause(-theLit, leftLit, -rightLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, -leftLit)) return 0;
			if (!aplefoo_createBinaryClause(theLit, rightLit)) return 0;
		} else if (theNode->op == APLEFOO_OP_VAL_EQU){
			//The EQU gate is represented as follows in the full Tseitin transformation (the two binaries are added).
			//theLit <-> (leftLit = rightLit)
			// == (-theLit | -leftLit | rightLit)
			//    & (-theLit | leftLit | -rightLit)
			//    & (theLit | leftLit | rightLit)
			//    & (theLit | -leftLit | -rightLit)
			if (!aplefoo_createTernaryClause(-theLit, -leftLit, rightLit)) return 0;
			if (!aplefoo_createTernaryClause(-theLit, leftLit, -rightLit)) return 0;
			if (!aplefoo_createTernaryClause(theLit, leftLit, rightLit)) return 0;
			if (!aplefoo_createTernaryClause(theLit, -leftLit, -rightLit)) return 0;
		}
	} else {
		printf("c ERROR. APLEFOO Tseitin transformation encountered an unknown token in the tree.\n");
		return 0;
	}

	//If the current node is the root node, we will also append the global unit clause for it.
	if (theNode == aplefoo_tree_root){
		if (!aplefoo_createUnitClause(theLit)) return 0;
		//We can also reset the root and current pointer, because the tree does not exist anymore at this position.
		aplefoo_tree_root = NULL;
		aplefoo_tree_current = NULL;
	}

	//We finally delete the current node because we do not need it anymore.
	if (theNode->children != NULL){
		free(theNode->children);
	}
	free(theNode);

	return theLit;
}

void aplefoo_extern_printTree(aplefoo_treeNode* node, uint32_t indent){
	printf("c         APLEFOO:     TREE:");
	uint32_t i;
	for (i = 0; i < indent; ++i){
		printf("  ");
	}
	//We output some basic data regarding this node.
	printf("%p ", (void*)node);
	if (node != NULL){
		printf("[");
		if (node == aplefoo_tree_root){
			printf("R");
		}
		if (node == aplefoo_tree_current){
			printf("C");
		}
		printf("]");
		printf("Children %d %d, Parent %p %d ",
				node->numChildrenUsed, node->numChildrenAvail, (void*) node->parent, node->parentPos);
		//Based on the type of the node, we output some information. The types we have are as follows.
		//APLEFOO_TOKEN_TYPE_NULL,	//If the token has no type.
		//APLEFOO_TOKEN_TYPE_VAR,	//If the token is a variable.
		//APLEFOO_TOKEN_TYPE_OBR, 	//If the token is an opening bracket.
		//APLEFOO_TOKEN_TYPE_CBR, 	//If the token is a closing bracket.
		//APLEFOO_TOKEN_TYPE_UOP,	//If the token is an unary operator symbol.
		//APLEFOO_TOKEN_TYPE_BOP	//If the token is a binary operator symbol.
		if (node->type == APLEFOO_TOKEN_TYPE_NULL){
			printf("NULL ");
		} else if (node->type == APLEFOO_TOKEN_TYPE_VAR){
			printf("VAR %d ", node->var);
		} else if (node->type == APLEFOO_TOKEN_TYPE_OBR){
			printf("OBR ");
		} else if (node->type == APLEFOO_TOKEN_TYPE_CBR){
			printf("CBR ");
		} else if (node->type == APLEFOO_TOKEN_TYPE_UOP){
			printf("UOP ");
			if (node->op == APLEFOO_OP_VAL_NOT){
				printf("%c ", aplefoo_op_sym_not);
			} else {
				printf("UNKNOWN ");
			}
		} else if (node->type == APLEFOO_TOKEN_TYPE_BOP){
			printf("BOP ");
			if (node->op == APLEFOO_OP_VAL_AND){
				printf("%c ", aplefoo_op_sym_and);
			} else if (node->op == APLEFOO_OP_VAL_XOR){
				printf("%c ", aplefoo_op_sym_xor);
			} else if (node->op == APLEFOO_OP_VAL_OR){
				printf("%c ", aplefoo_op_sym_or);
			} else if (node->op == APLEFOO_OP_VAL_RIM){
				printf("%c ", aplefoo_op_sym_rim);
			} else if (node->op == APLEFOO_OP_VAL_LIM){
				printf("%c ", aplefoo_op_sym_lim);
			} else if (node->op == APLEFOO_OP_VAL_EQU){
				printf("%c ", aplefoo_op_sym_equ);
			} else {
				printf("UNKNOWN ");
			}
		} else {
			printf("UNKNOWN");
		}
	} else {
		printf("UNINITIALIZED");
	}
	printf("\n");
	for (i=0; i < node->numChildrenUsed; ++i){
		aplefoo_extern_printTree(node->children[i] ,indent+1);
	}
}

void aplefoo_extern_printFormulaFromTree(aplefoo_treeNode* theNode, uint32_t indent){
	//Prints the formula starting from the given node.
	printf("c         APLEFOO:     FORMULA:");
	uint32_t i, ch;
	for (i = 0; i < indent; ++i){
		printf("  ");
	}
	if (theNode->type == APLEFOO_TOKEN_TYPE_VAR){
		printf("%d", theNode->var);
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_UOP){
		if (theNode->op == APLEFOO_OP_VAL_NOT){
			printf("%c (\n", aplefoo_op_sym_not);
		}
		for (ch = 0; ch < theNode->numChildrenUsed; ++ch){
			aplefoo_extern_printFormulaFromTree(theNode->children[ch], indent+1);
		}
		printf("c         APLEFOO:     FORMULA:");
		for (i = 0; i < indent; ++i){
			printf("  ");
		}
		printf(")");
	} else if (theNode->type == APLEFOO_TOKEN_TYPE_BOP){
		for (ch=0; ch<theNode->numChildrenUsed; ++ch){
			printf("(\n");
			aplefoo_extern_printFormulaFromTree(theNode->children[ch], indent+1);
			printf("c         APLEFOO:     FORMULA:");
			for (i = 0; i < indent; ++i){
				printf("  ");
			}
			printf(") ");
			if (ch < theNode->numChildrenUsed-1){
				if (theNode->op == APLEFOO_OP_VAL_AND){
					printf("%c ", aplefoo_op_sym_and);
				} else if (theNode->op == APLEFOO_OP_VAL_XOR){
					printf("%c ", aplefoo_op_sym_xor);
				} else if (theNode->op == APLEFOO_OP_VAL_OR){
					printf("%c ", aplefoo_op_sym_or);
				} else if (theNode->op == APLEFOO_OP_VAL_RIM){
					printf("%c ", aplefoo_op_sym_rim);
				} else if (theNode->op == APLEFOO_OP_VAL_LIM){
					printf("%c ", aplefoo_op_sym_lim);
				} else if (theNode->op == APLEFOO_OP_VAL_EQU){
					printf("%c ", aplefoo_op_sym_equ);
				} else {
					printf("?");
				}
			}
		}
	}

	printf("\n");
}
