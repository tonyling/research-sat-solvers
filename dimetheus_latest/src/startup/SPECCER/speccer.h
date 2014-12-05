/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SPECCER_H_
#define SPECCER_H_

#include "../startup.h"

#define SPECCER_VERSION_MA 1
#define SPECCER_VERSION_MI 125
#define SPECCER_VERSION_NAME "SPECCER"

//GLOBAL data-structures for SPECCER.
int32_t speccer_returnCode;					//The return-code of the call to the specification computation component.

//PLUGINS for SPECCER.
#include "_DIMACSCNF/spec_dimacscnf.h"
#include "_APLEFOO/spec_aplefoo.h"

//Methods exclusively accessible to the SPECCER module (INTERN).
//None.

//Methods PROVIDED by this module (EXTERN).
void speccer_extern_printVersion();			//Just print the version information of this module.
void speccer_extern_determineSpecification();//Acquire the input specification.

//Methods to initialize this module (called by the one who wants to use the provided method).
void speccer_resetModule();					//To reset the SPECCER module.
void speccer_initModule();					//To initialize all its data structures.
void speccer_disposeModule();				//To dispose all its data structures.

#endif /* SPECCER_H_ */
