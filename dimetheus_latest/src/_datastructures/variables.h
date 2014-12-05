/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "../dimetheus.h"

typedef struct variable variable;
typedef struct eqVar eqVar;

struct variable {
	clause* reason;			//The reason (a clause) if this literal is found in the UP.
	float_ty activity;		//The activity of the variable.
	uint32_t baseData;		//2 bit global assignment (1 bit ISASSIGNED 1 bit PHASE), 1 bit enabled/disabled, 29 bits offset.
	int32_t id;				//The number of the variable, i.e. 1, 2, ..., 42...
	uint32_t decLevel;		//In what decision level was this variable assigned?
	int32_t isEquivTo;		//To what variable is this variable equivalent to? Same sign if larger zero.
	uint32_t vQPos;			//The position of the variable in the variable priority queue.
	int32_t vQScore;		//The score of the variable in the priority queue. The smaller the score, the better.
	uint32_t vSPos;			//The position of the variable in the variable stack.
};

struct eqVar {
	eqVar* representedBy;
	uint32_t numRepresents;
	variable** equiv;
};

//Bit-masks with 32-bit unsigned integers.
#define VAR_HEADER_MASK 3758096384U		//1110 0000 0000 0000 0000 0000 0000 0000
#define VAR_ASSIGNMENT_MASK 3221225472U	//1100 0000 0000 0000 0000 0000 0000 0000
#define VAR_UNASSIGNED_MASK 2147483648U	//1000 0000 0000 0000 0000 0000 0000 0000
#define VAR_PHASE_MASK 1073741824U		//0100 0000 0000 0000 0000 0000 0000 0000
#define VAR_TRUE 1073741824U			//0100 0000 0000 0000 0000 0000 0000 0000
#define VAR_FALSE 0U					//0000 0000 0000 0000 0000 0000 0000 0000
#define VAR_DISABLED_MASK 536870912U	//0010 0000 0000 0000 0000 0000 0000 0000
#define VAR_OFFSET_MASK	536870911U		//0001 1111 1111 1111 1111 1111 1111 1111

//The following macros are used to access the variables header data (i.e. check if it is assigned, if it is true or false...).

#define IS_VAR_UNASSIGNED( __vPtr )	( ( __vPtr->baseData & VAR_UNASSIGNED_MASK ) == VAR_UNASSIGNED_MASK )
#define IS_VAR_TRUE( __vPtr ) 		( (!IS_VAR_UNASSIGNED(__vPtr))&&(( __vPtr->baseData & VAR_ASSIGNMENT_MASK ) == VAR_TRUE) )
#define IS_VAR_FALSE( __vPtr )		( (!IS_VAR_UNASSIGNED(__vPtr))&&( __vPtr->baseData & VAR_ASSIGNMENT_MASK ) == VAR_FALSE )
#define IS_VAR_DISABLED( __vPtr )	( ( __vPtr->baseData & VAR_DISABLED_MASK ) == VAR_DISABLED_MASK )

#define GET_VAR_HEADER( __vPtr )	( __vPtr->baseData & VAR_HEADER_MASK )
#define GET_VAR_RAWASS( __vPtr )	( __vPtr->baseData & VAR_ASSIGNMENT_MASK )
#define GET_VAR_RAWDIS( __vPtr )	( __vPtr->baseData & VAR_DISABLED_MASK )
#define GET_VAR_RAWPHA( __vPtr )	( __vPtr->baseData & VAR_PHASE_MASK )
#define GET_VAR_OFFSET( __vPtr )	( __vPtr->baseData & VAR_OFFSET_MASK )

#define GET_VAR_ACTIVITY( __vPtr )	(__vPtr->activity )
#define GET_VAR_ASS_UINT( __vPtr )  (GET_VAR_RAWASS( __vPtr ) >> 30 )
#define GET_VAR_DIS_UINT( __vPtr )  (GET_VAR_RAWDIS( __vPtr ) >> 29 )
#define GET_VAR_PHASE( __vPtr )		( (__vPtr->baseData & VAR_PHASE_MASK) == VAR_TRUE )

#define GET_VAR_ID( __vPtr )		( __vPtr->id )

#define GET_VAR_ISEQUIVTO( __vPtr )	( __vPtr->isEquivTo )

#define GET_VAR_REASON( __vPtr )	( __vPtr->reason )

#define GET_VAR_DECLEVEL( __vPtr )	( __vPtr->decLevel )

//The following macros are used to set the variable data.
#define SET_VAR_ASS_2( __vPtr )		( __vPtr->baseData =  __vPtr->baseData | VAR_UNASSIGNED_MASK )
#define SET_VAR_ASS_1( __vPtr )		( __vPtr->baseData =  ( __vPtr->baseData | VAR_TRUE ) & ~VAR_UNASSIGNED_MASK )
#define SET_VAR_ASS_0( __vPtr )		( __vPtr->baseData = __vPtr->baseData & ~VAR_ASSIGNMENT_MASK )
#define SET_VAR_ASS_FLIP( __vPtr )	( __vPtr->baseData = __vPtr->baseData ^ VAR_TRUE )
#define SET_VAR_PHASE_1( __vPtr )	( __vPtr->baseData |= VAR_PHASE_MASK )
#define SET_VAR_PHASE_0( __vPtr )	( __vPtr->baseData &= ~VAR_PHASE_MASK )

#define SET_VAR_ID( __vPtr , __id )	( __vPtr->id = __id )

#define SET_VAR_ISEQUIVTO( __vPtr , __e )	( __vPtr->isEquivTo = __e )

#define SET_VAR_ACTIVITY( __vPtr , __a )	(__vPtr->activity = __a )

#define SET_VAR_DISABL_1( __vPtr )	( __vPtr->baseData |= VAR_DISABLED_MASK )
#define SET_VAR_DISABL_0( __vPtr )	( __vPtr->baseData = __vPtr->baseData & ~VAR_DISABLED_MASK )

#define SET_VAR_OFFSET( __vPtr , __p ) ( __vPtr->baseData = ( __vPtr->baseData & VAR_HEADER_MASK ) | __p )

#define SET_VAR_REASON( __vPtr , __r )	( __vPtr->reason = __r )

#define SET_VAR_DECLEVEL( __vPtr, __d )	( __vPtr->decLevel = __d )

//The following macros are used to access the variables queue relevant data.
#define SET_VAR_Q_POS( __vPtr , __value ) 	( __vPtr->vQPos = __value )
#define SET_VAR_Q_SCORE( __vPtr , __value )	( __vPtr->vQScore = __value )
#define GET_VAR_Q_POS( __vPtr ) 			( __vPtr->vQPos )
#define GET_VAR_Q_SCORE( __vPtr )			( __vPtr->vQScore )

#define SET_VAR_S_POS( __vPtr , __value ) 	( __vPtr->vSPos = __value )
#define GET_VAR_S_POS( __vPtr ) 			( __vPtr->vSPos )

//The following macros are used to remove or add a variable from or to the respective list.
#define REM_VAR_FROM_VARS_LIST( __list , __pos , __used ) { \
		__list[ __pos ] = __list[ --__used ]; \
		__list[ __pos ]->baseData = ( __list[ __pos ]->baseData & VAR_HEADER_MASK ) | __pos; \
}

#define SWITCH_VAR_ENABLED_DISABLED_LIST( __list, __vPtr , __used ) { \
		__list[ __used ] = __vPtr; \
		__vPtr->baseData = ( ( __vPtr->baseData & VAR_HEADER_MASK ) ^ VAR_DISABLED_MASK) | __used++; \
}


void variables_reset();								//To reset the variable related data for a clean start.
uint32_t variables_init();							//To initialize the variables of the formula. Returns 0 if unsuccessful.
void variables_resetActivitiesAndPhases(); 			//Set the initial activities and phases for all variables.
void variables_dispose();							//To free all the variable related data.

void variables_disable(variable*);					//To disable a single variable.
void variables_enable(variable*);					//To re-enable a single variable.

void variables_assignEquivVars_assign(variable*); 	//To assign a variable if its equivalent representative is assigned.

#ifdef VERBOSE
void variables_printVariableAssignments();			//This will output all the variable assignments.
void variables_printVariable(variable*);			//This will output a single comment line with all the variable data.
void variables_printEnabledVariables();				//This will print all currently enabled variables.
void variables_printDisabledVariables();			//This will print all currently disabled variable.
void variables_printEnabledVariablesData(); 		//Prints all the data to the enabled variables.

uint32_t variables_verifyVariable(variable*);		//Verify a single variable for consistency.
uint32_t variables_verifyAll();						//Check all variable data for consistency.
#endif

#endif /* VARIABLES_H_ */
