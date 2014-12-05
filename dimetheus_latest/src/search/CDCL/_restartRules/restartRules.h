/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RESTARTRULES_H_
#define RESTARTRULES_H_

#include "../cdcl.h"

#define CDCL_RESTARTRULE_MINVALUE 0
#define CDCL_RESTARTRULE_MAXVALUE 6

#define CDCL_RESTARTRULE_NULL 0	//If no restarts are to be performed.
#include "rr_null.h"

#define CDCL_RESTARTRULE_LUBY 1	//Using the Luby series a.k.a. the reluctant doubling sequence.
#include "rr_luby.h"

#define CDCL_RESTARTRULE_LUBYAGILITY 2	//Using the Luby series, but do not restart if the variable agility is high.
#include "rr_lubyagility.h"

#define CDCL_RESTARTRULE_LUBYSTAG 3	//Using the Luby series, but prohibit restarts if stack height does not stagnate.
#include "rr_lubystag.h"

#define CDCL_RESTARTRULE_INNEROUTER 4	//Using the inner/outer geometric series restarts.
#include "rr_innerouter.h"

#define CDCL_RESTARTRULE_INNEROUTERAGILITY 5	//Using the inner/outer geometric series, but no restart if high agility.
#include "rr_innerouteragility.h"

#define CDCL_RESTARTRULE_INNEROUTERSTAG 6	//Using the inner/outer geometric series, but no restart if low stagnation.
#include "rr_innerouterstag.h"

//Restart Rule plug-in internal data-structures.
float_ty cdcl_restartRules_currentAgiInc;		//This is the value with which we increase the variable agilities (this
												//value gets bumped after each restart by the bump parameter).
float_ty cdcl_restartRules_maxAgility;			//The maximum agility currently encountered.
uint32_t cdcl_restartRules_backjumpDistance;	//The sum of all distances of back-jumps in the last restart frame.
uint32_t cdcl_restartRules_backjumpHeight;		//The sum of all the conflict levels from which we back-jump.

static inline void cdcl_restartRules_printHelp(){
	cdcl_restartRule_null_printHelp();
	cdcl_restartRule_luby_printHelp();
	cdcl_restartRule_lubyagility_printHelp();
	cdcl_restartRule_lubystag_printHelp();
	cdcl_restartRule_innerouter_printHelp();
	cdcl_restartRule_innerouteragility_printHelp();
	cdcl_restartRule_innerouterstag_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_restartRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_NULL %d\n", prefix, CDCL_RESTARTRULE_NULL);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_LUBY %d\n", prefix, CDCL_RESTARTRULE_LUBY);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_LUBYAGILITY %d\n", prefix, CDCL_RESTARTRULE_LUBYAGILITY);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_LUBYSTAG %d\n", prefix, CDCL_RESTARTRULE_LUBYSTAG);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_INNEROUTER %d\n", prefix, CDCL_RESTARTRULE_INNEROUTER);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_INNEROUTERAGILITY %d\n", prefix, CDCL_RESTARTRULE_INNEROUTERAGILITY);
	printf("%s CDCL-RESTART-RULES: CDCL_RESTARTRULE_INNEROUTERSTAG %d\n", prefix, CDCL_RESTARTRULE_INNEROUTERSTAG);
}
void (*cdcl_restartRule_prepare)();	//This points to one of the restart rules call preparation methods.
uint32_t (*cdcl_restartRule_check)();//This points to one of the restart rules and is given the current number of
										//conflicts discovered by the CDCL. This method returns 0 if no restart is
										//to be performed and 1 if we should restart.
void (*cdcl_restartRule_incAgility)(cdclVariable*);//Bumps the agility of a variable if the assignment is not the phase.
void (*cdcl_restartRule_signalBackjump)();//Signals back-jump from level to level.

void cdcl_extern_restartRules_switch(int32_t);//Use to switch the restart rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_restartRules_resetPlugin();
void cdcl_restartRules_initPlugin();
void cdcl_restartRules_disposePlugin();

#endif /* RESTARTRULES_H_ */
