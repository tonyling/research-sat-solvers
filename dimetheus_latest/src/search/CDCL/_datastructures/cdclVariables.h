/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCLVARIABLES_H_
#define CDCLVARIABLES_H_

typedef struct {
	cdclClause* reason;		//In case this variable was put on the stack via implication, this is the clause reason for it.
	float_ty activity;		//The variable activity for various heuristics like VSIDS.
	float_ty agility;		//The variable agility for restart heuristics.
	uint32_t baseData;		//2 bit global assignment, 30 bits offset.
	uint32_t phase;			//The phase of the variable.
	uint32_t decLevel;		//In what decision level was this variable put into the decision stack?
	uint32_t stamp;			//The stamp of the variable.
	int32_t id;				//The number of the variable, i.e. 1, 2, ..., 42...
	int32_t isEquivTo;		//In case the equivTo variable was assigned, we must assign the current variable the same.
	uint32_t vSPos;			//The position of this variable in the variable stack.
} cdclVariable;

typedef struct cdclEqVar cdclEqVar;

struct cdclEqVar {
	cdclEqVar* representedBy;
	uint32_t numRepresents;
	cdclVariable** equiv;
};

#define CDCL_VAR_HEADER_MASK 3221225472U		//1100 0000 0000 0000 0000 0000 0000 0000
#define CDCL_VAR_ASSIGNMENT_MASK 3221225472U	//1100 0000 0000 0000 0000 0000 0000 0000
#define CDCL_VAR_UNASSIGNED_MASK 2147483648U	//1000 0000 0000 0000 0000 0000 0000 0000
#define CDCL_VAR_TRUE 3221225472U				//1100 0000 0000 0000 0000 0000 0000 0000
#define CDCL_VAR_FALSE 2147483648U				//1000 0000 0000 0000 0000 0000 0000 0000
#define CDCL_VAR_OFFSET_MASK 1073741823U 		//0011 1111 1111 1111 1111 1111 1111 1111

#define GET_CDCL_VAR_ID_FROM_LIT_PTR( __lPtr )	((int32_t)(cdcl_literals > __lPtr ? cdcl_literals - __lPtr : __lPtr - cdcl_literals))

//The following macros are used to access the variables header data (i.e. check if it is assigned, if it is true or false...).
#define IS_CDCL_VAR_UNASSIGNED( __v )		( ( cdcl_variables[__v].baseData & CDCL_VAR_UNASSIGNED_MASK ) == 0U )
#define IS_CDCL_VAR_TRUE( __v )				( ((cdcl_variables[__v].baseData & CDCL_VAR_ASSIGNMENT_MASK) == CDCL_VAR_TRUE) )
#define IS_CDCL_VAR_FALSE( __v )			( ((cdcl_variables[__v].baseData & CDCL_VAR_ASSIGNMENT_MASK) == CDCL_VAR_FALSE) )

#define IS_CDCL_VAR_PTR_UNASSIGNED( __vPtr )	( ( __vPtr->baseData & CDCL_VAR_UNASSIGNED_MASK ) == 0U )
#define IS_CDCL_VAR_PTR_TRUE( __vPtr ) 		( (( __vPtr->baseData & CDCL_VAR_ASSIGNMENT_MASK ) == CDCL_VAR_TRUE) )
#define IS_CDCL_VAR_PTR_FALSE( __vPtr )		( (( __vPtr->baseData & CDCL_VAR_ASSIGNMENT_MASK ) == CDCL_VAR_FALSE) )

//The following macro is used to flip the assignment to a variable by pointer.
#define SET_CDCL_VAR_PTR_ASS_FLIP( __vPtr) ( IS_CDCL_VAR_PTR_TRUE(__vPtr) ? SET_CDCL_VAR_PTR_ASS_0(__vPtr) : SET_CDCL_VAR_PTR_ASS_1(__vPtr) )

//The following macros are used to get and set the base data and other values for the variable given as integer.
#define GET_CDCL_VAR_ACTIVITY( __v ) 	( cdcl_variables[__v].activity )
#define GET_CDCL_VAR_AGILITY( __v ) 	( cdcl_variables[__v].agility )
#define GET_CDCL_VAR_OFFSET( __v )		( cdcl_variables[__v].baseData & CDCL_VAR_OFFSET_MASK )
#define GET_CDCL_VAR_OFFSET_UNASS( __v ) ( cdcl_variables[__v].baseData )
#define GET_CDCL_VAR_RAWASS( __v )		( cdcl_variables[__v].baseData & CDCL_VAR_ASSIGNMENT_MASK )
#define GET_CDCL_VAR_ASS_UINT( __v ) 	 (GET_CDCL_VAR_RAWASS( __v ) >> 30 )
#define GET_CDCL_VAR_ID( __v )			( cdcl_variables[__v].id )
#define GET_CDCL_VAR_BASEDATA( __v )	( cdcl_variables[__v].baseData )
#define GET_CDCL_VAR_PHASE( __v )		( cdcl_variables[__v].phase )
#define GET_CDCL_VAR_STAMP( __v )		( cdcl_variables[__v].stamp )
#define GET_CDCL_VAR_REASON( __v )		( cdcl_variables[__v].reason )
#define GET_CDCL_VAR_DECLEVEL( __v )	( cdcl_variables[__v].decLevel )
#define GET_CDCL_VAR_ISEQUIVTO( __v )	( cdcl_variables[__v].isEquivTo )
#define GET_CDCL_VAR_SPOS( __v )		( cdcl_variables[__v].vSPos )

#define SET_CDCL_VAR_ACTIVITY( __v , __a )	( cdcl_variables[__v].activity = __a )
#define SET_CDCL_VAR_AGILITY( __v , __a )	( cdcl_variables[__v].agility = __a )
#define SET_CDCL_VAR_OFFSET( __v , __p ) 	( cdcl_variables[__v].baseData = ( cdcl_variables[__v].baseData & CDCL_VAR_HEADER_MASK ) | __p )
#define SET_CDCL_VAR_OFFSET_UNASS( __v, __p ) ( cdcl_variables_[__v].baseData = __p )
#define SET_CDCL_VAR_ASS_2( __v )			( cdcl_variables[__v].baseData = cdcl_variables[__v].baseData & ~CDCL_VAR_UNASSIGNED_MASK  )
#define SET_CDCL_VAR_ASS_1( __v )			( cdcl_variables[__v].baseData = cdcl_variables[__v].baseData | CDCL_VAR_TRUE )
#define SET_CDCL_VAR_ASS_0( __v )			( cdcl_variables[__v].baseData = ( cdcl_variables[__v].baseData & ~CDCL_VAR_ASSIGNMENT_MASK ) | CDCL_VAR_FALSE)
#define SET_CDCL_VAR_ID( __v , __i )		( cdcl_variables[__v].id = __i )
#define SET_CDCL_VAR_BASEDATA( __v , __b )	( cdcl_variables[__v].baseData = __b )
#define SET_CDCL_VAR_PHASE( __v , __p )		( cdcl_variables[__v].phase = __p )
#define SET_CDCL_VAR_STAMP( __v , __s )		( cdcl_variables[__v].stamp = __s )
#define SET_CDCL_VAR_REASON( __v , __r )	( cdcl_variables[__v].reason = __r )
#define SET_CDCL_VAR_DECLEVEL( __v , __dl )	( cdcl_variables[__v].decLevel = __dl )
#define SET_CDCL_VAR_ISEQUIVTO( __v , __e )	( cdcl_variables[__v].isEquivTo = __e )
#define SET_CDCL_VAR_SPOS( __v , __p )		( cdcl_variables[__v].vSPos = __p )

#define SET_CDCL_VAR_ACT_INC_BY( __v , __val ) ( cdcl_variables[__v].activity += __val )
#define SET_CDCL_VAR_ACT_NORM_BY( __v , __val )  ( cdcl_variables[__v].activity = (cdcl_variables[__v].activity / __val) )

#define SET_CDCL_VAR_AGI_INC_BY( __v , __val ) ( cdcl_variables[__v].agility += __val )
#define SET_CDCL_VAR_AGI_NORM_BY( __v , __val )  ( cdcl_variables[__v].agility = (cdcl_variables[__v].agility / __val) )

//The following macros are used to get and set the base data and other values for the variable given as pointer.
#define GET_CDCL_VAR_PTR_ACTIVITY( __vPtr ) 	( __vPtr->activity )
#define GET_CDCL_VAR_PTR_AGILITY( __vPtr ) 		( __vPtr->agility )
#define GET_CDCL_VAR_PTR_OFFSET( __vPtr )		( __vPtr->baseData & CDCL_VAR_OFFSET_MASK )
#define GET_CDCL_VAR_PTR_OFFSET_UNASS( __vPtr )	( __vPtr->baseData )
#define GET_CDCL_VAR_PTR_RAWASS( __vPtr )		( __vPtr->baseData & CDCL_VAR_ASSIGNMENT_MASK )
#define GET_CDCL_VAR_PTR_ASS_UINT( __vPtr ) 	( GET_CDCL_VAR_PTR_RAWASS( __vPtr ) >> 30 )
#define GET_CDCL_VAR_PTR_ID( __vPtr )			( __vPtr->id )
#define GET_CDCL_VAR_PTR_BASEDATA( __vPtr )		( __vPtr->baseData )
#define GET_CDCL_VAR_PTR_PHASE( __vPtr )		( __vPtr->phase )
#define GET_CDCL_VAR_PTR_STAMP( __vPtr )		( __vPtr->stamp )
#define GET_CDCL_VAR_PTR_REASON( __vPtr )		( __vPtr->reason )
#define GET_CDCL_VAR_PTR_DECLEVEL( __vPtr )		( __vPtr->decLevel )
#define GET_CDCL_VAR_PTR_ISEQUIVTO( __vPtr )	( __vPtr->isEquivTo )
#define GET_CDCL_VAR_PTR_SPOS( __vPtr )			( __vPtr->vSPos )

#define SET_CDCL_VAR_PTR_ACTIVITY( __vPtr , __a )	( __vPtr->activity = __a )
#define SET_CDCL_VAR_PTR_AGILITY( __vPtr , __a )	( __vPtr->agility = __a )
#define SET_CDCL_VAR_PTR_OFFSET( __vPtr , __p )		( __vPtr->baseData = ( __vPtr->baseData & CDCL_VAR_HEADER_MASK ) | __p )
#define SET_CDCL_VAR_PTR_OFFSET_UNASS( __vPtr , __p )	( __vPtr->baseData = __p )
#define SET_CDCL_VAR_PTR_ASS_2( __vPtr )			( __vPtr->baseData = __vPtr->baseData & ~CDCL_VAR_UNASSIGNED_MASK  )
#define SET_CDCL_VAR_PTR_ASS_1( __vPtr )			( __vPtr->baseData = __vPtr->baseData | CDCL_VAR_TRUE  )
#define SET_CDCL_VAR_PTR_ASS_0( __vPtr )			( __vPtr->baseData = ( __vPtr->baseData & ~CDCL_VAR_ASSIGNMENT_MASK ) | CDCL_VAR_FALSE )
#define SET_CDCL_VAR_PTR_ID( __vPtr , __i )			( __vPtr->id = __i )
#define SET_CDCL_VAR_PTR_BASEDATA( __vPtr , __b )	( __vPtr->baseData  = __b )
#define SET_CDCL_VAR_PTR_PHASE( __vPtr , __p )		( __vPtr->phase  = __p )
#define SET_CDCL_VAR_PTR_STAMP( __vPtr , __s)		( __vPtr->stamp  = __s )
#define SET_CDCL_VAR_PTR_REASON( __vPtr , __r ) 	( __vPtr->reason = __r )
#define SET_CDCL_VAR_PTR_DECLEVEL( __vPtr , __d )	( __vPtr->decLevel = __d )
#define SET_CDCL_VAR_PTR_ISEQUIVTO( __vPtr , __e )	( __vPtr->isEquivTo = __e )
#define SET_CDCL_VAR_PTR_SPOS( __vPtr , __p )		( __vPtr->vSPos = __p )

#define SET_CDCL_VAR_PTR_ACT_INC_BY( __vPtr , __val ) ( __vPtr->activity += __val )
#define SET_CDCL_VAR_PTR_ACT_NORM_BY( __vPtr , __val )  ( __vPtr->activity = ONE + ( __vPtr->activity / __val) )

#define SET_CDCL_VAR_PTR_AGI_INC_BY( __vPtr , __val ) ( __vPtr->agility += __val )
#define SET_CDCL_VAR_PTR_AGI_NORM_BY( __vPtr , __val )  ( __vPtr->agility = ONE + ( __vPtr->agility / __val) )

//Methods for handling the CDCL variable priority queue. THIS IS FOR THE VARIABLE SELECTION HEURISTIC ONLY.
void cdcl_variables_Q_moveUp(cdclVariable*);		//To move an element up in the CDCL variable priority queue.
void cdcl_variables_Q_moveDown(cdclVariable*);		//To move an element down in the CDCL variable priority queue.
cdclVariable* cdcl_variables_Q_readBest();			//To return the topmost element without deleting it from the queue.
void cdcl_variables_Q_delete(cdclVariable*);		//To delete an element from the CDCL variable priority queue.
void cdcl_variables_Q_enqueue(cdclVariable*);		//To enqueue an element in the CDCL variable priority queue.
void cdcl_variables_Q_flush();						//To remove all elements in the CDCL variable queue.

//Methods for handling the variable stack.
void cdcl_variables_S_delete(cdclVariable*);		//To delete a variable from the variable stack.
void cdcl_variables_S_touchVariable_null(cdclVariable*);//To do nothing when touching a variable.
void cdcl_variables_S_touchVariable_inprocessing_pl(cdclVariable*);//To touch a variable for the variable stack.
void (*cdcl_variables_S_touchVariable)(cdclVariable*);//This points to one of the touching methods above.
cdclVariable* cdcl_variables_S_pop();				//To pop the topmost element from the stack.
void cdcl_variables_S_flush();						//To remove all elements from the variable stack.

cdclVariable* cdcl_variables_getRootRepresentative(cdclVariable *);//Grab the root equivalence representative for a variable.
void cdcl_variables_flipVarWithEquivs(cdclVariable*);//Flips a variable assignment together with all equivalent variables.
void cdcl_variables_assignEquivVars_assign(cdclVariable*);//Assigns an equivalent variable if the root is assigned.
void cdcl_variables_assignRemainingVars();			//This assigns all variables that the CDCL is allowed to assign.

void cdcl_variables_prepareCall();

void cdcl_variables_reset();
void cdcl_variables_init();
void cdcl_variables_dispose();

#ifdef VERBOSE_CDCL
void cdcl_variables_printQueue_var(cdclVariable*, uint32_t);
void cdcl_variables_printQueue();
void cdcl_variables_printEnabled();
void cdcl_variables_printDisabled();
void cdcl_variables_printAssignments();
void cdcl_variables_printVariable(cdclVariable*);
void cdcl_variables_printAll();

uint32_t cdcl_variables_verifyQueueChildren(cdclVariable*);
uint32_t cdcl_variables_verifyQueue();
#endif
#endif /* CDCLVARIABLES_H_ */
