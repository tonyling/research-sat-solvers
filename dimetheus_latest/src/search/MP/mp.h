/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MP_H_
#define MP_H_

#include "../search.h"
#include "_datastructures/mpVariables.h"
#include "_datastructures/mpLiterals.h"
#include "_datastructures/mpClauses.h"

#define MP_VERSION_MA 2
#define MP_VERSION_MI 125
#define MP_VERSION_NAME "MP"

//GLOBAL data-structures for MP.
mpVariable* mp_variables;				//The variables for the MP module.
uint32_t mp_varNum;						//The number of variables in the snap-shot formula.

mpClause *mp_clauses;					//The clauses of the MP module.
uint32_t mp_clsUsed;					//The number of clauses used by the MP.
uint32_t mp_clsAvail;					//The number of clauses we have the memory allocated for.
uint32_t mp_clsMaxSize;					//The longest clause size.

mpLiteral *mp_literals;					//The literals of the MP module.

int32_t mp_returnCode;					//The return-code of the call to the message passing component.
float_ty *mp_clauseDeltas;				//All the factors from a clause to create a message.
float_ty mp_maxMessageDiff;				//The maximum message difference we encountered in the last iteration.
float_ty mp_globalMagnetization;		//The total magnetization of all active variables combined.

//PLUGINS for MP.
#include "_updateRules/updateRules.h"	//IMPLEMENTS: *mp_updateRule_: printHelp, msgUpdate, computeBias, prepare

//Methods used only by MP module (INTERN).
void mp_intern_retainMsgs();			//This method is called at the end of the iterations to retain the messages.
void mp_intern_retainBiases();			//This method is callet at the onf of the iterations to retain biases.

void mp_extern_printVersion();			//Just print the version information of this module.
void mp_extern_iterate(uint32_t);		//Iterate (the UINT set to one means it is suppose to re-prepare the call).
void mp_extern_computeBiases();			//Given the current messages, this method computes biases and determines paramagnetic.


//Methods to initialize this module (called by the one who wants to call the provided methods).
void mp_resetModule();					//To reset the MP module.
void mp_initModule();					//To initialize all its data structures.
void mp_disposeModule();				//To dispose all its data structures.
#endif /* MP_H_ */
