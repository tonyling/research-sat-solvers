/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MPVARIABLES_H_
#define MPVARIABLES_H_

#include "../mp.h"

typedef struct {
	float_ty bias;			//The bias of the variable. This is SIGNED. A value of -0.5 means it tends to be assigned to 0.
} mpVariable;

#define CONVERT_MP_VAR_BIAS_TO_ACTIVITY( __v )	( fabs(mp_variables[__v].bias) )

#define GET_MP_VAR_BIAS(__v)					( mp_variables[__v].bias )
#define SET_MP_VAR_BIAS(__v, __b)				( mp_variables[__v].bias = __b )

#define GET_MP_VAR_ASSIGNMENT_FAVORITE( __v ) 	( mp_variables[__v].bias > 0.0f ? 1U : 0U )

#define GET_MP_VAR_PROBFOR_ONE( __v )			( (ONE + mp_variables[__v].bias) / TWO )

#define GET_MP_VAR_BIGF( __v )					( GET_MP_LIT_NUMZEROS(__v) ? 0.0f : GET_MP_LIT_TORF(__v) )
#define GET_MP_VAR_BIGT( __v )					( GET_MP_LIT_NUMZEROS(-__v) ? 0.0f : GET_MP_LIT_TORF(-__v) )
#define GET_MP_VAR_BIGB( __v )					( GET_MP_VAR_BIGF( __v ) * GET_MP_VAR_BIGT( __v ) )

#define GET_MP_VAR_SMAF( __v )					( GET_MP_LIT_NUMOCCS( __v ) )
#define GET_MP_VAR_SMAT( __v )					( GET_MP_LIT_NUMOCCS(-__v ) )
#define GET_MP_VAR_SMAB( __v )					( GET_MP_VAR_SMAF( __v ) + GET_MP_VAR_SMAT( -__v ) )

void mp_variables_prepareCall();

void mp_variables_retainBiases();

void mp_variables_reset();
void mp_variables_init();
void mp_variables_dispose();

#ifdef VERBOSE_MP
void mp_variables_printVariable(mpVariable*);
#endif

#endif /* MPVARIABLES_H_ */
