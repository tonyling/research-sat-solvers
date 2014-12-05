/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

/*
 * This file represents the external interface to the Dimetheus solver in case it is compiled as a library. Include this
 * header file in your program and link the library. You can then use this interface to use Dimetheus.
 */

#ifndef DIMETHEUS_IFC_H_
#define DIMETHEUS_IFC_H_

#include <stdint.h>

/*
 * The dimetheus_handleSignal(int) function is used to provide the information, that a specific signal has been caught by the
 * program, to the Dimetheus solver. The default behavior is to shut down independent of the signal. It returns SIGNAL.
 */
int32_t dimetheus_handleSignal(int);

/*
 * The dimetheus_getVersion(*int,*int,*int) function is used to return the major, minor, and revision version information.
 */
void dimetheus_getVersion(int32_t*,int32_t*,int32_t*);

/*
 * The dimetheus_getLicenseHash() function returns the hash of the license under which this version of the Dimetheus solver
 * runs. Go to https://www.gableske.net/dimetheus/license to see the type of license and verify that it is indeed valid.
 * Please contact the author (oliver@gableske.net) in case you encounter any problems.
 */
char* dimetheus_getLicenseHash();

/* The following methods are actually used to call the solver to solve something. The return codes of all the solve-methods
 * are as follows.
 * The method returns an integer that represents the result. We currently have:
 * UNKNOWN 0				//If no result was found or no computations did happen yet.
 * SAT 10					//If the formula is satisfiable.
 * UNSAT 20					//If the formula is unsatisfiable.
 * ERROR 30					//In case an error occurred during the execution (out of memory exceptions).
 * SPECIALPARAM_HELP 31		//In case the special parameter for showing the help was read.
 * SPECIALPARAM_VERSION 32	//In case the special parameter for showing the version was read.
 * SPECIALPARAM_SPECS 33	//In case the special parameter for showing the specs was read.
 * WRONGSOLUTION 35			//In case the solution failed verification.
 * BADPARAM 36				//If a given parameter was not recognized or a mandatory parameter was missing.
 * BADINPUTFORMAT 37		//In case the input format of the problem is not supported.
 * BADFORMULA 38			//If the formula is somehow flawed.
 * CONFLICTINGPARAMS 39		//If the provided parameters somehow do not make sense or are ambiguous.
 * SIGNAL 40				//If the computations where aborted due to a signal.
 */

/*
 * The dimetheus_solve_dimacscnf() function is executed to solve a given formula in DIMACS CNF input format. Parameters:
 * 1. argc: Provides the counter for how many parameters are handed over to the solver. The parameter count must be at least 1.
 * 2. argv: Provides a two-dimensional character array containing all the parameters. The very first parameter must always be
 *          the solver name "dimetheus". All consecutive parameters are two-paired tuples. The first is the parameter name,
 *          the second the parameter setting. A valid parameter setting could be:
 *          int argc = 3;
 *          char** argv = {"dimetheus", "-guide", "5"};
 * 3. n   : This parameter tells the solver how many variables occur in the formula that is to be solved.
 * 4. m   : This parameter tells the solver how many clauses will appear in the formula.
 * 5. clsA: This parameter is a two dimensional integer array that represents the formula. The first dimension represents
 *          the clause number and the second dimension gives the literal position for this clause. Literals are integers, e.g.
 *          for variable v_i, the positive literal is "i" and the negative literal is "-i". A clause must be terminated with a
 *          zero. A valid formula looks like this:
 *          int n = 3;
 *          int m = 3;
 *          int** clsA = {
 *                         {1, 2, 3, 0}
 *                         {-1, -2, -3, 0}
 *                         {1, 2, -3, 0}
 *                       }
 * 6. ass : This parameter is an array allocated to size n+1. In case the formula is satisfiable, all the values in this
 *          array will be overridden to contain the assignments to all variable (0 = false, 1 = true, 2 = unassigned).
 */
int32_t dimetheus_solve_dimacscnf(int32_t, char**, int32_t, int32_t, int32_t**, int32_t*);


/*
 * The dimetheus_solve_aplefoo() function is executed to solve a given formula in APLEFOO input format. Parameters:
 * 1. argc: Provides the counter for how many parameters are handed over to the solver. The parameter count must be at least 1.
 * 2. argv: Provides a two-dimensional character array containing all the parameters. The very first parameter must always be
 *          the solver name "dimetheus". All consecutive parameters are two-paired tuples. The first is the parameter name,
 *          the second the parameter setting. A valid parameter setting could be:
 *          int argc = 3;
 *          char** argv = {"dimetheus", "-guide", "5"};
 * 3. formula : This parameter hosts the formula as a string. Please check the APLEFOO description to check the format. An
 *              example formula is char * formula = "$ a & b | ( b & c ) | ( a > b )" (don't forget the "$" symbol).
 * 4. solution: This parameter points to a two dimensional character array, that must be initialized in the first dimension in
 *              the amount of variables in the formula+1. The second dimension must be uninitialized and will be overridden by
 *              the solver to contain the variable name followed by a space and either 0 for false, 1 for true, or 2 for
 *              unassigned. Example for the formula given above:
 *              You hand over	: char **solution = {NULL, NULL, NULL, NULL}.
 *              You get			: char **solution = {NULL, "a 1", "b 1", "c 2"}.
 */
int32_t dimetheus_solve_aplefoo(int32_t, char**, char*, char**);

#endif /* DIMETHEUS_IFC_H_ */
