/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "tools.h"

clause** cQ;						//The clause priority queue.
uint32_t cQ_leaf;					//The current last position in this queue.
uint32_t cQ_avail;					//The amount of memory available for the cQ.
clause** cS;						//The clause stack.
uint32_t cS_leaf;					//The current last position in the stack.
uint32_t cS_avail;					//The amount of memory available for the cS.

clause** cSS0;						//The clause stack.
uint32_t cSS0_leaf;					//The current last position in the stack.
uint32_t cSS0_avail;				//The amount of memory available for the cS.

clause** cSS1;						//The clause stack.
uint32_t cSS1_leaf;					//The current last position in the stack.
uint32_t cSS1_avail;				//The amount of memory available for the cS.

//Methods for handling the clause priority queue.
void cQ_moveUp(clause*);			//To move an element up in the clause priority queue.
void cQ_moveDown(clause*);			//To move an element down in the clause priority queue.
clause* cQ_dequeueBest();			//To dequeue the best element in the clause priority queue.
void cQ_delete(clause*);			//To delete an element from the clause priority queue.
void cQ_enqueue(clause*);			//To enqueue an element in the clause priority queue.
void cQ_touchClause_null(clause*);	//To disable the clause touching totally.
void cQ_touchClause_se(clause*);	//To touch a clause when doing subsumption elimination.
void cQ_touchClause_inp_se(clause*);//To touch a clause when doing SE in in-processing.
void cQ_touchClause_inp_str(clause*);//To touch a clause when doing STR in in-processing.
void cQ_touchClause_inp_treelook(clause*);//To do touch a clause when doing in-processing TREELOOK.

void (*cQ_touchClause)(clause*);	//This method points to one of the above cQ_touchClause() methods.
void cQ_flush();					//To remove all elements in the clause queue.

void cQ_reset();					//Reset the clause priority queue.
uint32_t cQ_init();					//For initializing once f.m_initial is known. Returns 0 if problems arise.
void cQ_dispose();					//For freeing all related memory.

//Methods for handling the clause stack.
void cS_delete(clause*);			//To delete a clause from the clause stack.
void cS_touchClause_null(clause*);	//To do absolutely nothing when touching a clause for the stack.
void cS_touchClause_str(clause*);	//To touch a clause while using the stack for strengthening clauses.
void cS_touchClause_ate(clause*);	//To touch a clause when doing asymmetric tautology elimination.
void (*cS_touchClause)(clause*);	//This points to one of the touching methods above.
void cS_push(clause*);				//To push a clause to the stack.
clause* cS_pop();					//To pop the topmost element from the stack.
clause* cS_read();					//To just read the topmost element from the stack.
void cS_drop();						//To just drop the topmost element.
void cS_flush();					//To remove all elements from the clause stack.

void cS_reset();					//Reset the clause stack.
uint32_t cS_init();					//For initializing once f.m_initial is known. Returns 0 if problems arise.
void cS_dispose();					//For freeing all related memory.

//Methods for handling the S0 clause stack.
void cSS0_delete(clause*);			//To delete a clause from the clause stack.
void cSS0_touchClause_null(clause*);//To do absolutely nothing when touching a clause for the stack.
void cSS0_touchClause_satelite(clause*);//To touch a clause while using the stack in SATELITE.
void (*cSS0_touchClause)(clause*);	//This points to one of the touching methods above.
void cSS0_push(clause*);			//To push a clause to the stack.
clause* cSS0_pop();					//To pop the topmost element from the stack.
clause* cSS0_read();				//To just read the topmost element from the stack.
void cSS0_drop();					//To just drop the topmost element.
void cSS0_flush();					//To remove all elements from the clause stack.

void cSS0_reset();					//Reset the clause stack.
uint32_t cSS0_init();				//For initializing once f.m_initial is known. Returns 0 if problems arise.
void cSS0_dispose();				//For freeing all related memory.

//Methods for handling the S1 clause stack.
void cSS1_delete(clause*);			//To delete a clause from the clause stack.
void cSS1_touchClause_null(clause*);//To do absolutely nothing when touching a clause for the stack.
void cSS1_touchClause_satelite(clause*);//To touch a clause while using the stack in SATELITE.
void (*cSS1_touchClause)(clause*);	//This points to one of the touching methods above.
void cSS1_push(clause*);			//To push a clause to the stack.
clause* cSS1_pop();					//To pop the topmost element from the stack.
clause* cSS1_read();				//To just read the topmost element from the stack.
void cSS1_drop();					//To just drop the topmost element.
void cSS1_flush();					//To remove all elements from the clause stack.

void cSS1_reset();					//Reset the clause stack.
uint32_t cSS1_init();				//For initializing once f.m_initial is known. Returns 0 if problems arise.
void cSS1_dispose();				//For freeing all related memory.

#endif /* CQUEUE_H_ */
