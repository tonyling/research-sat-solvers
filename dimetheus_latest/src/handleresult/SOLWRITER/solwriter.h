/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SOLWRITER_H_
#define SOLWRITER_H_

#include "../handleresult.h"

#define SOLWRITER_VERSION_MA 1
#define SOLWRITER_VERSION_MI 115
#define SOLWRITER_VERSION_NAME "SOLWRITER"

//GLOBAL data-structures for SOLWRITER.
int32_t solwriter_returnCode;				//The return-code.

//PLUGINS for SOLWRITER.
#include "_DIMACSCNF/or_dimacscnf.h"
#include "_APLEFOO/or_aplefoo.h"

//Methods exclusively accessible to the SOLWRITER module (INTERN).
//None.

//Methods PROVIDED by this module (EXTERN).
void solwriter_extern_printVersion();		//Just print the version information of this module.
void solwriter_extern_writeSolution();		//Write the solution.

//Methods to initialize this module (called by the one who wants to use the provided method).
void solwriter_resetModule();				//To reset the SOLWRITER module.
void solwriter_initModule();				//To initialize all its data structures.
void solwriter_disposeModule();				//To dispose all its data structures.

#endif /* SOLWRITER_H_ */
