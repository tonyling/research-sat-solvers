/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SATELITE_H_
#define SATELITE_H_

#include "../prep.h"

#define SATELITE_VERSION_MA 0
#define SATELITE_VERSION_MI 100
#define SATELITE_VERSION_NAME "SATELITE"

int32_t satelite_returnCode;

//Methods PROVIDED by the module (EXTERN).
void satelite_extern_printVersion();	//Prints the version of this module.
void satelite_extern_simplify();

//The following methods are used to initialize this module (called by the one who wants to use the external methods).
void satelite_resetModule();
void satelite_initModule();
void satelite_disposeModule();

#endif /* SATELITE_H_ */
