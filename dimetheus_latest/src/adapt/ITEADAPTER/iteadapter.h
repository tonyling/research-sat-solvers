/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ITEADAPTER_H_
#define ITEADAPTER_H_

#include "../adapt.h"
#include "iteadapter_generic.h"

#define ITEADAPTER_VERSION_MA 2
#define ITEADAPTER_VERSION_MI 245
#define ITEADAPTER_VERSION_NAME "ITEADAPTER"

//Global variables and Macros of the ITEADAPTER module.
int32_t iteadapter_returnCode;						//The return code of the ITEADAPTER module.

//PLUGINS for ITEADAPTER.
//None.

//Methods PROVIDED by the module.
void iteadapter_extern_printVersion();				//Prints the version of this module.
void iteadapter_extern_adaptAllAllowed();			//Performs the adaptation.

//The following methods are used to initialize this module (called by the one who wants to use the external methods).
void iteadapter_resetModule();
void iteadapter_initModule();
void iteadapter_disposeModule();

#endif /* ITEADAPTER_H_ */
