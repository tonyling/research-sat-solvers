/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RANDOMFOREST_H_
#define RANDOMFOREST_H_

#include "../classify.h"
#include "randomforest_generic.h"
#include "randomforest_random.h"
#include "randomforest_crafted.h"
#include "randomforest_application.h"

#define RANDOMFOREST_VERSION_MA 2
#define RANDOMFOREST_VERSION_MI 300
#define RANDOMFOREST_VERSION_NAME "RANDOMFOREST"

//GLOBAL data-structures for RANDOMFOREST.
int32_t randomforest_returnCode;				//The return-code of the call to the random forest component.

//PLUGINS for RANDOMFOREST.
//None.

//Methods PROVIDED by this module (EXTERN).
void randomforest_extern_printVersion();		//Just print the version information of this module.
void randomforest_extern_classify();			//Performs classification using the random forest.

//Methods to initialize this module (called by the one who wants to use the provided method).
void randomforest_resetModule();				//To reset the RANDOMFOREST module.
void randomforest_initModule();					//To initialize all its data structures.
void randomforest_disposeModule();				//To dispose all its data structures.

#endif /* RANDOMFOREST_H_ */
