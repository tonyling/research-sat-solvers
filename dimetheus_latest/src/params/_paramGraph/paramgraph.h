/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PARAMGRAPH_H_
#define PARAMGRAPH_H_
#include "../params.h"
#ifdef SPECS_PARAMGRAPH
#define SPECS_ROOTNID 2
#define SPECS_NODEFID 3
uint32_t currNID;

#define SPECS_XMLNSXSI "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "

#define SPECS_NTYPE_A 1
#define SPECS_XSITYPE_AND "xsi:type=\"andNode\""

#define SPECS_NTYPE_O 2
#define SPECS_XSITYPE_OR "xsi:type=\"orNode\""

#define SPECS_DTYPE_I 1
#define SPECS_XSITYPE_INTEGER "xsi:type=\"integerDomain\""

#define SPECS_DTYPE_R 2
#define SPECS_XSITYPE_REAL "xsi:type=\"realDomain\""

#define SPECS_DTYPE_C 3
#define SPECS_XSITYPE_CAT "xsi:type=\"categoricalDomain\""

//Some helper functions that are used to write nodes of the graph.
void params_intern_writeNode_param(uint32_t, uint32_t, char*);
void params_intern_writeNode_domain(uint32_t, uint32_t, char*, uint32_t, float_ty, float_ty);
void params_intern_writeParametersTag(uint32_t,  float_ty, float_ty, char*);
void params_intern_writeEdgeTag(uint32_t, uint32_t);
void params_intern_appendONode(uint32_t, char*);
void params_intern_appendANode(uint32_t, char*, uint32_t, float_ty, float_ty);

void params_extern_writeParameterGraph(char*);	//This method generates the EDACC parameter graph of all tunable parameters.

#endif
#endif /* PARAMGRAPH_H_ */
