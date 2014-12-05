/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef LQUEUE_H_
#define LQUEUE_H_

#include "tools.h"

literal** lQ;									//The literal priority queue.
uint32_t lQ_leaf;								//The literal priority queue leaf.
literal** lS;									//The literal priority queue.
uint32_t lS_leaf;								//The literal priority queue leaf.

//Methods for handling the literal priority queue.
void lQ_moveUp(literal*);						//To move an element up in the literal priority queue.
void lQ_moveDown(literal*);						//To move an element down in the literal priority queue.
literal* lQ_dequeueBest();						//To dequeue the best element in the literal priority queue.
void lQ_delete(literal*);						//To delete an element from the literal priority queue.
void lQ_touchLiteral_null(literal*);			//To disable literal touching totally.
void lQ_touchLiteral_fld(literal*);				//To touch a literal while doing double-look failed literal detection.
void lQ_touchLiteral_slfld(literal*);			//To touch a literal while doing single-look failed literal detection.
void lQ_touchLiteral_acce(literal*);			//To touch a literal while doing asymmetric covered clause elimination.
void lQ_touchLiteral_nhbr(literal*);			//To touch a literal while doing NHBR.
void lQ_touchLiteral_inp_fld(literal*); 		//To touch a literal while doing FLD in in-processing.
void lQ_touchLiteral_inp_nhbr(literal*);		//To touch a literal in NHBR mode.
void lQ_touchLiteral_inp_treelook(literal*);	//To touch a literal in TREELOOK mode.
void lQ_touchLiteral_inp_niver(literal*);		//To touch a literal in NIVER mode.
void (*lQ_touchLiteral)(literal*);				//This method points to one of the lQ_touchLiteral() methods above.
void lQ_flush();								//To remove all elements in the literal queue.

void lQ_reset();								//Reset the literal priority queue.
uint32_t lQ_init();								//For initializing once f.n_initial is known. Returns 0 if problems arise.
void lQ_dispose();								//For freeing all related memory.

//Methods for handling the literal stack.
void lS_delete(literal*);						//To delete a literal from the literal stack.
void lS_touchLiteral_null(literal*);			//To do nothing when touching a literal.
void lS_touchLiteral_ternary(literal*);			//To touch a literal while doing unhiding.
void (*lS_touchLiteral)(literal*);  			//This points to one of the touching methods above.
literal* lS_pop();								//To pop the topmost element from the stack.
void lS_push();									//To push an element on the stack.
literal* lS_readAt(uint32_t);					//To read the element in the stack found at the given position.
void lS_flush();								//To remove all elements from the literal stack.

void lS_reset();								//Reset the literal stack.
uint32_t lS_init();								//For initializing once f.n_initial is known. Returns 0 if problems arise.
void lS_dispose();								//For freeing all related memory.

#endif /* LQUEUE_H_ */
