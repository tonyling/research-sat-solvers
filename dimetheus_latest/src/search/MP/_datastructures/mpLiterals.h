/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MPLITERALS_H_
#define MPLITERALS_H_

#include "../mp.h"

typedef struct {
	float_ty TorF;
	uint32_t numOccs;
	uint32_t numZeros;
} mpLiteral;

#define GET_MP_LIT_TORF( __l )					( mp_literals[__l].TorF )
#define GET_MP_LIT_NUMOCCS( __l )				( mp_literals[__l].numOccs )
#define GET_MP_LIT_NUMZEROS( __l )				( mp_literals[__l].numZeros )

#define GET_MP_LIT_PROBFOR_SAT( __l )			( __l > 0 ? GET_MP_VAR_PROBFOR_ONE(__l) : 1.0f - GET_MP_VAR_PROBFOR_ONE(-__l))
#define GET_MP_LIT_PROBFOR_UNS( __l )			( __l > 0 ? 1.0f - GET_MP_VAR_PROBFOR_ONE(__l) : GET_MP_VAR_PROBFOR_ONE(-__l))

#define SET_MP_LIT_TORF( __l , __v)				( mp_literals[__l].TorF = __v )
#define SET_MP_LIT_NUMOCCS( __l , __n )			( mp_literals[__l].numOccs = __n )
#define SET_MP_LIT_NUMZEROS( __l , __z )		( mp_literals[__l].numZeros = __z )

#define SET_MP_LIT_NUMOCCSINC( __l )			( ++mp_literals[__l].numOccs )
#define SET_MP_LIT_NUMOCCSDEC( __l )			( --mp_literals[__l].numOccs )

#define SET_MP_LIT_NUMZEROSINC( __l )			( ++mp_literals[__l].numZeros )
#define SET_MP_LIT_NUMZEROSDEC( __l )			( --mp_literals[__l].numZeros )

#define SET_MP_LIT_APPLYWARN_MULT( __l , __m )	( mp_literals[__l].TorF *= (__m) )
#define SET_MP_LIT_REMOVEWARN_MULT( __l , __m )	( mp_literals[__l].TorF /= (__m) )



void mp_literals_prepareCall();

void mp_literals_reset();
void mp_literals_init();
void mp_literals_dispose();

#endif /* MPLITERALS_H_ */
