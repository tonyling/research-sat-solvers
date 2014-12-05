/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DIMETHEUS_IFC_GLOBALDS_H_
#define DIMETHEUS_IFC_GLOBALDS_H_

//This file just contains some global data-structures in case the solver runs in library mode. Do NOT include this in your
//software project. It is not intended to be used by any other software.
int32_t dimetheus_ifc_n;
int32_t dimetheus_ifc_m;
int32_t** dimetheus_ifc_clsA;
int32_t* dimetheus_ifc_assA;
char* dimetheus_ifc_aplefooFormula;
char** dimetheus_ifc_aplefooSolution;

#endif /* DIMETHEUS_IFC_GLOBALDS_H_ */
