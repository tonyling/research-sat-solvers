/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DETERM_H_
#define DETERM_H_

#include "../startup.h"

#define DETERM_VERSION_MA 1
#define DETERM_VERSION_MI 120
#define DETERM_VERSION_NAME "DETERM"

//GLOBAL data-structures for DETERM.
int32_t determ_returnCode;					//The return-code of the call to the input format determination component.

//PLUGINS for DETERM.
#include "_DIMACSCNF/det_dimacscnf.h"
#include "_APLEFOO/det_aplefoo.h"

//Methods exclusively accessible to the DETERM module (INTERN).
//None.

//Methods PROVIDED by this module (EXTERN).
void determ_extern_printVersion();			//Just print the version information of this module.
void determ_extern_determineInputFormat();	//Acquire the input format.

//Methods to initialize this module (called by the one who wants to use the provided method).
void determ_resetModule();					//To reset the DETERM module.
void determ_initModule();					//To initialize all its data structures.
void determ_disposeModule();				//To dispose all its data structures.

#endif /* DETERM_H_ */
