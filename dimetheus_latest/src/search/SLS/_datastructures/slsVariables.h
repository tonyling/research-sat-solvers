/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SLSVARIABLES_H_
#define SLSVARIABLES_H_

typedef struct {
	int32_t brake;			//The brake score of a literal.
} slsVariable;

#define SET_SLS_VAR_BRAKE( __l, __b )			( sls_variables[abs(__l)].brake = __b )
#define SET_SLS_VAR_BRAKE_INC( __l )			( ++sls_variables[abs(__l)].brake )
#define SET_SLS_VAR_BRAKE_DEC( __l )			( --sls_variables[abs(__l)].brake )
#define GET_SLS_VAR_BRAKE( __l )				( sls_variables[abs(__l)].brake )


#define SET_SLS_VAR_ASSIGNMENT_TRUE( __var ) ( \
	sls_assignments[(( __var) >> 6ULL)] |= (1ULL << (__var & 63ULL)) \
)

#define SET_SLS_VAR_ASSIGNMENT_FALSE( __var ) ( \
	sls_assignments[(( __var) >> 6ULL)] &= ~(1ULL << (__var & 63ULL)) \
)

#define SET_SLS_VAR_ASSIGNMENT_FLIP( __var ) (\
		sls_assignments[(( __var) >> 6ULL)] ^= (1ULL << (__var & 63ULL))\
)

#define GET_SLS_VAR_ASSIGNMENT( __var ) ( \
	(((sls_assignments[(( __var) >> 6ULL)]) >> (( __var) & 63ULL) ) & 1ULL) \
)


void sls_variables_prepareCall();
void sls_variables_prepareTry();

void sls_variables_reset();
void sls_variables_init();
void sls_variables_dispose();

#ifdef VERBOSE_SLS
void sls_variables_printAll();
void sls_variables_printAssignment();
#endif
#endif /* SLSVARIABLES_H_ */
