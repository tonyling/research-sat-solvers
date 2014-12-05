/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCLUP_H_
#define CDCLUP_H_

#include "cdclTools.h"

uint32_t cdcl_up_stamp;			//The current stamp used to stamp unassigned literals already in the decision level.
cdclLiteral** cdcl_up_current;	//This points into the position in the current decision level where we are propagating.
cdclLiteral** cdcl_up_end;		//This points into the position in the current decision level where we add new implications.

//The following macros are used to handle the CDCL UP.
#define CDCL_UP_SET_DECLEVEL( __dl ) { \
	cdcl_up_current = cdcl_decStackPtrs[__dl]; \
	cdcl_up_end = cdcl_up_current; \
	CDCL_DECSTACK_REVERT_TO_LEVEL( __dl ); \
	++cdcl_up_stamp; \
}

#define CDCL_UP_SET_DECLEVEL_APPEND( __dl ) { \
	cdcl_up_current = cdcl_decStackPtrs[ __dl+1 ] - 1; \
	cdcl_up_end = cdcl_up_current; \
	CDCL_DECSTACK_REVERT_TO_LEVEL( __dl ); \
	++cdcl_up_stamp; \
}

#define CDCL_UP_TERMINATE_CURRENT() { \
	*(cdcl_up_end++) = NULL; \
	CDCL_DECSTACK_PREPARE_NEW_LEVEL(); \
	CDCL_UP_SET_DECLEVEL( cdcl_decLevel ); \
	++cdcl_up_stamp; \
}

#define CDCL_UP_IS_LIT_ON_S( __l ) 				( cdcl_literals[__l].upStamp == cdcl_up_stamp )
#define CDCL_UP_IS_LIT_ON_S_PTR( __lPtr )		( __lPtr->upStamp == cdcl_up_stamp )

#define CDCL_UP_IS_OPPLIT_ON_S( __l )			( cdcl_literals[-__l].upStamp == cdcl_up_stamp )
#define CDCL_UP_IS_OPPLIT_ON_S_PTR( __lPtr )	( (cdcl_literals-GET_CDCL_LIT_ID_FROM_PTR( __lPtr))->upStamp == cdcl_up_stamp )

#define CDCL_UP_ADD_DEC( __l , __vPtr) { \
	__vPtr->reason = NULL; \
	__vPtr->decLevel = cdcl_decLevel; \
	cdcl_literals[__l].upStamp = cdcl_up_stamp; \
	*(cdcl_up_end++) = GET_CDCL_LIT_PTR_FROM_ID( __l ); \
}

#define CDCL_UP_ADD_DEC_PTR( __lPtr , __vPtr) { \
	__vPtr->reason = NULL; \
	__vPtr->decLevel = cdcl_decLevel; \
	__lPtr->upStamp = cdcl_up_stamp; \
	*(cdcl_up_end++) = __lPtr; \
}

#define CDCL_UP_ADD_IMP( __l , __vPtr, __r ) { \
	__vPtr->reason = __r; \
	__vPtr->decLevel = cdcl_decLevel; \
	SET_CDCL_CLS_REASON_1( __r ); \
	cdcl_literals[__l].upStamp = cdcl_up_stamp; \
	*(cdcl_up_end++) = GET_CDCL_LIT_PTR_FROM_ID( __l ); \
}

#define CDCL_UP_ADD_IMP_PTR( __lPtr , __vPtr, __r ) { \
	__vPtr->reason = __r; \
	__vPtr->decLevel = cdcl_decLevel; \
	SET_CDCL_CLS_REASON_1( __r ); \
	__lPtr->upStamp = cdcl_up_stamp; \
	*(cdcl_up_end++) = __lPtr; \
}

cdclClause* cdcl_up_propagate();//Propagates from the current position in the decision stack.

void cdcl_up_prepare();			//To prepare the unit propagation for the next call to the CDCL module.

void cdcl_up_reset();			//To reset the unit propagation when the module is reset.
void cdcl_up_init();			//To initialize the unit propagation when the module is initialized.
void cdcl_up_dispose();			//To dispose the unit propagation when the module is disposed.

#ifdef VERBOSE_CDCL
void cdcl_up_printCurrentLevel();
#endif

#endif /* CDCLUP_H_ */
