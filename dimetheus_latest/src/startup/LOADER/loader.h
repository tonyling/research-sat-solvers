/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef LOADER_H_
#define LOADER_H_

#include "../startup.h"

#define LOADER_VERSION_MA 1
#define LOADER_VERSION_MI 125
#define LOADER_VERSION_NAME "LOADER"

//GLOBAL data-structures for LOADER.
int32_t loader_returnCode;					//The return-code of the call to the formula loading component.

//PLUGINS for LOADER.
#include "_DIMACSCNF/load_dimacscnf.h"
#include "_APLEFOO/load_aplefoo.h"

//Methods exclusively accessible to the LOADER module (INTERN).
//None.

//Methods PROVIDED by this module (EXTERN).
void loader_extern_printVersion();			//Just print the version information of this module.
void loader_extern_loadAsCNF();				//Load the formula into the CNF data-structure.

//Methods to initialize this module (called by the one who wants to use the provided method).
void loader_resetModule();					//To reset the LOADER module.
void loader_initModule();					//To initialize all its data structures.
void loader_disposeModule();				//To dispose all its data structures.

#endif /* LOADER_H_ */
