/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef VQUEUE_H_
#define VQUEUE_H_

#include "tools.h"

variable** vQ;									//The variable priority queue.
uint32_t vQ_leaf;								//The variable priority queue leaf.
variable** vS;									//The variable stack.
uint32_t vS_leaf;								//The variable stack last position.

//Methods for handling the variable priority queue.
void vQ_moveUp(variable*);						//To move an element up in the variable priority queue.
void vQ_moveDown(variable*);					//To move an element down in the variable priority queue.
void vQ_replaceRoot(variable*);					//To replace the root of the heap with the given variable, this might sink down.
variable* vQ_dequeueBest();						//To dequeue the best element in the variable priority queue.
void vQ_delete(variable*);						//To delete an element from the variable priority queue.
void vQ_enqueue(variable*);						//To enqueue an element in the variable priority queue.
void vQ_touchVariable_null(variable*);			//To disable variable touching completely.
void vQ_touchVariable_niver(variable*);			//To touch variables if the queue is used for NIVER.
void vQ_touchVariable_ge(variable*);			//To touch variables if the queue is used for GE.
void vQ_touchVariable_inp_ge(variable*);		//To touch variables if the queue is used for GE in in-processing.
void (*vQ_touchVariable)(variable*);			//This method points to any of the vQ_touchVariable() methods above.
void vQ_flush();								//To remove all elements in the variable queue.

void vQ_reset();								//Reset the variable priority queue.
uint32_t vQ_init();								//For initializing once f.n_initial is known. Return 0 if problems arise.
void vQ_dispose();								//For freeing all related memory.

//Methods for handling the variable stack.
void vS_delete(variable*);						//To delete a variable from the variable stack.
void vS_touchVariable_null(variable*);			//To do nothing when touching a variable.
void vS_touchVariable_pl(variable*);			//To touch a variable for PLE in the variable stack.
void vS_touchVariable_inp_pl(variable*);		//To touch a variable when doing PLE in in-processing.
void vS_touchVariable_inp_ternary(variable*);	//To touch a variable when doing TERNARY resolution in in-processing.
void (*vS_touchVariable)(variable*);			//This points to one of the touching methods above.
variable* vS_pop();								//To pop the topmost element from the stack.
void vS_push(variable*);						//To just push a variable to the stack if it is not already in there.
void vS_flush();								//To remove all elements from the variable stack.

void vS_reset();								//Reset the variable stack.
uint32_t vS_init();								//For initializing once f.n_initial is known. Returns 0 if problems arise.
void vS_dispose();								//For freeing all related memory.

#endif /* VQUEUE_H_ */
