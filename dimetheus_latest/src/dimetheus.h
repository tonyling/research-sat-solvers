/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DIMETHEUS_H_
#define DIMETHEUS_H_

#include "defines.h"
#include "license.h"
#include "params/parameters.h"

//Interface includes.
#ifdef COMPILEASLIBRARY
#include "_interface/dimetheus_ifc_globalds.h"
#include "_interface/dimetheus_ifc.h"
#endif

#include "_datastructures/clauses.h"
#include "_datastructures/variables.h"
#include "_datastructures/literals.h"
#include "_datastructures/formula.h"

CNF f;							//The formula we are currently working with.
char** main_varNames;			//The variable names, in case the input/output format work with names instead of numbers.
variable* main_varData;			//The variable data array. Variable 42 is main_varData + 42.
literal* main_litData;			//The literal data array. Literal -42 is main_litData - 42.
int32_t main_returnCode;		//The return value of the solver. See defines.h for the values.
uint32_t main_litStamp;			//The stamp that is currently used for standard literal stamps.
uint32_t main_upStamp;			//The stamp that is currently used for unit propagation.

//Tool includes.
#include "_tools/tools.h"

//Component includes.
#include "reset/reset.h"
#include "params/params.h"
#include "startup/startup.h"
#include "guidance/guidance.h"
#include "classify/classify.h"
#include "adapt/adapt.h"
#include "prep/prep.h"
#include "search/search.h"
#include "handleresult/handleresult.h"
#include "shutdown/shutdown.h"

//The various phases of SAT solving as introduces by the Logic Algorithms Engine (LAE). See https://www.gableske.net/lae
//for more details and check https://www.gableske.net/dimetheus for more information on what goes on in these phases.
void main_reset();
void main_params(int, char**);
void main_guidance(int, char**);
void main_startup();
void main_classify();
void main_adapt();
void main_preprocess();
void main_search();
void main_handleresult();
void main_shutdown();

//Some helper methods for the main module of teh solver. Nothing too interesting.
void main_printSolverHeader();	//This prints the name and version number of the solver.
void main_printVersion();		//This prints the solver version and license hash.
void main_printSpecs();			//This prints some of the solver specifications.

#ifndef COMPILEASLIBRARY
void main_handle_signal(int);	//This is the signal handler. If the solver receives any signal, it will terminate.
int32_t main(int,char**);
#endif
#endif /* DIMETHEUS_H_ */
