/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef COMPLEXUP_H_
#define COMPLEXUP_H_

#include "tools.h"

literal** main_complexUP_current;//This points into the position in the current decision level where we are propagating.
literal** main_complexUP_end;	//This points into the position in the current decision level where we add new implications.

//The following macros are used to handle the MAIN complex UP.
#define MAIN_COMPLEXUP_SET_DECLEVEL( __dl ) { \
	main_complexUP_current = main_decStackPtrs[__dl]; \
	main_complexUP_end = main_complexUP_current; \
	MAIN_DECSTACK_REVERT_TO_LEVEL( __dl ); \
	++main_upStamp; \
}

#define MAIN_COMPLEXUP_SET_DECLEVEL_APPEND( __dl ) { \
	main_complexUP_current = main_decStackPtrs[ __dl+1 ] - 1; \
	main_complexUP_end = main_complexUP_current; \
	MAIN_DECSTACK_REVERT_TO_LEVEL( __dl ); \
	++main_upStamp; \
}

#define MAIN_COMPLEXUP_TERMINATE_CURRENT() { \
	*(main_complexUP_end++) = NULL; \
	MAIN_DECSTACK_PREPARE_NEW_LEVEL(); \
	MAIN_COMPLEXUP_SET_DECLEVEL( main_decLevel ); \
	++main_upStamp; \
}

#define MAIN_COMPLEXUP_IS_LIT_ON_S( __l ) 			( main_litData[__l].upStamp == main_upStamp )
#define MAIN_COMPLEXUP_IS_LIT_ON_S_PTR( __lPtr )	( __lPtr->upStamp == main_upStamp )

#define MAIN_COMPLEXUP_IS_OPPLIT_ON_S( __l )		( main_litData[-__l].upStamp == main_upStamp )
#define MAIN_COMPLEXUP_IS_OPPLIT_ON_S_PTR( __lPtr )	( (main_litData-GET_LIT_NUM_FROM_PTR( __lPtr))->upStamp == main_upStamp )

#define MAIN_COMPLEXUP_ADD_DEC( __l , __vPtr) { \
	__vPtr->reason = NULL; \
	__vPtr->decLevel = main_decLevel; \
	main_litData[__l].upStamp = main_upStamp; \
	*(main_complexUP_end++) = GET_LIT_PTR_FROM_NUM( __l ); \
}

#define MAIN_COMPLEXUP_ADD_DEC_PTR( __lPtr , __vPtr) { \
	__vPtr->reason = NULL; \
	__vPtr->decLevel = main_decLevel; \
	__lPtr->upStamp = main_upStamp; \
	*(main_complexUP_end++) = __lPtr; \
}

#define MAIN_COMPLEXUP_ADD_DEC_AT_POS_PTR( __lPtr , __vPtr, __pos ) {\
		__vPtr->reason = NULL; \
		__vPtr->decLevel = main_decLevel; \
		__lPtr->upStamp = main_upStamp; \
		*(main_complexUP_current + __pos) = __lPtr; \
}

#define MAIN_COMPLEXUP_ADD_IMP( __l , __vPtr, __r ) { \
	__vPtr->reason = __r; \
	__vPtr->decLevel = main_decLevel; \
	SET_CLS_REASON_1( __r ); \
	main_litData[__l].upStamp = main_upStamp; \
	*(main_complexUP_end++) = GET_LIT_PTR_FROM_NUM( __l ); \
}

#define MAIN_COMPLEXUP_ADD_IMP_PTR( __lPtr , __vPtr, __r ) { \
	__vPtr->reason = __r; \
	__vPtr->decLevel = main_decLevel; \
	SET_CLS_REASON_1( __r ); \
	__lPtr->upStamp = main_upStamp; \
	*(main_complexUP_end++) = __lPtr; \
}

clause* main_complexup_propagate(uint32_t);//Propagates from the current position in the decision stack.

void main_complexup_reset();	//To reset the complex unit propagation.
uint32_t main_complexup_init();	//To initialize the complex unit propagation. Return 0 if problems arise.
void main_complexup_dispose();	//To dispose the complex unit propagation.

#ifdef VERBOSE
void main_complexup_printCurrentLevel();
void main_complexup_printFromLevel(uint32_t);
#endif

#endif /* COMPLEXUP_H_ */
