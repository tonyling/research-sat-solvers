/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef FORMULA_H_
#define FORMULA_H_

#include "../dimetheus.h"

typedef struct {			//The structure to store trend information for an attribute array.
	float_ty min;			//The minimum in the array.
	float_ty max;			//The maximum in the array.
	float_ty mean;			//The mean of the array.
	float_ty median;		//The median of the array.
	float_ty sent;			//The Shannon entropy of the array.
	float_ty rent;			//The Renyi entropy of the array.
	float_ty ment;			//The minimum entropy of the array.
	float_ty stddev;		//The standard deviation of the array.
	float_ty pgfa;			//The probability generating function for x~P G(z) = \sum_{x=0}^{\infinity} [p_i * z^x], z=0.95.
	float_ty pgfb;			//The probability generating function for x~P G(z) = \sum_{x=0}^{\infinity} [p_i * z^x], z=0,90.
	float_ty pgfc;			//The probability generating function for x~P G(z) = \sum_{x=0}^{\infinity} [p_i * z^x], z=0,85.
} formulaAttributeSet;

#define MAIN_RESET_FATT_FAS( __fas ) \
	__fas->min=ZERO; \
	__fas->max=ZERO; \
	__fas->mean=ZERO; \
	__fas->median=ZERO; \
	__fas->sent=ZERO; \
	__fas->rent=ZERO; \
	__fas->ment=ZERO; \
	__fas->stddev=ZERO; \
	__fas->pgfa=ZERO; \
	__fas->pgfb=ZERO; \
	__fas->pgfc=ZERO;

#define MAIN_PRINT_FATT_FASN( __fasn ) \
		printf("%s_MIN ", __fasn); \
		printf("%s_MAX ", __fasn); \
		printf("%s_MEAN ", __fasn); \
		printf("%s_MEDIAN ", __fasn); \
		printf("%s_SENT ", __fasn); \
		printf("%s_RENT ", __fasn); \
		printf("%s_MENT ", __fasn); \
		printf("%s_STDDEV ", __fasn); \
		printf("%s_PGFA ", __fasn); \
		printf("%s_PGFB ", __fasn); \
		printf("%s_PGFC ", __fasn);

#define MAIN_PRINT_FATT_FAS( __fas ) \
		printf("%f ", __fas->min); \
		printf("%f ", __fas->max); \
		printf("%f ", __fas->mean); \
		printf("%f ", __fas->median); \
		printf("%f ", __fas->sent); \
		printf("%f ", __fas->rent); \
		printf("%f ", __fas->ment); \
		printf("%f ", __fas->stddev); \
		printf("%f ", __fas->pgfa); \
		printf("%f ", __fas->pgfb); \
		printf("%f ", __fas->pgfc); \

typedef struct {							//The structure of the formula attributes computed for the original formula.
	uint32_t isSet;							//Tells us if the attributes have been computed.
	uint32_t class;							//Tells us what the class of the instance is after it has been classified.
	float_ty certainty;						//How certain the formula belongs to the class (a value in 0.0 to 1.0).
	float_ty certaintyDist;					//How much certainty there is between the winning class and the second (distance).
	//PROBLEM SIZE ATTRIBUTES
	float_ty n;								//Number of variables.
	float_ty m;								//Number of clauses.
	float_ty r;								//Ratio of the formula.
	float_ty initAss;						//How many assignments resulted from initial unit clauses?
	float_ty initDis;						//How many unassigned variables have been disabled initially?
	float_ty initPurePos;					//How many enabled variables appear purely in the positive direction?
	float_ty initPureNeg;					//How many enabled variables appear purely in the negative direction?
	//VARIABLE-CLAUSE GRAPH ATTRIBUTES
	formulaAttributeSet* vcg_vNodeDegree;	//Variable-Clause Graph variable node statistics.
	formulaAttributeSet* vcg_cNodeDegree;	//Variable-Clause Graph clause node statistics.
	float_ty vcg_maybeRand;					//Determines, if the formula may be a random formula via a few simple checks.
	//VARIABLE GRAPH ATTRIBUTES
	formulaAttributeSet* vg_estAvgNDeg;		//An estimated average node degree for all variables.
	formulaAttributeSet* vg_vNodeDegree;	//Variable Graph variable node degrees (exact values).
	float_ty vg_numComp;					//The number of components of the variable graph.
	formulaAttributeSet* vg_vCompSizes;		//The component size information (number of variables in a component).
	formulaAttributeSet* vg_vCompDiams;		//The component diameter information (diameter for each component).
	float_ty vg_numAP;						//The number of articulation points in the variable graph.
	//CLAUSE GRAPH ATTRIBUTES
	formulaAttributeSet* cg_cNodeDegree;	//Clause Graph variable node statistics.
	formulaAttributeSet* cg_avgDegPerLit;	//The average degree per literal of a clause in the clause graph.
	//BALANCE ATTRIBUTES
	formulaAttributeSet* b_vLitBalance;		//The ratio of positive to negative literal occurrences for each variable.
	formulaAttributeSet* b_cLitBalance;		//The ratio of positive to negative literals in the clauses.
	float_ty b_cSize2;						//The fraction of clauses of size 2.
	float_ty b_cSize3;						//The fraction of clauses of size 3.
	float_ty b_cSize4;						//The fraction of clauses of size 4.
	float_ty b_cSize5;						//The fraction of clauses of size 5.
	float_ty b_cSize6;						//The fraction of clauses of size 6.
	float_ty b_cSize7;						//The fraction of clauses of size 7.
	//HORN PROXIMITY
	float_ty hp_hornFrac;					//The fraction of Horn clauses.
	formulaAttributeSet* hp_vHornFrac;		//Fraction of number of occurrences in Horn clauses for each variable.
	formulaAttributeSet* hp_cHornDist;		//The distance of clauses to being a Horn clause (at most one positive literal).
	//BIG ATTRIBUTES
	formulaAttributeSet* big_vNodeDegree;	//Binary implication graph variable node statistics.
	float_ty big_vFrac;						//The fraction of all variables that are found in the BIG.
	float_ty big_numComp;					//Number of components in the binary implication graph.
	formulaAttributeSet* big_vCompSizes;	//The component size information (number of variables in a component).
	formulaAttributeSet* big_vCompDiams;	//The component diameter information (diameter for each component).
	float_ty big_numAP;						//The number of articulation points in the binary implication graph.
	//UP ATTRIBUTES
	float_ty up_posLitConfFrac;				//The fraction of positive literals on which the UP ran into conflicts.
	float_ty up_negLitConfFrac;				//The fraction of negative literals on which the UP ran into conflicts.
	formulaAttributeSet* up_posLitUPCount;	//Number of unit propagations that follow when doing UP on a positive literal.
	formulaAttributeSet* up_negLitUPCount;	//Number of unit propagations that follow when doing UP on a negative literal.
	//RW Attributes
	float_ty rw_q05;						//The the percent of values necessary to achieve 05% of the total mass.
	float_ty rw_q10;						//The the percent of values necessary to achieve 10% of the total mass.
	float_ty rw_q25;						//The the percent of values necessary to achieve 25% of the total mass.
	float_ty rw_q50;						//The the percent of values necessary to achieve 50% of the total mass.
	float_ty rw_q75;						//The the percent of values necessary to achieve 75% of the total mass.
	float_ty rw_q90;						//The the percent of values necessary to achieve 90% of the total mass.
	float_ty rw_q95;						//The the percent of values necessary to achieve 95% of the total mass.
	formulaAttributeSet* rw_vWeights;		//The variable weight of the recursive weight heuristic of Mjinders et al.
	//ELS Attributes
	float_ty els_numC;						//The number of equivalent literal classes.
	formulaAttributeSet * els_sizes;		//The equivalent literal class size attributes.
} formulaAttributes;

//The following macros are used to access the current formula attributes.
//PROBLEM SIZE
#define MAIN_GET_FATT_ISSET() 							( f.att->isSet )
#define MAIN_SET_FATT_ISSET( __d ) 						( f.att->isSet = __d )

#define MAIN_GET_FATT_CLASS()							( f.att->class )
#define MAIN_SET_FATT_CLASS( __d )						( f.att->class = __d )

#define MAIN_GET_FATT_CERTAINTY()						( f.att->certainty )
#define MAIN_SET_FATT_CERTAINTY( __d )					( f.att->certainty = __d )

#define MAIN_GET_FATT_CERTAINTYDIST()					( f.att->certaintyDist )
#define MAIN_SET_FATT_CERTAINTYDIST( __d )				( f.att->certaintyDist = __d )

#define MAIN_GET_FATT_N()								( f.att->n )
#define MAIN_SET_FATT_N( __d )							( f.att->n = __d )

#define MAIN_GET_FATT_M()								( f.att->m )
#define MAIN_SET_FATT_M( __d )							( f.att->m = __d )

#define MAIN_GET_FATT_R()								( f.att->r )
#define MAIN_SET_FATT_R( __d )							( f.att->r = __d )

#define MAIN_GET_FATT_INITASS()							( f.att->initAss )
#define MAIN_SET_FATT_INITASS( __d )					( f.att->initAss = __d )

#define MAIN_GET_FATT_INITDIS()							( f.att->initDis )
#define MAIN_SET_FATT_INITDIS( __d )					( f.att->initDis = __d )

#define MAIN_GET_FATT_INITPUREPOS()						( f.att->initPurePos )
#define MAIN_SET_FATT_INITPUREPOS( __d )				( f.att->initPurePos = __d )

#define MAIN_GET_FATT_INITPURENEG()						( f.att->initPureNeg )
#define MAIN_SET_FATT_INITPURENEG( __d )				( f.att->initPureNeg = __d )

//VARIABLE CLAUSE GRAPH
#define MAIN_GET_FATT_VCG_VNODEDEGREE() 				( f.att->vcg_vNodeDegree )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_MIN()				( f.att->vcg_vNodeDegree->min )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_MAX()				( f.att->vcg_vNodeDegree->max )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_MEAN()			( f.att->vcg_vNodeDegree->mean )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_MEDIAN()			( f.att->vcg_vNodeDegree->median )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_SENT()			( f.att->vcg_vNodeDegree->sent )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_RENT()			( f.att->vcg_vNodeDegree->rent )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_MENT()			( f.att->vcg_vNodeDegree->ment )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_STDDEV()			( f.att->vcg_vNodeDegree->stddev )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_PGFA()			( f.att->vcg_vNodeDegree->pgfa )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_PGFB()			( f.att->vcg_vNodeDegree->pgfb )
#define MAIN_GET_FATT_VCG_VNODEDEGREE_PGFC()			( f.att->vcg_vNodeDegree->pgfc )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_MIN( __d )		( f.att->vcg_vNodeDegree->min = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_MAX( __d )		( f.att->vcg_vNodeDegree->max = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_MEAN( __d )		( f.att->vcg_vNodeDegree->mean = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_MEDIAN( __d )		( f.att->vcg_vNodeDegree->median = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_SENT( __d )		( f.att->vcg_vNodeDegree->sent = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_RENT( __d )		( f.att->vcg_vNodeDegree->rent = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_MENT( __d )		( f.att->vcg_vNodeDegree->ment = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_STDDEV( __d )		( f.att->vcg_vNodeDegree->stddev = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_PGFA( __d )		( f.att->vcg_vNodeDegree->pgfa = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_PGFB( __d )		( f.att->vcg_vNodeDegree->pgfb = __d )
#define MAIN_SET_FATT_VCG_VNODEDEGREE_PGFC( __d )		( f.att->vcg_vNodeDegree->pgfc = __d )

#define MAIN_GET_FATT_VCG_CNODEDEGREE() 				( f.att->vcg_cNodeDegree )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_MIN()				( f.att->vcg_cNodeDegree->min )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_MAX()				( f.att->vcg_cNodeDegree->max )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_MEAN()			( f.att->vcg_cNodeDegree->mean )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_MEDIAN()			( f.att->vcg_cNodeDegree->median )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_SENT()			( f.att->vcg_cNodeDegree->sent )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_RENT()			( f.att->vcg_cNodeDegree->rent )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_MENT()			( f.att->vcg_cNodeDegree->ment )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_STDDEV()			( f.att->vcg_cNodeDegree->stddev )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_PGFA()			( f.att->vcg_cNodeDegree->pgfa )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_PGFB()			( f.att->vcg_cNodeDegree->pgfb )
#define MAIN_GET_FATT_VCG_CNODEDEGREE_PGFC()			( f.att->vcg_cNodeDegree->pgfc )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_MIN( __d )		( f.att->vcg_cNodeDegree->min = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_MAX( __d )		( f.att->vcg_cNodeDegree->max = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_MEAN( __d )		( f.att->vcg_cNodeDegree->mean = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_MEDIAN( __d )		( f.att->vcg_cNodeDegree->median = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_SENT( __d )		( f.att->vcg_cNodeDegree->sent = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_RENT( __d )		( f.att->vcg_cNodeDegree->rent = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_MENT( __d )		( f.att->vcg_cNodeDegree->ment = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_STDDEV( __d )		( f.att->vcg_cNodeDegree->stddev = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_PGFA( __d )		( f.att->vcg_cNodeDegree->pgfa = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_PGFB( __d )		( f.att->vcg_cNodeDegree->pgfb = __d )
#define MAIN_SET_FATT_VCG_CNODEDEGREE_PGFC( __d )		( f.att->vcg_cNodeDegree->pgfc = __d )

#define MAIN_GET_FATT_VCG_MAYBERAND() 					( f.att->vcg_maybeRand )
#define MAIN_SET_FATT_VCG_MAYBERAND( __d ) 				( f.att->vcg_maybeRand = __d )

//VARIABLE GRAPH
#define MAIN_GET_FATT_VG_ESTAVGNDEG() 					( f.att->vg_estAvgNDeg )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_MIN()				( f.att->vg_estAvgNDeg->min )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_MAX()				( f.att->vg_estAvgNDeg->max )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_MEAN()				( f.att->vg_estAvgNDeg->mean )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_MEDIAN()			( f.att->vg_estAvgNDeg->median )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_SENT()				( f.att->vg_estAvgNDeg->sent )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_RENT()				( f.att->vg_estAvgNDeg->rent )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_MENT()				( f.att->vg_estAvgNDeg->ment )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_STDDEV()			( f.att->vg_estAvgNDeg->stddev )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_PGFA()				( f.att->vg_estAvgNDeg->pgfa )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_PGFB()				( f.att->vg_estAvgNDeg->pgfb )
#define MAIN_GET_FATT_VG_ESTAVGNDEG_PGFC()				( f.att->vg_estAvgNDeg->pgfc )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_MIN( __d )			( f.att->vg_estAvgNDeg->min = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_MAX( __d )			( f.att->vg_estAvgNDeg->max = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_MEAN( __d )			( f.att->vg_estAvgNDeg->mean = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_MEDIAN( __d )		( f.att->vg_estAvgNDeg->median = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_SENT( __d )			( f.att->vg_estAvgNDeg->sent = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_RENT( __d )			( f.att->vg_estAvgNDeg->rent = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_MENT( __d )			( f.att->vg_estAvgNDeg->ment = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_STDDEV( __d )		( f.att->vg_estAvgNDeg->stddev = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_PGFA( __d )			( f.att->vg_estAvgNDeg->pgfa = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_PGFB( __d )			( f.att->vg_estAvgNDeg->pgfb = __d )
#define MAIN_SET_FATT_VG_ESTAVGNDEG_PGFC( __d )			( f.att->vg_estAvgNDeg->pgfc = __d )

#define MAIN_GET_FATT_VG_VNODEDEGREE() 					( f.att->vg_vNodeDegree )
#define MAIN_GET_FATT_VG_VNODEDEGREE_MIN()				( f.att->vg_vNodeDegree->min )
#define MAIN_GET_FATT_VG_VNODEDEGREE_MAX()				( f.att->vg_vNodeDegree->max )
#define MAIN_GET_FATT_VG_VNODEDEGREE_MEAN()				( f.att->vg_vNodeDegree->mean )
#define MAIN_GET_FATT_VG_VNODEDEGREE_MEDIAN()			( f.att->vg_vNodeDegree->median )
#define MAIN_GET_FATT_VG_VNODEDEGREE_SENT()				( f.att->vg_vNodeDegree->sent )
#define MAIN_GET_FATT_VG_VNODEDEGREE_RENT()				( f.att->vg_vNodeDegree->rent )
#define MAIN_GET_FATT_VG_VNODEDEGREE_MENT()				( f.att->vg_vNodeDegree->ment )
#define MAIN_GET_FATT_VG_VNODEDEGREE_STDDEV()			( f.att->vg_vNodeDegree->stddev )
#define MAIN_GET_FATT_VG_VNODEDEGREE_PGFA()				( f.att->vg_vNodeDegree->pgfa )
#define MAIN_GET_FATT_VG_VNODEDEGREE_PGFB()				( f.att->vg_vNodeDegree->pgfb )
#define MAIN_GET_FATT_VG_VNODEDEGREE_PGFC()				( f.att->vg_vNodeDegree->pgfc )
#define MAIN_SET_FATT_VG_VNODEDEGREE_MIN( __d )			( f.att->vg_vNodeDegree->min = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_MAX( __d )			( f.att->vg_vNodeDegree->max = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_MEAN( __d )		( f.att->vg_vNodeDegree->mean = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_MEDIAN( __d )		( f.att->vg_vNodeDegree->median = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_SENT( __d )		( f.att->vg_vNodeDegree->sent = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_RENT( __d )		( f.att->vg_vNodeDegree->rent = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_MENT( __d )		( f.att->vg_vNodeDegree->ment = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_STDDEV( __d )		( f.att->vg_vNodeDegree->stddev = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_PGFA( __d )		( f.att->vg_vNodeDegree->pgfa = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_PGFB( __d )		( f.att->vg_vNodeDegree->pgfb = __d )
#define MAIN_SET_FATT_VG_VNODEDEGREE_PGFC( __d )		( f.att->vg_vNodeDegree->pgfc = __d )

#define MAIN_GET_FATT_VG_NUMCOMP()						( f.att->vg_numComp )
#define MAIN_SET_FATT_VG_NUMCOMP( __d )					( f.att->vg_numComp = __d )

#define MAIN_GET_FATT_VG_VCOMPSIZES() 					( f.att->vg_vCompSizes )
#define MAIN_GET_FATT_VG_VCOMPSIZES_MIN()				( f.att->vg_vCompSizes->min )
#define MAIN_GET_FATT_VG_VCOMPSIZES_MAX()				( f.att->vg_vCompSizes->max )
#define MAIN_GET_FATT_VG_VCOMPSIZES_MEAN()				( f.att->vg_vCompSizes->mean )
#define MAIN_GET_FATT_VG_VCOMPSIZES_MEDIAN()			( f.att->vg_vCompSizes->median )
#define MAIN_GET_FATT_VG_VCOMPSIZES_SENT()				( f.att->vg_vCompSizes->sent )
#define MAIN_GET_FATT_VG_VCOMPSIZES_RENT()				( f.att->vg_vCompSizes->rent )
#define MAIN_GET_FATT_VG_VCOMPSIZES_MENT()				( f.att->vg_vCompSizes->ment )
#define MAIN_GET_FATT_VG_VCOMPSIZES_STDDEV()			( f.att->vg_vCompSizes->stddev )
#define MAIN_GET_FATT_VG_VCOMPSIZES_PGFA()				( f.att->vg_vCompSizes->pgfa )
#define MAIN_GET_FATT_VG_VCOMPSIZES_PGFB()				( f.att->vg_vCompSizes->pgfb )
#define MAIN_GET_FATT_VG_VCOMPSIZES_PGFC()				( f.att->vg_vCompSizes->pgfc )
#define MAIN_SET_FATT_VG_VCOMPSIZES_MIN( __d )			( f.att->vg_vCompSizes->min = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_MAX( __d )			( f.att->vg_vCompSizes->max = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_MEAN( __d )			( f.att->vg_vCompSizes->mean = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_MEDIAN( __d )		( f.att->vg_vCompSizes->median = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_SENT( __d )			( f.att->vg_vCompSizes->sent = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_RENT( __d )			( f.att->vg_vCompSizes->rent = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_MENT( __d )			( f.att->vg_vCompSizes->ment = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_STDDEV( __d )		( f.att->vg_vCompSizes->stddev = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_PGFA( __d )			( f.att->vg_vCompSizes->pgfa = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_PGFB( __d )			( f.att->vg_vCompSizes->pgfb = __d )
#define MAIN_SET_FATT_VG_VCOMPSIZES_PGFC( __d )			( f.att->vg_vCompSizes->pgfc = __d )

#define MAIN_GET_FATT_VG_VCOMPDIAMS() 					( f.att->vg_vCompDiams )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_MIN()				( f.att->vg_vCompDiams->min )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_MAX()				( f.att->vg_vCompDiams->max )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_MEAN()				( f.att->vg_vCompDiams->mean )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_MEDIAN()			( f.att->vg_vCompDiams->median )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_SENT()				( f.att->vg_vCompDiams->sent )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_RENT()				( f.att->vg_vCompDiams->rent )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_MENT()				( f.att->vg_vCompDiams->ment )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_STDDEV()			( f.att->vg_vCompDiams->stddev )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_PGFA()				( f.att->vg_vCompDiams->pgfa )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_PGFB()				( f.att->vg_vCompDiams->pgfb )
#define MAIN_GET_FATT_VG_VCOMPDIAMS_PGFC()				( f.att->vg_vCompDiams->pgfc )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_MIN( __d )			( f.att->vg_vCompDiams->min = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_MAX( __d )			( f.att->vg_vCompDiams->max = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_MEAN( __d )			( f.att->vg_vCompDiams->mean = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_MEDIAN( __d )		( f.att->vg_vCompDiams->median = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_SENT( __d )			( f.att->vg_vCompDiams->sent = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_RENT( __d )			( f.att->vg_vCompDiams->rent = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_MENT( __d )			( f.att->vg_vCompDiams->ment = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_STDDEV( __d )		( f.att->vg_vCompDiams->stddev = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_PGFA( __d )			( f.att->vg_vCompDiams->pgfa = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_PGFB( __d )			( f.att->vg_vCompDiams->pgfb = __d )
#define MAIN_SET_FATT_VG_VCOMPDIAMS_PGFC( __d )			( f.att->vg_vCompDiams->pgfc = __d )

#define MAIN_GET_FATT_VG_NUMAP()						( f.att->vg_numAP )
#define MAIN_SET_FATT_VG_NUMAP( __d )					( f.att->vg_numAP = __d )

//CLAUSE GRAPH
#define MAIN_GET_FATT_CG_CNODEDEGREE() 					( f.att->cg_cNodeDegree )
#define MAIN_GET_FATT_CG_CNODEDEGREE_MIN()				( f.att->cg_cNodeDegree->min )
#define MAIN_GET_FATT_CG_CNODEDEGREE_MAX()				( f.att->cg_cNodeDegree->max )
#define MAIN_GET_FATT_CG_CNODEDEGREE_MEAN()				( f.att->cg_cNodeDegree->mean )
#define MAIN_GET_FATT_CG_CNODEDEGREE_MEDIAN()			( f.att->cg_cNodeDegree->median )
#define MAIN_GET_FATT_CG_CNODEDEGREE_SENT()				( f.att->cg_cNodeDegree->sent )
#define MAIN_GET_FATT_CG_CNODEDEGREE_RENT()				( f.att->cg_cNodeDegree->rent )
#define MAIN_GET_FATT_CG_CNODEDEGREE_MENT()				( f.att->cg_cNodeDegree->ment )
#define MAIN_GET_FATT_CG_CNODEDEGREE_STDDEV()			( f.att->cg_cNodeDegree->stddev )
#define MAIN_GET_FATT_CG_CNODEDEGREE_PGFA()				( f.att->cg_cNodeDegree->pgfa )
#define MAIN_GET_FATT_CG_CNODEDEGREE_PGFB()				( f.att->cg_cNodeDegree->pgfb )
#define MAIN_GET_FATT_CG_CNODEDEGREE_PGFC()				( f.att->cg_cNodeDegree->pgfc )
#define MAIN_SET_FATT_CG_CNODEDEGREE_MIN( __d )			( f.att->cg_cNodeDegree->min = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_MAX( __d )			( f.att->cg_cNodeDegree->max = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_MEAN( __d )		( f.att->cg_cNodeDegree->mean = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_MEDIAN( __d )		( f.att->cg_cNodeDegree->median = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_SENT( __d )		( f.att->cg_cNodeDegree->sent = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_RENT( __d )		( f.att->cg_cNodeDegree->rent = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_MENT( __d )		( f.att->cg_cNodeDegree->ment = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_STDDEV( __d )		( f.att->cg_cNodeDegree->stddev = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_PGFA( __d )		( f.att->cg_cNodeDegree->pgfa = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_PGFB( __d )		( f.att->cg_cNodeDegree->pgfb = __d )
#define MAIN_SET_FATT_CG_CNODEDEGREE_PGFC( __d )		( f.att->cg_cNodeDegree->pgfc = __d )

#define MAIN_GET_FATT_CG_AVGDEGPERLIT() 				( f.att->cg_avgDegPerLit )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_MIN()				( f.att->cg_avgDegPerLit->min )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_MAX()				( f.att->cg_avgDegPerLit->max )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_MEAN()			( f.att->cg_avgDegPerLit->mean )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_MEDIAN()			( f.att->cg_avgDegPerLit->median )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_SENT()			( f.att->cg_avgDegPerLit->sent )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_RENT()			( f.att->cg_avgDegPerLit->rent )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_MENT()			( f.att->cg_avgDegPerLit->ment )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_STDDEV()			( f.att->cg_avgDegPerLit->stddev )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFA()			( f.att->cg_avgDegPerLit->pgfa )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFB()			( f.att->cg_avgDegPerLit->pgfb )
#define MAIN_GET_FATT_CG_AVGDEGPERLIT_PGFC()			( f.att->cg_avgDegPerLit->pgfc )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_MIN( __d )		( f.att->cg_avgDegPerLit->min = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_MAX( __d )		( f.att->cg_avgDegPerLit->max = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_MEAN( __d )		( f.att->cg_avgDegPerLit->mean = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_MEDIAN( __d )		( f.att->cg_avgDegPerLit->median = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_SENT( __d )		( f.att->cg_avgDegPerLit->sent = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_RENT( __d )		( f.att->cg_avgDegPerLit->rent = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_MENT( __d )		( f.att->cg_avgDegPerLit->ment = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_STDDEV( __d )		( f.att->cg_avgDegPerLit->stddev = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFA( __d )		( f.att->cg_avgDegPerLit->pgfa = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFB( __d )		( f.att->cg_avgDegPerLit->pgfb = __d )
#define MAIN_SET_FATT_CG_AVGDEGPERLIT_PGFC( __d )		( f.att->cg_avgDegPerLit->pgfc = __d )

//BALANCE
#define MAIN_GET_FATT_B_VLITBALANCE() 					( f.att->b_vLitBalance )
#define MAIN_GET_FATT_B_VLITBALANCE_MIN()				( f.att->b_vLitBalance->min )
#define MAIN_GET_FATT_B_VLITBALANCE_MAX()				( f.att->b_vLitBalance->max )
#define MAIN_GET_FATT_B_VLITBALANCE_MEAN()				( f.att->b_vLitBalance->mean )
#define MAIN_GET_FATT_B_VLITBALANCE_MEDIAN()			( f.att->b_vLitBalance->median )
#define MAIN_GET_FATT_B_VLITBALANCE_SENT()				( f.att->b_vLitBalance->sent )
#define MAIN_GET_FATT_B_VLITBALANCE_RENT()				( f.att->b_vLitBalance->rent )
#define MAIN_GET_FATT_B_VLITBALANCE_MENT()				( f.att->b_vLitBalance->ment )
#define MAIN_GET_FATT_B_VLITBALANCE_STDDEV()			( f.att->b_vLitBalance->stddev )
#define MAIN_GET_FATT_B_VLITBALANCE_PGFA()				( f.att->b_vLitBalance->pgfa )
#define MAIN_GET_FATT_B_VLITBALANCE_PGFB()				( f.att->b_vLitBalance->pgfb )
#define MAIN_GET_FATT_B_VLITBALANCE_PGFC()				( f.att->b_vLitBalance->pgfc )
#define MAIN_SET_FATT_B_VLITBALANCE_MIN( __d )			( f.att->b_vLitBalance->min = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_MAX( __d )			( f.att->b_vLitBalance->max = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_MEAN( __d )			( f.att->b_vLitBalance->mean = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_MEDIAN( __d )		( f.att->b_vLitBalance->median = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_SENT( __d )			( f.att->b_vLitBalance->sent = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_RENT( __d )			( f.att->b_vLitBalance->rent = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_MENT( __d )			( f.att->b_vLitBalance->ment = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_STDDEV( __d )		( f.att->b_vLitBalance->stddev = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_PGFA( __d )			( f.att->b_vLitBalance->pgfa = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_PGFB( __d )			( f.att->b_vLitBalance->pgfb = __d )
#define MAIN_SET_FATT_B_VLITBALANCE_PGFC( __d )			( f.att->b_vLitBalance->pgfc = __d )

#define MAIN_GET_FATT_B_CLITBALANCE() 					( f.att->b_cLitBalance )
#define MAIN_GET_FATT_B_CLITBALANCE_MIN()				( f.att->b_cLitBalance->min )
#define MAIN_GET_FATT_B_CLITBALANCE_MAX()				( f.att->b_cLitBalance->max )
#define MAIN_GET_FATT_B_CLITBALANCE_MEAN()				( f.att->b_cLitBalance->mean )
#define MAIN_GET_FATT_B_CLITBALANCE_MEDIAN()			( f.att->b_cLitBalance->median )
#define MAIN_GET_FATT_B_CLITBALANCE_SENT()				( f.att->b_cLitBalance->sent )
#define MAIN_GET_FATT_B_CLITBALANCE_RENT()				( f.att->b_cLitBalance->rent )
#define MAIN_GET_FATT_B_CLITBALANCE_MENT()				( f.att->b_cLitBalance->ment )
#define MAIN_GET_FATT_B_CLITBALANCE_STDDEV()			( f.att->b_cLitBalance->stddev )
#define MAIN_GET_FATT_B_CLITBALANCE_PGFA()				( f.att->b_cLitBalance->pgfa )
#define MAIN_GET_FATT_B_CLITBALANCE_PGFB()				( f.att->b_cLitBalance->pgfb )
#define MAIN_GET_FATT_B_CLITBALANCE_PGFC()				( f.att->b_cLitBalance->pgfc )
#define MAIN_SET_FATT_B_CLITBALANCE_MIN( __d )			( f.att->b_cLitBalance->min = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_MAX( __d )			( f.att->b_cLitBalance->max = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_MEAN( __d )			( f.att->b_cLitBalance->mean = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_MEDIAN( __d )		( f.att->b_cLitBalance->median = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_SENT( __d )			( f.att->b_cLitBalance->sent = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_RENT( __d )			( f.att->b_cLitBalance->rent = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_MENT( __d )			( f.att->b_cLitBalance->ment = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_STDDEV( __d )		( f.att->b_cLitBalance->stddev = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_PGFA( __d )			( f.att->b_cLitBalance->pgfa = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_PGFB( __d )			( f.att->b_cLitBalance->pgfb = __d )
#define MAIN_SET_FATT_B_CLITBALANCE_PGFC( __d )			( f.att->b_cLitBalance->pgfc = __d )

#define MAIN_GET_FATT_B_CSIZE2()						( f.att->b_cSize2 )
#define MAIN_SET_FATT_B_CSIZE2( __d )					( f.att->b_cSize2 = __d )

#define MAIN_GET_FATT_B_CSIZE3()						( f.att->b_cSize3 )
#define MAIN_SET_FATT_B_CSIZE3( __d )					( f.att->b_cSize3 = __d )

#define MAIN_GET_FATT_B_CSIZE4()						( f.att->b_cSize4 )
#define MAIN_SET_FATT_B_CSIZE4( __d )					( f.att->b_cSize4 = __d )

#define MAIN_GET_FATT_B_CSIZE5()						( f.att->b_cSize5 )
#define MAIN_SET_FATT_B_CSIZE5( __d )					( f.att->b_cSize5 = __d )

#define MAIN_GET_FATT_B_CSIZE6()						( f.att->b_cSize6 )
#define MAIN_SET_FATT_B_CSIZE6( __d )					( f.att->b_cSize6 = __d )

#define MAIN_GET_FATT_B_CSIZE7()						( f.att->b_cSize7 )
#define MAIN_SET_FATT_B_CSIZE7( __d )					( f.att->b_cSize7 = __d )

//HORN PROXIMITY
#define MAIN_GET_FATT_HP_HORNFRAC()						( f.att->hp_hornFrac )
#define MAIN_SET_FATT_HP_HORNFRAC( __d )				( f.att->hp_hornFrac = __d )

#define MAIN_GET_FATT_HP_VHORNFRAC() 					( f.att->hp_vHornFrac )
#define MAIN_GET_FATT_HP_VHORNFRAC_MIN()				( f.att->hp_vHornFrac->min )
#define MAIN_GET_FATT_HP_VHORNFRAC_MAX()				( f.att->hp_vHornFrac->max )
#define MAIN_GET_FATT_HP_VHORNFRAC_MEAN()				( f.att->hp_vHornFrac->mean )
#define MAIN_GET_FATT_HP_VHORNFRAC_MEDIAN()				( f.att->hp_vHornFrac->median )
#define MAIN_GET_FATT_HP_VHORNFRAC_SENT()				( f.att->hp_vHornFrac->sent )
#define MAIN_GET_FATT_HP_VHORNFRAC_RENT()				( f.att->hp_vHornFrac->rent )
#define MAIN_GET_FATT_HP_VHORNFRAC_MENT()				( f.att->hp_vHornFrac->ment )
#define MAIN_GET_FATT_HP_VHORNFRAC_STDDEV()				( f.att->hp_vHornFrac->stddev )
#define MAIN_GET_FATT_HP_VHORNFRAC_PGFA()				( f.att->hp_vHornFrac->pgfa )
#define MAIN_GET_FATT_HP_VHORNFRAC_PGFB()				( f.att->hp_vHornFrac->pgfb )
#define MAIN_GET_FATT_HP_VHORNFRAC_PGFC()				( f.att->hp_vHornFrac->pgfc )
#define MAIN_SET_FATT_HP_VHORNFRAC_MIN( __d )			( f.att->hp_vHornFrac->min = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_MAX( __d )			( f.att->hp_vHornFrac->max = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_MEAN( __d )			( f.att->hp_vHornFrac->mean = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_MEDIAN( __d )		( f.att->hp_vHornFrac->median = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_SENT( __d )			( f.att->hp_vHornFrac->sent = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_RENT( __d )			( f.att->hp_vHornFrac->rent = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_MENT( __d )			( f.att->hp_vHornFrac->ment = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_STDDEV( __d )		( f.att->hp_vHornFrac->stddev = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_PGFA( __d )			( f.att->hp_vHornFrac->pgfa = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_PGFB( __d )			( f.att->hp_vHornFrac->pgfb = __d )
#define MAIN_SET_FATT_HP_VHORNFRAC_PGFC( __d )			( f.att->hp_vHornFrac->pgfc = __d )

#define MAIN_GET_FATT_HP_CHORNDIST() 					( f.att->hp_cHornDist )
#define MAIN_GET_FATT_HP_CHORNDIST_MIN()				( f.att->hp_cHornDist->min )
#define MAIN_GET_FATT_HP_CHORNDIST_MAX()				( f.att->hp_cHornDist->max )
#define MAIN_GET_FATT_HP_CHORNDIST_MEAN()				( f.att->hp_cHornDist->mean )
#define MAIN_GET_FATT_HP_CHORNDIST_MEDIAN()				( f.att->hp_cHornDist->median )
#define MAIN_GET_FATT_HP_CHORNDIST_SENT()				( f.att->hp_cHornDist->sent )
#define MAIN_GET_FATT_HP_CHORNDIST_RENT()				( f.att->hp_cHornDist->rent )
#define MAIN_GET_FATT_HP_CHORNDIST_MENT()				( f.att->hp_cHornDist->ment )
#define MAIN_GET_FATT_HP_CHORNDIST_STDDEV()				( f.att->hp_cHornDist->stddev )
#define MAIN_GET_FATT_HP_CHORNDIST_PGFA()				( f.att->hp_cHornDist->pgfa )
#define MAIN_GET_FATT_HP_CHORNDIST_PGFB()				( f.att->hp_cHornDist->pgfb )
#define MAIN_GET_FATT_HP_CHORNDIST_PGFC()				( f.att->hp_cHornDist->pgfc )
#define MAIN_SET_FATT_HP_CHORNDIST_MIN( __d )			( f.att->hp_cHornDist->min = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_MAX( __d )			( f.att->hp_cHornDist->max = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_MEAN( __d )			( f.att->hp_cHornDist->mean = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_MEDIAN( __d )		( f.att->hp_cHornDist->median = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_SENT( __d )			( f.att->hp_cHornDist->sent = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_RENT( __d )			( f.att->hp_cHornDist->rent = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_MENT( __d )			( f.att->hp_cHornDist->ment = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_STDDEV( __d )		( f.att->hp_cHornDist->stddev = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_PGFA( __d )			( f.att->hp_cHornDist->pgfa = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_PGFB( __d )			( f.att->hp_cHornDist->pgfb = __d )
#define MAIN_SET_FATT_HP_CHORNDIST_PGFC( __d )			( f.att->hp_cHornDist->pgfc = __d )

//BINARY IMPLICATION GRAPH
#define MAIN_GET_FATT_BIG_VNODEDEGREE() 				( f.att->big_vNodeDegree )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_MIN()				( f.att->big_vNodeDegree->min )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_MAX()				( f.att->big_vNodeDegree->max )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_MEAN()			( f.att->big_vNodeDegree->mean )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_MEDIAN()			( f.att->big_vNodeDegree->median )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_SENT()			( f.att->big_vNodeDegree->sent )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_RENT()			( f.att->big_vNodeDegree->rent )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_MENT()			( f.att->big_vNodeDegree->ment )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_STDDEV()			( f.att->big_vNodeDegree->stddev )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_PGFA()			( f.att->big_vNodeDegree->pgfa )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_PGFB()			( f.att->big_vNodeDegree->pgfb )
#define MAIN_GET_FATT_BIG_VNODEDEGREE_PGFC()			( f.att->big_vNodeDegree->pgfc )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_MIN( __d )		( f.att->big_vNodeDegree->min = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_MAX( __d )		( f.att->big_vNodeDegree->max = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_MEAN( __d )		( f.att->big_vNodeDegree->mean = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_MEDIAN( __d )		( f.att->big_vNodeDegree->median = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_SENT( __d )		( f.att->big_vNodeDegree->sent = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_RENT( __d )		( f.att->big_vNodeDegree->rent = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_MENT( __d )		( f.att->big_vNodeDegree->ment = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_STDDEV( __d )		( f.att->big_vNodeDegree->stddev = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_PGFA( __d )		( f.att->big_vNodeDegree->pgfa = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_PGFB( __d )		( f.att->big_vNodeDegree->pgfb = __d )
#define MAIN_SET_FATT_BIG_VNODEDEGREE_PGFC( __d )		( f.att->big_vNodeDegree->pgfc = __d )

#define MAIN_GET_FATT_BIG_VFRAC()						( f.att->big_vFrac )
#define MAIN_SET_FATT_BIG_VFRAC( __d )					( f.att->big_vFrac = __d )

#define MAIN_GET_FATT_BIG_NUMCOMP()						( f.att->big_numComp )
#define MAIN_SET_FATT_BIG_NUMCOMP( __d )				( f.att->big_numComp = __d )

#define MAIN_GET_FATT_BIG_VCOMPSIZES() 					( f.att->big_vCompSizes )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_MIN()				( f.att->big_vCompSizes->min )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_MAX()				( f.att->big_vCompSizes->max )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_MEAN()				( f.att->big_vCompSizes->mean )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_MEDIAN()			( f.att->big_vCompSizes->median )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_SENT()				( f.att->big_vCompSizes->sent )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_RENT()				( f.att->big_vCompSizes->rent )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_MENT()				( f.att->big_vCompSizes->ment )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_STDDEV()			( f.att->big_vCompSizes->stddev )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_PGFA()				( f.att->big_vCompSizes->pgfa )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_PGFB()				( f.att->big_vCompSizes->pgfb )
#define MAIN_GET_FATT_BIG_VCOMPSIZES_PGFC()				( f.att->big_vCompSizes->pgfc )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_MIN( __d )			( f.att->big_vCompSizes->min = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_MAX( __d )			( f.att->big_vCompSizes->max = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_MEAN( __d )		( f.att->big_vCompSizes->mean = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_MEDIAN( __d )		( f.att->big_vCompSizes->median = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_SENT( __d )		( f.att->big_vCompSizes->sent = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_RENT( __d )		( f.att->big_vCompSizes->rent = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_MENT( __d )		( f.att->big_vCompSizes->ment = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_STDDEV( __d )		( f.att->big_vCompSizes->stddev = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_PGFA( __d )		( f.att->big_vCompSizes->pgfa = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_PGFB( __d )		( f.att->big_vCompSizes->pgfb = __d )
#define MAIN_SET_FATT_BIG_VCOMPSIZES_PGFC( __d )		( f.att->big_vCompSizes->pgfc = __d )

#define MAIN_GET_FATT_BIG_VCOMPDIAMS() 					( f.att->big_vCompDiams )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_MIN()				( f.att->big_vCompDiams->min )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_MAX()				( f.att->big_vCompDiams->max )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_MEAN()				( f.att->big_vCompDiams->mean )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_MEDIAN()			( f.att->big_vCompDiams->median )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_SENT()				( f.att->big_vCompDiams->sent )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_RENT()				( f.att->big_vCompDiams->rent )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_MENT()				( f.att->big_vCompDiams->ment )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_STDDEV()			( f.att->big_vCompDiams->stddev )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFA()				( f.att->big_vCompDiams->pgfa )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFB()				( f.att->big_vCompDiams->pgfb )
#define MAIN_GET_FATT_BIG_VCOMPDIAMS_PGFC()				( f.att->big_vCompDiams->pgfc )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_MIN( __d )			( f.att->big_vCompDiams->min = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_MAX( __d )			( f.att->big_vCompDiams->max = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_MEAN( __d )		( f.att->big_vCompDiams->mean = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_MEDIAN( __d )		( f.att->big_vCompDiams->median = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_SENT( __d )		( f.att->big_vCompDiams->sent = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_RENT( __d )		( f.att->big_vCompDiams->rent = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_MENT( __d )		( f.att->big_vCompDiams->ment = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_STDDEV( __d )		( f.att->big_vCompDiams->stddev = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFA( __d )		( f.att->big_vCompDiams->pgfa = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFB( __d )		( f.att->big_vCompDiams->pgfb = __d )
#define MAIN_SET_FATT_BIG_VCOMPDIAMS_PGFC( __d )		( f.att->big_vCompDiams->pgfc = __d )

#define MAIN_GET_FATT_BIG_NUMAP()						( f.att->big_numAP )
#define MAIN_SET_FATT_BIG_NUMAP( __d )					( f.att->big_numAP = __d )

//UP
#define MAIN_GET_FATT_UP_POSLITCONFFRAC()				( f.att->up_posLitConfFrac )
#define MAIN_SET_FATT_UP_POSLITCONFFRAC( __d )			( f.att->up_posLitConfFrac = __d )

#define MAIN_GET_FATT_UP_NEGLITCONFFRAC()				( f.att->up_negLitConfFrac )
#define MAIN_SET_FATT_UP_NEGLITCONFFRAC( __d )			( f.att->up_negLitConfFrac = __d )

#define MAIN_GET_FATT_UP_POSLITUPCOUNT() 				( f.att->up_posLitUPCount )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_MIN()			( f.att->up_posLitUPCount->min )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_MAX()			( f.att->up_posLitUPCount->max )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_MEAN()			( f.att->up_posLitUPCount->mean )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_MEDIAN()			( f.att->up_posLitUPCount->median )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_SENT()			( f.att->up_posLitUPCount->sent )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_RENT()			( f.att->up_posLitUPCount->rent )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_MENT()			( f.att->up_posLitUPCount->ment )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_STDDEV()			( f.att->up_posLitUPCount->stddev )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFA()			( f.att->up_posLitUPCount->pgfa )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFB()			( f.att->up_posLitUPCount->pgfb )
#define MAIN_GET_FATT_UP_POSLITUPCOUNT_PGFC()			( f.att->up_posLitUPCount->pgfc )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_MIN( __d )		( f.att->up_posLitUPCount->min = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_MAX( __d )		( f.att->up_posLitUPCount->max = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_MEAN( __d )		( f.att->up_posLitUPCount->mean = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_MEDIAN( __d )	( f.att->up_posLitUPCount->median = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_SENT( __d )		( f.att->up_posLitUPCount->sent = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_RENT( __d )		( f.att->up_posLitUPCount->rent = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_MENT( __d )		( f.att->up_posLitUPCount->ment = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_STDDEV( __d )	( f.att->up_posLitUPCount->stddev = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFA( __d )		( f.att->up_posLitUPCount->pgfa = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFB( __d )		( f.att->up_posLitUPCount->pgfb = __d )
#define MAIN_SET_FATT_UP_POSLITUPCOUNT_PGFC( __d )		( f.att->up_posLitUPCount->pgfc = __d )

#define MAIN_GET_FATT_UP_NEGLITUPCOUNT() 				( f.att->up_negLitUPCount )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_MIN()			( f.att->up_negLitUPCount->min )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_MAX()			( f.att->up_negLitUPCount->max )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_MEAN()			( f.att->up_negLitUPCount->mean )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_MEDIAN()			( f.att->up_negLitUPCount->median )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_SENT()			( f.att->up_negLitUPCount->sent )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_RENT()			( f.att->up_negLitUPCount->rent )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_MENT()			( f.att->up_negLitUPCount->ment )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_STDDEV()			( f.att->up_negLitUPCount->stddev )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFA()			( f.att->up_negLitUPCount->pgfa )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFB()			( f.att->up_negLitUPCount->pgfb )
#define MAIN_GET_FATT_UP_NEGLITUPCOUNT_PGFC()			( f.att->up_negLitUPCount->pgfc )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_MIN( __d )		( f.att->up_negLitUPCount->min = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_MAX( __d )		( f.att->up_negLitUPCount->max = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_MEAN( __d )		( f.att->up_negLitUPCount->mean = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_MEDIAN( __d )	( f.att->up_negLitUPCount->median = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_SENT( __d )		( f.att->up_negLitUPCount->sent = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_RENT( __d )		( f.att->up_negLitUPCount->rent = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_MENT( __d )		( f.att->up_negLitUPCount->ment = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_STDDEV( __d )	( f.att->up_negLitUPCount->stddev = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFA( __d )		( f.att->up_negLitUPCount->pgfa = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFB( __d )		( f.att->up_negLitUPCount->pgfb = __d )
#define MAIN_SET_FATT_UP_NEGLITUPCOUNT_PGFC( __d )		( f.att->up_negLitUPCount->pgfc = __d )

//RW
#define MAIN_GET_FATT_RW_Q05()							( f.att->rw_q05 )
#define MAIN_SET_FATT_RW_Q05( __d )						( f.att->rw_q05 = __d )
#define MAIN_GET_FATT_RW_Q10()							( f.att->rw_q10 )
#define MAIN_SET_FATT_RW_Q10( __d )						( f.att->rw_q10 = __d )
#define MAIN_GET_FATT_RW_Q25()							( f.att->rw_q25 )
#define MAIN_SET_FATT_RW_Q25( __d )						( f.att->rw_q25 = __d )
#define MAIN_GET_FATT_RW_Q50()							( f.att->rw_q50 )
#define MAIN_SET_FATT_RW_Q50( __d )						( f.att->rw_q50 = __d )
#define MAIN_GET_FATT_RW_Q75()							( f.att->rw_q75 )
#define MAIN_SET_FATT_RW_Q75( __d )						( f.att->rw_q75 = __d )
#define MAIN_GET_FATT_RW_Q90()							( f.att->rw_q90 )
#define MAIN_SET_FATT_RW_Q90( __d )						( f.att->rw_q90 = __d )
#define MAIN_GET_FATT_RW_Q95()							( f.att->rw_q95 )
#define MAIN_SET_FATT_RW_Q95( __d )						( f.att->rw_q95 = __d )

#define MAIN_GET_FATT_RW_VWEIGHTS() 					( f.att->rw_vWeights )
#define MAIN_GET_FATT_RW_VWEIGHTS_MIN()					( f.att->rw_vWeights->min )
#define MAIN_GET_FATT_RW_VWEIGHTS_MAX()					( f.att->rw_vWeights->max )
#define MAIN_GET_FATT_RW_VWEIGHTS_MEAN()				( f.att->rw_vWeights->mean )
#define MAIN_GET_FATT_RW_VWEIGHTS_MEDIAN()				( f.att->rw_vWeights->median )
#define MAIN_GET_FATT_RW_VWEIGHTS_SENT()				( f.att->rw_vWeights->sent )
#define MAIN_GET_FATT_RW_VWEIGHTS_RENT()				( f.att->rw_vWeights->rent )
#define MAIN_GET_FATT_RW_VWEIGHTS_MENT()				( f.att->rw_vWeights->ment )
#define MAIN_GET_FATT_RW_VWEIGHTS_STDDEV()				( f.att->rw_vWeights->stddev )
#define MAIN_GET_FATT_RW_VWEIGHTS_PGFA()				( f.att->rw_vWeights->pgfa )
#define MAIN_GET_FATT_RW_VWEIGHTS_PGFB()				( f.att->rw_vWeights->pgfb )
#define MAIN_GET_FATT_RW_VWEIGHTS_PGFC()				( f.att->rw_vWeights->pgfc )
#define MAIN_SET_FATT_RW_VWEIGHTS_MIN( __d )			( f.att->rw_vWeights->min = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_MAX( __d )			( f.att->rw_vWeights->max = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_MEAN( __d )			( f.att->rw_vWeights->mean = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_MEDIAN( __d )			( f.att->rw_vWeights->median = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_SENT( __d )			( f.att->rw_vWeights->sent = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_RENT( __d )			( f.att->rw_vWeights->rent = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_MENT( __d )			( f.att->rw_vWeights->ment = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_STDDEV( __d )			( f.att->rw_vWeights->stddev = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_PGFA( __d )			( f.att->rw_vWeights->pgfa = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_PGFB( __d )			( f.att->rw_vWeights->pgfb = __d )
#define MAIN_SET_FATT_RW_VWEIGHTS_PGFC( __d )			( f.att->rw_vWeights->pgfc = __d )

//ELS
#define MAIN_GET_FATT_ELS_NUMC()						( f.att->els_numC )
#define MAIN_SET_FATT_ELS_NUMC( __d )					( f.att->els_numC = __d )

#define MAIN_GET_FATT_ELS_SIZES() 						( f.att->els_sizes )
#define MAIN_GET_FATT_ELS_SIZES_MIN()					( f.att->els_sizes->min )
#define MAIN_GET_FATT_ELS_SIZES_MAX()					( f.att->els_sizes->max )
#define MAIN_GET_FATT_ELS_SIZES_MEAN()					( f.att->els_sizes->mean )
#define MAIN_GET_FATT_ELS_SIZES_MEDIAN()				( f.att->els_sizes->median )
#define MAIN_GET_FATT_ELS_SIZES_SENT()					( f.att->els_sizes->sent )
#define MAIN_GET_FATT_ELS_SIZES_RENT()					( f.att->els_sizes->rent )
#define MAIN_GET_FATT_ELS_SIZES_MENT()					( f.att->els_sizes->ment )
#define MAIN_GET_FATT_ELS_SIZES_STDDEV()				( f.att->els_sizes->stddev )
#define MAIN_GET_FATT_ELS_SIZES_PGFA()					( f.att->els_sizes->pgfa )
#define MAIN_GET_FATT_ELS_SIZES_PGFB()					( f.att->els_sizes->pgfb )
#define MAIN_GET_FATT_ELS_SIZES_PGFC()					( f.att->els_sizes->pgfc )
#define MAIN_SET_FATT_ELS_SIZES_MIN( __d )				( f.att->els_sizes->min = __d )
#define MAIN_SET_FATT_ELS_SIZES_MAX( __d )				( f.att->els_sizes->max = __d )
#define MAIN_SET_FATT_ELS_SIZES_MEAN( __d )				( f.att->els_sizes->mean = __d )
#define MAIN_SET_FATT_ELS_SIZES_MEDIAN( __d )			( f.att->els_sizes->median = __d )
#define MAIN_SET_FATT_ELS_SIZES_SENT( __d )				( f.att->els_sizes->sent = __d )
#define MAIN_SET_FATT_ELS_SIZES_RENT( __d )				( f.att->els_sizes->rent = __d )
#define MAIN_SET_FATT_ELS_SIZES_MENT( __d )				( f.att->els_sizes->ment = __d )
#define MAIN_SET_FATT_ELS_SIZES_STDDEV( __d )			( f.att->els_sizes->stddev = __d )
#define MAIN_SET_FATT_ELS_SIZES_PGFA( __d )				( f.att->els_sizes->pgfa = __d )
#define MAIN_SET_FATT_ELS_SIZES_PGFB( __d )				( f.att->els_sizes->pgfb = __d )
#define MAIN_SET_FATT_ELS_SIZES_PGFC( __d )				( f.att->els_sizes->pgfc = __d )

#define MAIN_FATT_INIT() {\
	f.att = malloc(sizeof(formulaAttributes)*1); \
	MAIN_SET_FATT_ISSET(0); \
	MAIN_SET_FATT_CLASS(0); \
	MAIN_SET_FATT_CERTAINTY(ZERO); \
	MAIN_SET_FATT_CERTAINTYDIST(ZERO); \
	/*PROBLEM SIZE ATTRIBUTES*/ \
	MAIN_SET_FATT_N(ZERO); \
	MAIN_SET_FATT_M(ZERO); \
	MAIN_SET_FATT_R(ZERO); \
	MAIN_SET_FATT_INITASS(ZERO); \
	MAIN_SET_FATT_INITDIS(ZERO); \
	MAIN_SET_FATT_INITPUREPOS(ZERO); \
	MAIN_SET_FATT_INITPURENEG(ZERO); \
	/*VARIABLE-CLAUSE GRAPH ATTRIBUTES*/ \
	f.att->vcg_vNodeDegree = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vcg_vNodeDegree); \
	f.att->vcg_cNodeDegree = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vcg_cNodeDegree); \
	MAIN_SET_FATT_VCG_MAYBERAND(ZERO); \
	/*VARIABLE GRAPH ATTRIBUTES*/ \
	f.att->vg_estAvgNDeg = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vg_estAvgNDeg); \
	f.att->vg_vNodeDegree = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vg_vNodeDegree); \
	MAIN_SET_FATT_VG_NUMCOMP(ZERO); \
	f.att->vg_vCompSizes = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vg_vCompSizes); \
	f.att->vg_vCompDiams = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->vg_vCompDiams); \
	MAIN_SET_FATT_VG_NUMAP(ZERO); \
	/*CLAUSE GRAPH ATTRIBUTES*/ \
	f.att->cg_cNodeDegree = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->cg_cNodeDegree); \
	f.att->cg_avgDegPerLit = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->cg_avgDegPerLit); \
	/*BALANCE ATTRIBUTES*/ \
	f.att->b_vLitBalance = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->b_vLitBalance); \
	f.att->b_cLitBalance = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->b_cLitBalance); \
	MAIN_SET_FATT_B_CSIZE2(ZERO); \
	MAIN_SET_FATT_B_CSIZE3(ZERO); \
	MAIN_SET_FATT_B_CSIZE4(ZERO); \
	MAIN_SET_FATT_B_CSIZE5(ZERO); \
	MAIN_SET_FATT_B_CSIZE6(ZERO); \
	MAIN_SET_FATT_B_CSIZE7(ZERO); \
	/*HORN PROXIMITY ATTRIBUTES*/ \
	MAIN_SET_FATT_HP_HORNFRAC(ZERO); \
	f.att->hp_vHornFrac = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->hp_vHornFrac); \
	f.att->hp_cHornDist = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->hp_cHornDist); \
	/*BIG ATTRIBUTES*/ \
	f.att->big_vNodeDegree = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->big_vNodeDegree); \
	MAIN_SET_FATT_BIG_VFRAC(ZERO); \
	MAIN_SET_FATT_BIG_NUMCOMP(ZERO); \
	f.att->big_vCompSizes = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->big_vCompSizes); \
	f.att->big_vCompDiams = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->big_vCompDiams); \
	MAIN_SET_FATT_BIG_NUMAP(ZERO); \
	/*UP ATTRIBUTES*/ \
	MAIN_SET_FATT_UP_POSLITCONFFRAC(ZERO); \
	MAIN_SET_FATT_UP_NEGLITCONFFRAC(ZERO); \
	f.att->up_posLitUPCount = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->up_posLitUPCount); \
	f.att->up_negLitUPCount = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->up_negLitUPCount); \
	/*RW ATTRIBUTES*/ \
	MAIN_SET_FATT_RW_Q05(ZERO); \
	MAIN_SET_FATT_RW_Q10(ZERO); \
	MAIN_SET_FATT_RW_Q25(ZERO); \
	MAIN_SET_FATT_RW_Q50(ZERO); \
	MAIN_SET_FATT_RW_Q75(ZERO); \
	MAIN_SET_FATT_RW_Q90(ZERO); \
	MAIN_SET_FATT_RW_Q95(ZERO); \
	f.att->rw_vWeights = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->rw_vWeights); \
	/*ELS ATTRIBUTES*/ \
	MAIN_SET_FATT_ELS_NUMC(ZERO); \
	f.att->els_sizes = malloc(sizeof(formulaAttributeSet)); \
	MAIN_RESET_FATT_FAS(f.att->els_sizes); \
}

#define MAIN_FATT_DISPOSE() { \
	if (f.att != NULL) {\
	free(f.att->vcg_vNodeDegree); \
	free(f.att->vcg_cNodeDegree); \
	free(f.att->vg_estAvgNDeg); \
	free(f.att->vg_vNodeDegree); \
	free(f.att->vg_vCompSizes); \
	free(f.att->vg_vCompDiams); \
	free(f.att->cg_cNodeDegree); \
	free(f.att->cg_avgDegPerLit); \
	free(f.att->b_vLitBalance); \
	free(f.att->b_cLitBalance); \
	free(f.att->hp_vHornFrac); \
	free(f.att->hp_cHornDist); \
	free(f.att->big_vNodeDegree); \
	free(f.att->big_vCompSizes); \
	free(f.att->big_vCompDiams); \
	free(f.att->up_posLitUPCount); \
	free(f.att->up_negLitUPCount); \
	free(f.att->rw_vWeights); \
	free(f.att->els_sizes); \
	free(f.att);\
	f.att = NULL;\
	}\
}

#define MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __value) {\
	__value = round(__value * __scale);\
	__value = __value / __scale; \
}

#define MAIN_FATT_SETPREC_FAS(__scale, __fas) {\
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->min); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->max); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->mean); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->median); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->sent); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->rent); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->ment); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->stddev); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->pgfa); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->pgfb); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, __fas->pgfc); \
}

#define MAIN_FATT_SETPREC(__scale) {\
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_N()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_M()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_R()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_INITASS()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_INITDIS()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_INITPUREPOS()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_INITPURENEG()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vcg_vNodeDegree); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vcg_cNodeDegree); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_VCG_MAYBERAND()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vg_estAvgNDeg); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vg_vNodeDegree); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_VG_NUMCOMP()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vg_vCompSizes); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->vg_vCompDiams); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_VG_NUMAP()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->cg_cNodeDegree); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->cg_avgDegPerLit); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->b_vLitBalance); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->b_cLitBalance); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE2()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE3()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE4()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE5()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE6()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE7()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_HP_HORNFRAC()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->hp_vHornFrac); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->hp_cHornDist); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->big_vNodeDegree); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_B_CSIZE2()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_BIG_NUMCOMP()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->big_vCompSizes); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->big_vCompDiams); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_BIG_NUMAP()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_UP_POSLITCONFFRAC()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_UP_NEGLITCONFFRAC()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->up_posLitUPCount); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->up_negLitUPCount); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q05()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q10()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q25()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q50()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q75()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q90()); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_RW_Q95()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->rw_vWeights); \
	MAIN_FATT_SETPREC_ROUNDSINGLE(__scale, MAIN_GET_FATT_ELS_NUMC()); \
	MAIN_FATT_SETPREC_FAS(__scale, f.att->els_sizes); \
}

#ifdef VERBOSE_CLASSIFY_ATTVALUE
#define MAIN_PRINT_FATTNAMES_GENERIC() { \
    printf("MAIN_GET_FATT_N "); \
    printf("MAIN_GET_FATT_M "); \
    printf("MAIN_GET_FATT_R "); \
    printf("MAIN_GET_FATT_INITASS "); \
    printf("MAIN_GET_FATT_INITDIS "); \
    printf("MAIN_GET_FATT_INITPUREPOS "); \
    printf("MAIN_GET_FATT_INITPURENEG "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_VNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_CNODEDEGREE"); \
    printf("MAIN_GET_FATT_VCG_MAYBERAND "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_ESTAVGNDEG"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VNODEDEGREE"); \
    printf("MAIN_GET_FATT_VG_NUMCOMP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPSIZES"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPDIAMS"); \
    printf("MAIN_GET_FATT_VG_NUMAP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_CNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_AVGDEGPERLIT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_VLITBALANCE"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_CLITBALANCE"); \
	printf("MAIN_GET_FATT_B_CSIZE2 "); \
	printf("MAIN_GET_FATT_B_CSIZE3 "); \
	printf("MAIN_GET_FATT_B_CSIZE4 "); \
	printf("MAIN_GET_FATT_B_CSIZE5 "); \
	printf("MAIN_GET_FATT_B_CSIZE6 "); \
	printf("MAIN_GET_FATT_B_CSIZE7 "); \
	printf("MAIN_GET_FATT_HP_HORNFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_VHORNFRAC"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_CHORNDIST"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VNODEDEGREE"); \
	printf("MAIN_GET_FATT_BIG_VFRAC "); \
	printf("MAIN_GET_FATT_BIG_NUMCOMP "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPSIZES"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPDIAMS"); \
	printf("MAIN_GET_FATT_BIG_NUMAP "); \
	printf("MAIN_GET_FATT_UP_POSLITCONFFRAC "); \
	printf("MAIN_GET_FATT_UP_NEGLITCONFFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_POSLITUPCOUNT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_NEGLITUPCOUNT"); \
	printf("MAIN_GET_FATT_RW_Q05 "); \
	printf("MAIN_GET_FATT_RW_Q10 "); \
	printf("MAIN_GET_FATT_RW_Q25 "); \
	printf("MAIN_GET_FATT_RW_Q50 "); \
	printf("MAIN_GET_FATT_RW_Q75 "); \
	printf("MAIN_GET_FATT_RW_Q90 "); \
	printf("MAIN_GET_FATT_RW_Q95 "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_RW_VWEIGHTS"); \
	printf("MAIN_GET_FATT_ELS_NUMC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_ELS_SIZES"); \
}

#define MAIN_PRINT_FATT_GENERIC() {\
	printf("%f ", MAIN_GET_FATT_N()); \
	printf("%f ", MAIN_GET_FATT_M()); \
	printf("%f ", MAIN_GET_FATT_R()); \
	printf("%f ", MAIN_GET_FATT_INITASS()); \
	printf("%f ", MAIN_GET_FATT_INITDIS()); \
	printf("%f ", MAIN_GET_FATT_INITPUREPOS()); \
	printf("%f ", MAIN_GET_FATT_INITPURENEG()); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_vNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_cNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VCG_MAYBERAND()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_estAvgNDeg); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_VG_NUMAP()); \
	MAIN_PRINT_FATT_FAS(f.att->cg_cNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->cg_avgDegPerLit); \
	MAIN_PRINT_FATT_FAS(f.att->b_vLitBalance); \
	MAIN_PRINT_FATT_FAS(f.att->b_cLitBalance); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE2()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE3()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE4()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE5()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE6()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE7()); \
	printf("%f ", MAIN_GET_FATT_HP_HORNFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->hp_vHornFrac); \
	MAIN_PRINT_FATT_FAS(f.att->hp_cHornDist); \
	MAIN_PRINT_FATT_FAS(f.att->big_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_BIG_VFRAC()); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMAP()); \
	printf("%f ", MAIN_GET_FATT_UP_POSLITCONFFRAC()); \
	printf("%f ", MAIN_GET_FATT_UP_NEGLITCONFFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->up_posLitUPCount); \
	MAIN_PRINT_FATT_FAS(f.att->up_negLitUPCount); \
	printf("%f ", MAIN_GET_FATT_RW_Q05()); \
	printf("%f ", MAIN_GET_FATT_RW_Q10()); \
	printf("%f ", MAIN_GET_FATT_RW_Q25()); \
	printf("%f ", MAIN_GET_FATT_RW_Q50()); \
	printf("%f ", MAIN_GET_FATT_RW_Q75()); \
	printf("%f ", MAIN_GET_FATT_RW_Q90()); \
	printf("%f ", MAIN_GET_FATT_RW_Q95()); \
	MAIN_PRINT_FATT_FAS(f.att->rw_vWeights); \
	printf("%f ", MAIN_GET_FATT_ELS_NUMC()); \
	MAIN_PRINT_FATT_FAS(f.att->els_sizes); \
}

#define MAIN_PRINT_FATTNAMES_RANDOM() { \
    printf("MAIN_GET_FATT_N "); \
    printf("MAIN_GET_FATT_M "); \
    printf("MAIN_GET_FATT_R "); \
    printf("MAIN_GET_FATT_INITASS "); \
    printf("MAIN_GET_FATT_INITDIS "); \
    printf("MAIN_GET_FATT_INITPUREPOS "); \
    printf("MAIN_GET_FATT_INITPURENEG "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_VNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_CNODEDEGREE"); \
    printf("MAIN_GET_FATT_VCG_MAYBERAND "); \
}

#define MAIN_PRINT_FATT_RANDOM() {\
	printf("%f ", MAIN_GET_FATT_N()); \
	printf("%f ", MAIN_GET_FATT_M()); \
	printf("%f ", MAIN_GET_FATT_R()); \
	printf("%f ", MAIN_GET_FATT_INITASS()); \
	printf("%f ", MAIN_GET_FATT_INITDIS()); \
	printf("%f ", MAIN_GET_FATT_INITPUREPOS()); \
	printf("%f ", MAIN_GET_FATT_INITPURENEG()); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_vNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_cNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VCG_MAYBERAND()); \
}

#define MAIN_PRINT_FATTNAMES_CRAFTED() { \
    printf("MAIN_GET_FATT_N "); \
    printf("MAIN_GET_FATT_M "); \
    printf("MAIN_GET_FATT_R "); \
    printf("MAIN_GET_FATT_INITASS "); \
    printf("MAIN_GET_FATT_INITDIS "); \
    printf("MAIN_GET_FATT_INITPUREPOS "); \
    printf("MAIN_GET_FATT_INITPURENEG "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_VNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_CNODEDEGREE"); \
    printf("MAIN_GET_FATT_VCG_MAYBERAND "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_ESTAVGNDEG"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VNODEDEGREE"); \
    printf("MAIN_GET_FATT_VG_NUMCOMP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPSIZES"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPDIAMS"); \
    printf("MAIN_GET_FATT_VG_NUMAP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_CNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_AVGDEGPERLIT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_VLITBALANCE"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_CLITBALANCE"); \
	printf("MAIN_GET_FATT_B_CSIZE2 "); \
	printf("MAIN_GET_FATT_B_CSIZE3 "); \
	printf("MAIN_GET_FATT_B_CSIZE4 "); \
	printf("MAIN_GET_FATT_B_CSIZE5 "); \
	printf("MAIN_GET_FATT_B_CSIZE6 "); \
	printf("MAIN_GET_FATT_B_CSIZE7 "); \
	printf("MAIN_GET_FATT_HP_HORNFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_VHORNFRAC"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_CHORNDIST"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VNODEDEGREE"); \
	printf("MAIN_GET_FATT_BIG_VFRAC "); \
	printf("MAIN_GET_FATT_BIG_NUMCOMP "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPSIZES"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPDIAMS"); \
	printf("MAIN_GET_FATT_BIG_NUMAP "); \
	printf("MAIN_GET_FATT_UP_POSLITCONFFRAC "); \
	printf("MAIN_GET_FATT_UP_NEGLITCONFFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_POSLITUPCOUNT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_NEGLITUPCOUNT"); \
	printf("MAIN_GET_FATT_RW_Q05 "); \
	printf("MAIN_GET_FATT_RW_Q10 "); \
	printf("MAIN_GET_FATT_RW_Q25 "); \
	printf("MAIN_GET_FATT_RW_Q50 "); \
	printf("MAIN_GET_FATT_RW_Q75 "); \
	printf("MAIN_GET_FATT_RW_Q90 "); \
	printf("MAIN_GET_FATT_RW_Q95 "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_RW_VWEIGHTS"); \
	printf("MAIN_GET_FATT_ELS_NUMC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_ELS_SIZES"); \
}

#define MAIN_PRINT_FATT_CRAFTED() {\
	printf("%f ", MAIN_GET_FATT_N()); \
	printf("%f ", MAIN_GET_FATT_M()); \
	printf("%f ", MAIN_GET_FATT_R()); \
	printf("%f ", MAIN_GET_FATT_INITASS()); \
	printf("%f ", MAIN_GET_FATT_INITDIS()); \
	printf("%f ", MAIN_GET_FATT_INITPUREPOS()); \
	printf("%f ", MAIN_GET_FATT_INITPURENEG()); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_vNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_cNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VCG_MAYBERAND()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_estAvgNDeg); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_VG_NUMAP()); \
	MAIN_PRINT_FATT_FAS(f.att->cg_cNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->cg_avgDegPerLit); \
	MAIN_PRINT_FATT_FAS(f.att->b_vLitBalance); \
	MAIN_PRINT_FATT_FAS(f.att->b_cLitBalance); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE2()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE3()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE4()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE5()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE6()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE7()); \
	printf("%f ", MAIN_GET_FATT_HP_HORNFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->hp_vHornFrac); \
	MAIN_PRINT_FATT_FAS(f.att->hp_cHornDist); \
	MAIN_PRINT_FATT_FAS(f.att->big_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_BIG_VFRAC()); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMAP()); \
	printf("%f ", MAIN_GET_FATT_UP_POSLITCONFFRAC()); \
	printf("%f ", MAIN_GET_FATT_UP_NEGLITCONFFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->up_posLitUPCount); \
	MAIN_PRINT_FATT_FAS(f.att->up_negLitUPCount); \
	printf("%f ", MAIN_GET_FATT_RW_Q05()); \
	printf("%f ", MAIN_GET_FATT_RW_Q10()); \
	printf("%f ", MAIN_GET_FATT_RW_Q25()); \
	printf("%f ", MAIN_GET_FATT_RW_Q50()); \
	printf("%f ", MAIN_GET_FATT_RW_Q75()); \
	printf("%f ", MAIN_GET_FATT_RW_Q90()); \
	printf("%f ", MAIN_GET_FATT_RW_Q95()); \
	MAIN_PRINT_FATT_FAS(f.att->rw_vWeights); \
	printf("%f ", MAIN_GET_FATT_ELS_NUMC()); \
	MAIN_PRINT_FATT_FAS(f.att->els_sizes); \
}

#define MAIN_PRINT_FATTNAMES_APPLICATION() { \
    printf("MAIN_GET_FATT_N "); \
    printf("MAIN_GET_FATT_M "); \
    printf("MAIN_GET_FATT_R "); \
    printf("MAIN_GET_FATT_INITASS "); \
    printf("MAIN_GET_FATT_INITDIS "); \
    printf("MAIN_GET_FATT_INITPUREPOS "); \
    printf("MAIN_GET_FATT_INITPURENEG "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_VNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VCG_CNODEDEGREE"); \
    printf("MAIN_GET_FATT_VCG_MAYBERAND "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_ESTAVGNDEG"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VNODEDEGREE"); \
    printf("MAIN_GET_FATT_VG_NUMCOMP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPSIZES"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_VG_VCOMPDIAMS"); \
    printf("MAIN_GET_FATT_VG_NUMAP "); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_CNODEDEGREE"); \
    MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_CG_AVGDEGPERLIT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_VLITBALANCE"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_B_CLITBALANCE"); \
	printf("MAIN_GET_FATT_B_CSIZE2 "); \
	printf("MAIN_GET_FATT_B_CSIZE3 "); \
	printf("MAIN_GET_FATT_B_CSIZE4 "); \
	printf("MAIN_GET_FATT_B_CSIZE5 "); \
	printf("MAIN_GET_FATT_B_CSIZE6 "); \
	printf("MAIN_GET_FATT_B_CSIZE7 "); \
	printf("MAIN_GET_FATT_HP_HORNFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_VHORNFRAC"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_HP_CHORNDIST"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VNODEDEGREE"); \
	printf("MAIN_GET_FATT_BIG_VFRAC "); \
	printf("MAIN_GET_FATT_BIG_NUMCOMP "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPSIZES"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_BIG_VCOMPDIAMS"); \
	printf("MAIN_GET_FATT_BIG_NUMAP "); \
	printf("MAIN_GET_FATT_UP_POSLITCONFFRAC "); \
	printf("MAIN_GET_FATT_UP_NEGLITCONFFRAC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_POSLITUPCOUNT"); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_UP_NEGLITUPCOUNT"); \
	printf("MAIN_GET_FATT_RW_Q05 "); \
	printf("MAIN_GET_FATT_RW_Q10 "); \
	printf("MAIN_GET_FATT_RW_Q25 "); \
	printf("MAIN_GET_FATT_RW_Q50 "); \
	printf("MAIN_GET_FATT_RW_Q75 "); \
	printf("MAIN_GET_FATT_RW_Q90 "); \
	printf("MAIN_GET_FATT_RW_Q95 "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_RW_VWEIGHTS"); \
	printf("MAIN_GET_FATT_ELS_NUMC "); \
	MAIN_PRINT_FATT_FASN("MAIN_GET_FATT_ELS_SIZES"); \
}

#define MAIN_PRINT_FATT_APPLICATION() {\
	printf("%f ", MAIN_GET_FATT_N()); \
	printf("%f ", MAIN_GET_FATT_M()); \
	printf("%f ", MAIN_GET_FATT_R()); \
	printf("%f ", MAIN_GET_FATT_INITASS()); \
	printf("%f ", MAIN_GET_FATT_INITDIS()); \
	printf("%f ", MAIN_GET_FATT_INITPUREPOS()); \
	printf("%f ", MAIN_GET_FATT_INITPURENEG()); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_vNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->vcg_cNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VCG_MAYBERAND()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_estAvgNDeg); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_VG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->vg_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_VG_NUMAP()); \
	MAIN_PRINT_FATT_FAS(f.att->cg_cNodeDegree); \
	MAIN_PRINT_FATT_FAS(f.att->cg_avgDegPerLit); \
	MAIN_PRINT_FATT_FAS(f.att->b_vLitBalance); \
	MAIN_PRINT_FATT_FAS(f.att->b_cLitBalance); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE2()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE3()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE4()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE5()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE6()); \
	printf("%f ", MAIN_GET_FATT_B_CSIZE7()); \
	printf("%f ", MAIN_GET_FATT_HP_HORNFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->hp_vHornFrac); \
	MAIN_PRINT_FATT_FAS(f.att->hp_cHornDist); \
	MAIN_PRINT_FATT_FAS(f.att->big_vNodeDegree); \
	printf("%f ", MAIN_GET_FATT_BIG_VFRAC()); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMCOMP()); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompSizes); \
	MAIN_PRINT_FATT_FAS(f.att->big_vCompDiams); \
	printf("%f ", MAIN_GET_FATT_BIG_NUMAP()); \
	printf("%f ", MAIN_GET_FATT_UP_POSLITCONFFRAC()); \
	printf("%f ", MAIN_GET_FATT_UP_NEGLITCONFFRAC()); \
	MAIN_PRINT_FATT_FAS(f.att->up_posLitUPCount); \
	MAIN_PRINT_FATT_FAS(f.att->up_negLitUPCount); \
	printf("%f ", MAIN_GET_FATT_RW_Q05()); \
	printf("%f ", MAIN_GET_FATT_RW_Q10()); \
	printf("%f ", MAIN_GET_FATT_RW_Q25()); \
	printf("%f ", MAIN_GET_FATT_RW_Q50()); \
	printf("%f ", MAIN_GET_FATT_RW_Q75()); \
	printf("%f ", MAIN_GET_FATT_RW_Q90()); \
	printf("%f ", MAIN_GET_FATT_RW_Q95()); \
	MAIN_PRINT_FATT_FAS(f.att->rw_vWeights); \
	printf("%f ", MAIN_GET_FATT_ELS_NUMC()); \
	MAIN_PRINT_FATT_FAS(f.att->els_sizes); \
}
#endif

typedef struct {
	formulaAttributes* att;	//The pointer to the structure containing all the attributes of the original formula.
	variable* vars;			//An array of variables that are used in the formula.
	variable** vars_e;		//An array of pointers to the enabled variables.
	variable** vars_d;		//An array of pointers to the disabled variables.

	literal* lits;			//An array of the literals that are used in the formula.

	clause** clauses_eo; 	//An array of pointers to the enabled original clauses of the formula. ID is 00.
	clause** clauses_do; 	//An array of pointers to the disabled original clauses of the formula. ID is 01.
	clause** clauses_el;	//An array of pointers to the enabled learned clauses of the formula. ID is 10.
	clause** clauses_dl;	//An array of pointers to the disabled learned clauses of the formula. ID is 11.

	remClause** clauses_rem;//An array of pointers to the removed (NIVER/BLOCKED/COVERED) clauses.

	uint32_t n_initial;		//The number of initial variables in the formula.
	uint32_t n_afterPrep;	//The number of variables after prerocessing finished its work.
	uint32_t n_vars_e_used;	//The amount of address pointers used (number of variables) for the vars_e array.
	uint32_t n_vars_d_used;	//The amount of address pointers used (number of variables) for the vars_d array.

	uint32_t m_initial;		//The number of initial clauses in the formula, they are all original clauses of course.

	uint32_t m_eo_used;		//The amount of address pointers used (number of clauses) for the clauses_eo array.
	uint32_t m_eo_avail;	//The amount of address pointers available for the clauses_eo array.

	uint32_t m_do_used;		//The amount of address pointers used (number of clauses) for the clauses_do array.
	uint32_t m_do_avail;	//The amount of address pointers available for the clauses_do array.

	uint32_t m_el_used;		//The amount of address pointers used (number of clauses) for the clauses_el array.
	uint32_t m_el_avail;	//The amount of address pointers available for the clauses_el array.

	uint32_t m_dl_used;		//The amount of address pointers used (number of clauses) for the clauses_dl array.
	uint32_t m_dl_avail;	//The amount of address pointers available for the clauses_dl array.

	uint32_t m_rem_used;	//The amount of address pointers used (number of removed clauses) for the clauses_rem array.
	uint32_t m_rem_avail;	//The amount of address pointers available for the clauses_rem array.
} CNF;

void formula_reset();		//For resetting the CNF formula data.
uint32_t formula_init();	//For initializing the CNF formula data-structure. It returns 0 if it was unsuccessful.
void formula_dispose();		//For disposing the CNF formula data-structure.
#endif /* FORMULA_H_ */
