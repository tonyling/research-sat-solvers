/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include "../dimetheus.h"
#include "_fuzzing/paramfuzzing.h"
#ifdef SPECS_PARAMGRAPH
#include "_paramGraph/paramgraph.h"
#endif

uint32_t params_returnCode;

void params_extern_printHelp();								//Prints the help of the solver.
void params_extern_readSpecial(int, char**);				//This is called first and checks for special parameters.
void params_extern_readNormal(int, char**);					//This is called second and reads all normal parameters.
void params_extern_printAll(char*);							//Prints all parameters with their settings.
void params_extern_printAdaptableWithPreconditions(char*);	//Prints all parameter names with preconditions.

void params_resetPhase();
void params_initPhase();
void params_disposePhase();

#endif /* PARAMS_H_ */
