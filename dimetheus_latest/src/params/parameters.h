/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "../defines.h"

//MAIN
FILE *param_formula;					//The file containing the formula in DIMACS CNF INPUT format.

int32_t param_seed;						//The seed for the random number generator.
#define PARAM_DEFAULT_SEED 101

#ifdef SPECS_PARAMGRAPH
//SPECS
FILE *param_specsGraphOutput;			//The file containing the EDACC parameter Graph.
#endif


//RESET


//PARAMS


//GUIDANCE
int32_t param_guide;					//This is the selector for one of the guides. Check guides/guides.h for values.
#define PARAM_DEFAULT_GUIDE 1
#define PARAM_FUZZMIN_GUIDE GUIDE_CDCL
#define PARAM_FUZZMAX_GUIDE GUIDE_MAXVALUE


//STARTUP
FILE *param_startupOutput;				//The file containing the formula in DIMACS CNF INPUT format after startup.
int32_t param_startupInputFormat;		//This tells us what input format is used (e.g. DIMACS CNF Input format).
#define PARAM_DEFAULT_STARTUPINPUTFORMAT STARTUP_INPUTFORMAT_UNKNOWN

//CLASSIFY
int32_t param_classifyInputDomain;		//This tells us what domain the problem is from (e.g. random or generic).
#define PARAM_DEFAULT_CLASSIFYINPUTDOMAIN CLASSIFY_INPUTDOMAIN_GENERIC

float_ty param_classifyAttSetTime;		//The time the classifier is allowed to use up to compute a single attribute set.
#define PARAM_DEFAULT_CLASSIFYATTSETTIME 30.0

int32_t param_classifyFloatPrec;		//The precision of floating point values (number of digits after the comma).
#define PARAM_DEFAULT_CLASSIFYFLOATPREC 6

int32_t param_classifyMaxVGSize;		//The maximum number of variables we allow to build up a variable graph.
#define PARAM_DEFAULT_CLASSIFYMAXVGSIZE 1000000

int32_t param_classifyMaxCGSize;		//The maximum number of clauses we allow to compute clause graph attributes.
#define PARAM_DEFAULT_CLASSIFYMAXCGSIZE 20000000

int32_t param_classifyMaxUPImps;		//The maximum number of literal implications for UP attribute computations.
#define PARAM_DEFAULT_CLASSIFYMAXUPIMPS 75

int32_t param_classifyMaxUPn;			//The maximum number of variables allowed for UP attribute computations.
#define PARAM_DEFAULT_CLASSIFYMAXUPN 150000

float_ty param_classifyPGFA;			//Used as input for calculating a probability generating function value.
#define PARAM_DEFAULT_CLASSIFYPGFA 0.9

float_ty param_classifyPGFB;			//Used as input for calculating a probability generating function value.
#define PARAM_DEFAULT_CLASSIFYPGFB 0.8

float_ty param_classifyPGFC;			//Used as input for calculating a probability generating function value.
#define PARAM_DEFAULT_CLASSIFYPGFC 0.7

int32_t param_classifySimplify;			//Whether the classification phase is supposed to simplify the formula.
#define PARAM_DEFAULT_CLASSIFYSIMPLIFY 1

int32_t param_classifyStrategy;			//The classification strategy that is used.
#define PARAM_DEFAULT_CLASSIFYSTRATEGY 2
#define PARAM_FUZZMIN_CLASSIFYSTRATEGY 0
#define PARAM_FUZZMAX_CLASSIFYSTRATEGY 0


//ADAPT
int32_t param_adaptStrategy;			//The adaptation strategy that is used.
#define PARAM_DEFAULT_ADAPTSTRATEGY 1
#define PARAM_FUZZMIN_ADAPTSTRATEGY 0
#define PARAM_FUZZMAX_ADAPTSTRATEGY 0

int32_t param_adaptPrepStrategy;		//If adapting the preprocessing strategy is allowed.
#define PARAM_DEFAULT_ADAPTPREPSTRATEGY 0

int32_t param_adaptPrepStrategyParams;	//If adapting the preprocessing strategy parameters is allowed.
#define PARAM_DEFAULT_ADAPTPREPSTRATEGYPARAMS 0

int32_t param_adaptInpStrategy;		//If adapting the in-processing strategy is allowed.
#define PARAM_DEFAULT_ADAPTINPSTRATEGY 0

int32_t param_adaptInpStrategyParams;	//If adapting the in-processing strategy is allowed.
#define PARAM_DEFAULT_ADAPTINPSTRATEGYPARAMS 0

int32_t param_adaptSearchStrategy;		//If adapting the search strategy is allowed.
#define PARAM_DEFAULT_ADAPTSEARCHSTRATEGY 0

int32_t param_adaptSearchStrategyParams;	//If adapting the search strategy parameters is allowed.
#define PARAM_DEFAULT_ADAPTSEARCHSTRATEGYPARAMS 0

int32_t param_adaptMpUR;				//If adapting the MP UR is allowed.
#define PARAM_DEFAULT_ADAPTMPUR 0

int32_t param_adaptMpURParams;			//If adapting the MP UR parameters is allowed.
#define PARAM_DEFAULT_ADAPTMPURPARAMS 0

int32_t param_adaptSlsPAFVR;			//If adapting the SLS PAFVR is allowed.
#define PARAM_DEFAULT_ADAPTSLSPAFVR 0

int32_t param_adaptSlsPAFVRParams;		//If adapting the SLS PAFVR parameters is allowed.
#define PARAM_DEFAULT_ADAPTSLSPAFVRPARAMS 0

int32_t param_adaptSlsAIR;				//If adapting the SLS AIR is allowed.
#define PARAM_DEFAULT_ADAPTSLSAIR 0

int32_t param_adaptSlsAIRParams;			//If adapting the SLS AIR parameters is allowed.
#define PARAM_DEFAULT_ADAPTSLSAIRPARAMS 0

int32_t param_adaptSlsTRR;				//If adapting the SLS TRR is allowed.
#define PARAM_DEFAULT_ADAPTSLSTRR 0

int32_t param_adaptSlsTRRParams;		//If adapting the SLS TRR parameters is allowed.
#define PARAM_DEFAULT_ADAPTSLSTRRPARAMS 0

int32_t param_adaptCdclSVR;				//If adapting the CDCL SVR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSVR 0

int32_t param_adaptCdclSVRParams;		//If adapting the CDCL SVR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSVRPARAMS 0

int32_t param_adaptCdclSVAI;			//If adapting the CDCL SVAI is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSVAI 0

int32_t param_adaptCdclSVAIParams;		//If adapting the CDCL SVAI parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSVAIPARAMS 0

int32_t param_adaptCdclSDR;				//If adapting the CDCL SDR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSDR 0

int32_t param_adaptCdclSDRParams;		//If adapting the CDCL SDR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSDRPARAMS 0

int32_t param_adaptCdclSDDI;			//If adapting the CDCL SDDI is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSDDI 0

int32_t param_adaptCdclSDDIParams;		//If adapting the CDCL SDDI parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSDDIPARAMS 0

int32_t param_adaptCdclCAR;				//If adapting the CDCL CAR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLCAR 0

int32_t param_adaptCdclCARParams;		//If adapting the CDCL CAR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLCARPARAMS 0

int32_t param_adaptCdclSLR;				//If adapting the CDCL SLR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSLR 0

int32_t param_adaptCdclSLRParams;		//If adapting the CDCL SLS parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSLRPARAMS 0

int32_t param_adaptCdclSOR;				//If adapting the CDCL SOR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSOR 0

int32_t param_adaptCdclSORParams;		//If adapting the CDCL SOR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLSORPARAMS 0

int32_t param_adaptCdclCBJLR;			//If adapting the CDCL CBJLR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLCBJLR 0

int32_t param_adaptCdclCBJLRParams;		//If adapting the CDCL CBJLR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLCBJLRPARAMS 0

int32_t param_adaptCdclRR;				//If adapting the CDCL RR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLRR 0

int32_t param_adaptCdclRRParams;		//If adapting the CDCL RR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLRRPARAMS 0

int32_t param_adaptCdclMRParams;		//If adapting the CDCL MR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLMRPARAMS 0

int32_t param_adaptCdclMR;				//If adapting the CDCL MR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLMR 0

int32_t param_adaptCdclIR;				//If adapting the CDCL IR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLIR 0

int32_t param_adaptCdclIRParams;		//If adapting the CDCL IR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLIRPARAMS 0

int32_t param_adaptCdclAR;				//If adapting the CDCL AR is allowed.
#define PARAM_DEFAULT_ADAPTCDCLAR 0

int32_t param_adaptCdclARParams;		//If adapting the CDCL AR parameters is allowed.
#define PARAM_DEFAULT_ADAPTCDCLARPARAMS 0


//PREP
FILE *param_prepOutput;					//The file containing the formula in DIMACS CNF INPUT format after preprocessing.

int32_t param_prepReviveCls;			//Bring back some of the removed clauses if the preprocessor terminates with UNKNOWN.
#define PARAM_DEFAULT_PREPREVIVECLS 1
#define PARAM_FUZZMIN_PREPREVIVECLS 0
#define PARAM_FUZZMAX_PREPREVIVECLS 1
#define PARAM_HARDMIN_PREPREVIVECLS 0
#define PARAM_HARDMAX_PREPREVIVECLS 1

int32_t param_prepUNHRounds;			//How many rounds of unhiding in a single call to UNH.
#define PARAM_DEFAULT_PREPUNHROUNDS 10
#define PARAM_FUZZMIN_PREPUNHROUNDS 1
#define PARAM_FUZZMAX_PREPUNHROUNDS 20
#define PARAM_HARDMIN_PREPUNHROUNDS 1
#define PARAM_HARDMAX_PREPUNHROUNDS INT32_MAX

int32_t param_prepACCEMax;				//Maximum literal occurrences allowed for the literal to be added to ACCE queue.
#define PARAM_DEFAULT_PREPACCEMAX 20
#define PARAM_FUZZMIN_PREPACCEMAX 1
#define PARAM_FUZZMAX_PREPACCEMAX 50
#define PARAM_HARDMIN_PREPACCEMAX 0
#define PARAM_HARDMAX_PREPACCEMAX INT32_MAX

int32_t param_prepACCEMinClsSize;		//Minimum clause size for clauses to be checked by ACCE.
#define PARAM_DEFAULT_PREPACCEMINCLSSIZE 3
#define PARAM_FUZZMIN_PREPACCEMINCLSSIZE 2
#define PARAM_FUZZMAX_PREPACCEMINCLSSIZE 50
#define PARAM_HARDMIN_PREPACCEMINCLSSIZE 2
#define PARAM_HARDMAX_PREPACCEMINCLSSIZE INT32_MAX

int32_t param_prepACCESaturate;			//Tells us if ACCE is to be performed until saturation.
#define PARAM_DEFAULT_PREPACCESATURATE 0
#define PARAM_FUZZMIN_PREPACCESATURATE 0
#define PARAM_FUZZMAX_PREPACCESATURATE 1
#define PARAM_HARDMIN_PREPACCESATURATE 0
#define PARAM_HARDMAX_PREPACCESATURATE 1

int32_t param_prepNIVERMax;				//Maximum literal occurrences allowed for the variable to be added to NIVER queue.
#define PARAM_DEFAULT_PREPNIVERMAX 25
#define PARAM_FUZZMIN_PREPNIVERMAX 5
#define PARAM_FUZZMAX_PREPNIVERMAX 500
#define PARAM_HARDMIN_PREPNIVERMAX 0
#define PARAM_HARDMAX_PREPNIVERMAX INT32_MAX

int32_t param_prepATEMin;				//Minimum size of a clause that is to be checked by separate ATE.
#define PARAM_DEFAULT_PREPATEMIN 3
#define PARAM_FUZZMIN_PREPATEMIN 2
#define PARAM_FUZZMAX_PREPATEMIN 50
#define PARAM_HARDMIN_PREPATEMIN 0
#define PARAM_HARDMAX_PREPATEMIN INT32_MAX

int32_t param_prepGEMaxAONN;			//Maximum number of resolvents allowed when extracting an AND/OR/NAND/NOR gate in GE.
#define PARAM_DEFAULT_PREPGEMAXAONN 8
#define PARAM_FUZZMIN_PREPGEMAXAONN 5
#define PARAM_FUZZMAX_PREPGEMAXAONN 1000
#define PARAM_HARDMIN_PREPGEMAXAONN 0
#define PARAM_HARDMAX_PREPGEMAXAONN INT32_MAX

int32_t param_prepGEMaxXOR;				//Maximum number of resolvents allowed to be added when extracting an XOR gate in GE.
#define PARAM_DEFAULT_PREPGEMAXXOR 10000
#define PARAM_FUZZMIN_PREPGEMAXXOR 5
#define PARAM_FUZZMAX_PREPGEMAXXOR 25000
#define PARAM_HARDMIN_PREPGEMAXXOR 0
#define PARAM_HARDMAX_PREPGEMAXXOR INT32_MAX

int32_t param_prepTERNARYLitMax;		//Maximum number of resolvents for a literal in TERNARY resolution.
#define PARAM_DEFAULT_PREPTERNARYLITMAX 1000
#define PARAM_FUZZMIN_PREPTERNARYLITMAX 100
#define PARAM_FUZZMAX_PREPTERNARYLITMAX 5000
#define PARAM_HARDMIN_PREPTERNARYLITMAX 0
#define PARAM_HARDMAX_PREPTERNARYLITMAX INT32_MAX

int32_t param_prepTERNARYMax;			//Maximum number of resolvents total in a run of TERNARY resolution.
#define PARAM_DEFAULT_PREPTERNARYMAX 5000
#define PARAM_FUZZMIN_PREPTERNARYMAX 1000
#define PARAM_FUZZMAX_PREPTERNARYMAX 20000
#define PARAM_HARDMIN_PREPTERNARYMAX 0
#define PARAM_HARDMAX_PREPTERNARYMAX INT32_MAX

int32_t param_prepNHBRMin;				//Minimum number of implications for a literal to be used in NHBR.
#define PARAM_DEFAULT_PREPNHBRMIN 10
#define PARAM_FUZZMIN_PREPNHBRMIN 5
#define PARAM_FUZZMAX_PREPNHBRMIN 500
#define PARAM_HARDMIN_PREPNHBRMIN 0
#define PARAM_HARDMAX_PREPNHBRMIN INT32_MAX

int32_t param_prepStrategy;				//The preprocessing strategy that is used.
#define PARAM_DEFAULT_PREPSTRATEGY 2
#define PARAM_FUZZMIN_PREPSTRATEGY PREP_STRATEGY_NULL
#define PARAM_FUZZMAX_PREPSTRATEGY PREP_STRATEGY_RAND

//INP
int32_t param_inpStrMaxSize;			//The maximum size of a clause we do strengthening with.
#define PARAM_DEFAULT_INPSTRMAXSIZE 4
#define PARAM_FUZZMIN_INPSTRMAXSIZE 3
#define PARAM_FUZZMAX_INPSTRMAXSIZE 6
#define PARAM_HARDMIN_INPSTRMAXSIZE 0
#define PARAM_HARDMAX_INPSTRMAXSIZE INT32_MAX

int32_t param_inpTernaryLitMax;			//The maximum number of ternary clauses that can be learned with one literal.
#define PARAM_DEFAULT_INPTERNARYLITMAX 100
#define PARAM_FUZZMIN_INPTERNARYLITMAX 10
#define PARAM_FUZZMAX_INPTERNARYLITMAX 1000
#define PARAM_HARDMIN_INPTERNARYLITMAX 0
#define PARAM_HARDMAX_INPTERNARYLITMAX INT32_MAX

int32_t param_inpTernaryMax;			//The maximum number of ternary clauses that can be learned in total.
#define PARAM_DEFAULT_INPTERNARYMAX 1000
#define PARAM_FUZZMIN_INPTERNARYMAX 10
#define PARAM_FUZZMAX_INPTERNARYMAX 5000
#define PARAM_HARDMIN_INPTERNARYMAX 0
#define PARAM_HARDMAX_INPTERNARYMAX INT32_MAX

int32_t param_inpNiverMax;				//The maximum literal occurrence to check a variable with NIVER.
#define PARAM_DEFAULT_INPNIVERMAX 25
#define PARAM_FUZZMIN_INPNIVERMAX 3
#define PARAM_FUZZMAX_INPNIVERMAX 25
#define PARAM_HARDMIN_INPNIVERMAX 0
#define PARAM_HARDMAX_INPNIVERMAX INT32_MAX

int32_t param_inpGEMaxAONN;				//The maximum number of resolvents allowed when removing AND/OR/NAND/NOR gates in GE.
#define PARAM_DEFAULT_INPGEMAXAONN 8
#define PARAM_FUZZMIN_INPGEMAXAONN 1
#define PARAM_FUZZMAX_INPGEMAXAONN 100
#define PARAM_HARDMIN_INPGEMAXAONN 0
#define PARAM_HARDMAX_INPGEMAXAONN INT32_MAX

int32_t param_inpGEMaxXOR;				//The maximum number of resolvents allowed when removing XOR gates in GE.
#define PARAM_DEFAULT_INPGEMAXXOR 50
#define PARAM_FUZZMIN_INPGEMAXXOR 1
#define PARAM_FUZZMAX_INPGEMAXXOR 100
#define PARAM_HARDMIN_INPGEMAXXOR 0
#define PARAM_HARDMAX_INPGEMAXXOR INT32_MAX

int32_t param_inpNHBRMin;				//The minimum number of implications a literal must have to be used in NHBR.
#define PARAM_DEFAULT_INPNHBRMIN 10
#define PARAM_FUZZMIN_INPNHBRMIN 1
#define PARAM_FUZZMAX_INPNHBRMIN 50
#define PARAM_HARDMIN_INPNHBRMIN 0
#define PARAM_HARDMAX_INPNHBRMIN INT32_MAX

int32_t param_inpUNHRounds;				//The number of unhiding rounds in a single call to the method.
#define PARAM_DEFAULT_INPUNHROUNDS 10
#define PARAM_FUZZMIN_INPUNHROUNDS 1
#define PARAM_FUZZMAX_INPUNHROUNDS 20
#define PARAM_HARDMIN_INPUNHROUNDS 1
#define PARAM_HARDMAX_INPUNHROUNDS INT32_MAX

int32_t param_inpStrategy;				//The in-processing strategy that is used.
#define PARAM_DEFAULT_INPSTRATEGY 2
#define PARAM_FUZZMIN_INPSTRATEGY PREP_STRATEGY_NULL
#define PARAM_FUZZMAX_INPSTRATEGY PREP_STRATEGY_RAND

//SEARCH
float_ty param_searchMIDBlockSize;		//The fraction of new variables we want to assign when doing MID.
#define PARAM_DEFAULT_SEARCHMIDBLOCKSIZE 0.05
#define PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE 0.0
#define PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE 1.0
#define PARAM_HARDMIN_SEARCHMIDBLOCKSIZE 0.0
#define PARAM_HARDMAX_SEARCHMIDBLOCKSIZE 1.0

float_ty param_searchRetreatFact;		//The fraction of old variables we allow to unassign when doing MID.
#define PARAM_DEFAULT_SEARCHRETREATFACT 0.25
#define PARAM_FUZZMIN_SEARCHRETREATFACT 0.0
#define PARAM_FUZZMAX_SEARCHRETREATFACT 1.0
#define PARAM_HARDMIN_SEARCHRETREATFACT 0.0
#define PARAM_HARDMAX_SEARCHRETREATFACT 1.0

float_ty param_searchMinClsImpact;		//The minimum MP impact a learned clause must have such that it is retained.
#define PARAM_DEFAULT_SEARCHMINCLSIMPACT 0.333
#define PARAM_FUZZMIN_SEARCHMINCLSIMPACT 0.0
#define PARAM_FUZZMAX_SEARCHMINCLSIMPACT 1.0
#define PARAM_HARDMIN_SEARCHMINCLSIMPACT 0.0
#define PARAM_HARDMAX_SEARCHMINCLSIMPACT 1.0

float_ty param_searchImpactMaxFactor;	//The maximum impact of learned things before we disable MP until a new unit is learned.
#define PARAM_DEFAULT_SEARCHIMPACTMAXFACTOR 0.03
#define PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR 0.0
#define PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR 1.0
#define PARAM_HARDMIN_SEARCHIMPACTMAXFACTOR 0.0
#define PARAM_HARDMAX_SEARCHIMPACTMAXFACTOR 1.0

float_ty param_searchMaxMagForSLS;		//The maximum magnetization for a sub formula that is to be checked by SLS.
#define PARAM_DEFAULT_SEARCHMAXMAGFORSLS 0.05
#define PARAM_FUZZMIN_SEARCHMAXMAGFORSLS 0.0
#define PARAM_FUZZMAX_SEARCHMAXMAGFORSLS 1.0
#define PARAM_HARDMIN_SEARCHMAXMAGFORSLS 0.0
#define PARAM_HARDMAX_SEARCHMAXMAGFORSLS 1.0

float_ty param_searchMPWeightModSAT;	//The amount of weight that we remove from a satisfying literal during MPWRL.
#define PARAM_DEFAULT_SEARCHMPWEIGHTMODSAT 0.01
#define PARAM_FUZZMIN_SEARCHMPWEIGHTMODSAT 0.00001
#define PARAM_FUZZMAX_SEARCHMPWEIGHTMODSAT 0.3
#define PARAM_HARDMIN_SEARCHMPWEIGHTMODSAT 0.0
#define PARAM_HARDMAX_SEARCHMPWEIGHTMODSAT 10.0

float_ty param_searchMPWeightModUNS;	//The amount of weight that we add to a non-satisfying literal during MPWRL.
#define PARAM_DEFAULT_SEARCHMPWEIGHTMODUNS 0.01
#define PARAM_FUZZMIN_SEARCHMPWEIGHTMODUNS 0.00001
#define PARAM_FUZZMAX_SEARCHMPWEIGHTMODUNS 0.3
#define PARAM_HARDMIN_SEARCHMPWEIGHTMODUNS 0.0
#define PARAM_HARDMAX_SEARCHMPWEIGHTMODUNS 10.0

int32_t param_searchStrategy;			//The search strategy that is used.
#define PARAM_DEFAULT_SEARCHSTRATEGY 2
#define PARAM_FUZZMIN_SEARCHSTRATEGY SEARCH_STRATEGY_SLS
#define PARAM_FUZZMAX_SEARCHSTRATEGY SEARCH_STRATEGY_EMP


//DETERM


//SPECCER
int32_t param_speccerNNFFirst;			//Whether SPECCER should translate non-CNF formulas to NNF first for CNF translation.
#define PARAM_DEFAULT_SPECCERNNFFIRST 0
#define PARAM_FUZZMIN_SPECCERNNFFIRST 0
#define PARAM_FUZZMAX_SPECCERNNFFIRST 1
#define PARAM_HARDMIN_SPECCERNNFFIRST 0
#define PARAM_HARDMAX_SPECCERNNFFIRST 1

//LOADER


//ATTRIBUTOR


//RANDOMFOREST


//ITEADAPTER


//PRE


//SATELITE


//MP
int32_t param_mpMaxNumIterations;		//Maximum number of iterations in message passing before non-convergence is claimed.
#define PARAM_DEFAULT_MPMAXNUMITERATIONS 1000
#define PARAM_HARDMIN_MPMAXNUMITERATIONS 1
#define PARAM_HARDMAX_MPMAXNUMITERATIONS INT32_MAX

int32_t param_mpMaxNonImpIterations;	//The maximum number of non-improving iterations regarding convergence difference.
#define PARAM_DEFAULT_MPMAXNONIMPITERATIONS 50
#define PARAM_HARDMIN_MPMAXNONIMPITERATIONS 1
#define PARAM_HARDMAX_MPMAXNONIMPITERATIONS 1000

float_ty param_mpMaxMagnetization;		//Maximum allowed magnetization to assume a paramagnetic state.
#define PARAM_DEFAULT_MPMAXMAGNETIZATION 0.01
#define PARAM_HARDMIN_MPMAXMAGNETIZATION 0.0001
#define PARAM_HARDMAX_MPMAXMAGNETIZATION 0.5

float_ty param_mpEpsilon;				//The smallest allowed value before we take it as zero.
#define PARAM_DEFAULT_MPEPSILON 0.0000001
#define PARAM_HARDMIN_MPEPSILON 0.0000001
#define PARAM_HARDMAX_MPEPSILON 0.01

float_ty param_mpRho;					//Controls the interpolation of BP (rho=0) and SP (rho=1).
#define PARAM_DEFAULT_MPRHO 0.98
#define PARAM_FUZZMIN_MPRHO 0.0
#define PARAM_FUZZMAX_MPRHO 1.0
#define PARAM_HARDMIN_MPRHO 0.0
#define PARAM_HARDMAX_MPRHO 1.0

float_ty param_mpSigma;					//Controls the interpolation of non-EM (sigma=0) and full-EM (sigma=1).
#define PARAM_DEFAULT_MPSIGMA 0.001
#define PARAM_FUZZMIN_MPSIGMA 0.0
#define PARAM_FUZZMAX_MPSIGMA 1.0
#define PARAM_HARDMIN_MPSIGMA 0.0
#define PARAM_HARDMAX_MPSIGMA 1.0

float_ty param_mpPhi;					//The force-field strength (Phi=0 no force, +1 strong towards true, -1 strong towards false).
#define PARAM_DEFAULT_MPPHI 0.0
#define PARAM_FUZZMIN_MPPHI -1.0
#define PARAM_FUZZMAX_MPPHI 1.0
#define PARAM_HARDMIN_MPPHI -1.0
#define PARAM_HARDMAX_MPPHI 1.0

float_ty param_mpUpsilon;				//The temperature parameter (Upsilon=0 means no heat, while Upsilon=1 means super hot.)
#define PARAM_DEFAULT_MPUPSILON 0.0
#define PARAM_FUZZMIN_MPUPSILON 0.0
#define PARAM_FUZZMAX_MPUPSILON 1.0
#define PARAM_HARDMIN_MPUPSILON 0.0
#define PARAM_HARDMAX_MPUPSILON 1.0

int32_t param_mpUpdateRule;				//Determines what update rule to use in the next call to MP.
#define PARAM_DEFAULT_MPUPDATERULE 10
#define PARAM_FUZZMIN_MPUPDATERULE 10
#define PARAM_FUZZMAX_MPUPDATERULE 10

float_ty param_mpMaxConvergenceDiff;	//Maximum allowed difference in any message update before convergence is claimed.
#define PARAM_DEFAULT_MPMAXCONVERGENCEDIFF 0.01
#define PARAM_HARDMIN_MPMAXCONVERGENCEDIFF 0.000001
#define PARAM_HARDMAX_MPMAXCONVERGENCEDIFF 0.5
#define PARAM_FUZZMIN_MPMAXCONVERGENCEDIFF 0.000001
#define PARAM_FUZZMAX_MPMAXCONVERGENCEDIFF 0.5

float_ty param_mpWeightNoise;			//A noise value that is either added or subtracted from the literal occurrence weight.
#define PARAM_DEFAULT_MPWEIGHTNOISE 0.0
#define PARAM_HARDMIN_MPWEIGHTNOISE 0.0
#define PARAM_HARDMAX_MPWEIGHTNOISE 1.0
#define PARAM_FUZZMIN_MPWEIGHTNOISE 0.0
#define PARAM_FUZZMAX_MPWEIGHTNOISE 0.001

//SLS
float_ty param_slsMaxFlipsA;			//A parameter to influence the try length.
#define PARAM_DEFAULT_SLSMAXFLIPSA 30.0f
#define PARAM_FUZZMIN_SLSMAXFLIPSA 1.0
#define PARAM_FUZZMAX_SLSMAXFLIPSA 30.0
#define PARAM_HARDMIN_SLSMAXFLIPSA 0.00001
#define PARAM_HARDMAX_SLSMAXFLIPSA LARGEFLOAT

float_ty param_slsMaxFlipsB;			//A parameter to influence the try length.
#define PARAM_DEFAULT_SLSMAXFLIPSB 2.0
#define PARAM_FUZZMIN_SLSMAXFLIPSB 1.0
#define PARAM_FUZZMAX_SLSMAXFLIPSB 10.0
#define PARAM_HARDMIN_SLSMAXFLIPSB 0.00001
#define PARAM_HARDMAX_SLSMAXFLIPSB LARGEFLOAT

int32_t param_slsMaxTries;				//A parameter to influence the maximum number of tries.
#define PARAM_DEFAULT_SLSMAXTRIES 0
#define PARAM_FUZZMIN_SLSMAXTRIES 1
#define PARAM_FUZZMAX_SLSMAXTRIES 5
#define PARAM_HARDMIN_SLSMAXTRIES 0
#define PARAM_HARDMAX_SLSMAXTRIES INT32_MAX

float_ty param_slsNoise;				//A noise value that can be used in many occasions or pick variable rules.
#define PARAM_DEFAULT_SLSNOISE 0.5866f
#define PARAM_FUZZMIN_SLSNOISE 0.1
#define PARAM_FUZZMAX_SLSNOISE 1.0
#define PARAM_HARDMIN_SLSNOISE 0.0
#define PARAM_HARDMAX_SLSNOISE 1.0

float_ty param_slsProbsatCB;			//The brake base for the ProbSAT score computation.
#define PARAM_DEFAULT_SLSPROBSATCB 2.06f
#define PARAM_FUZZMIN_SLSPROBSATCB 2.0
#define PARAM_FUZZMAX_SLSPROBSATCB 7.0
#define PARAM_HARDMIN_SLSPROBSATCB 1.0
#define PARAM_HARDMAX_SLSPROBSATCB LARGEFLOAT

float_ty param_slsProbsatCBShift;		//The shift value for the brake values for the ProbSAT score computation.
#define PARAM_DEFAULT_SLSPROBSATCBSHIFT 0.9f
#define PARAM_FUZZMIN_SLSPROBSATCBSHIFT 0.0
#define PARAM_FUZZMAX_SLSPROBSATCBSHIFT 1.0
#define PARAM_HARDMIN_SLSPROBSATCBSHIFT 0.0
#define PARAM_HARDMAX_SLSPROBSATCBSHIFT 1.0

int32_t param_slsPickAndFlipVarRule;	//Determines what variable picking and flipping implementation to use.
#define PARAM_DEFAULT_SLSPICKANDFLIPVARRULE 1
#define PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE SLS_PICKANDFLIPVARRULE_PROBSATPB
#define PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE SLS_PICKANDFLIPVARRULE_PROBSATEB

int32_t param_slsAssInitRule;			//Determines how to initialize the SLS starting assignment.
#define PARAM_DEFAULT_SLSASSINITRULE 0
#define PARAM_FUZZMIN_SLSASSINITRULE SLS_ASSINITRULE_MINVALUE
#define PARAM_FUZZMAX_SLSASSINITRULE SLS_ASSINITRULE_MAXVALUE

int32_t param_slsTryRestartRule;		//Determines how to initialize the SLS try restarts.
#define PARAM_DEFAULT_SLSTRYRESTARTRULE 0
#define PARAM_FUZZMIN_SLSTRYRESTARTRULE SLS_TRYRESTARTRULE_CONST
#define PARAM_FUZZMAX_SLSTRYRESTARTRULE SLS_TRYRESTARTRULE_EXP

//CDCL
FILE *param_cdclOutput;						//The file containing the CDCL formula in DIMACS CNF INPUT format.

float_ty param_cdclRestartStagMaxFrac;		//The maximum stagnation fraction allowed for doing a restart.
#define PARAM_DEFAULT_CDCLRESTARTSTAGMAXFRAC 0.1
#define PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC 0.0
#define PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC 1.0
#define PARAM_HARDMIN_CDCLRESTARTSTAGMAXFRAC 0.0
#define PARAM_HARDMAX_CDCLRESTARTSTAGMAXFRAC 1.0

int32_t param_cdclRestartInnerouterInit;	//This is the base value for the inner/outer restart series.
#define PARAM_DEFAULT_CDCLRESTARTINNEROUTERINIT 100
#define PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT 50
#define PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT 512
#define PARAM_HARDMIN_CDCLRESTARTINNEROUTERINIT 1
#define PARAM_HARDMAX_CDCLRESTARTINNEROUTERINIT INT32_MAX

float_ty param_cdclRestartInnerouterInc;	//This is the increase factor for inner/outer restarts.
#define PARAM_DEFAULT_CDCLRESTARTINNEROUTERINC 1.1
#define PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC 1.001
#define PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC 1.5
#define PARAM_HARDMIN_CDCLRESTARTINNEROUTERINC 1.0
#define PARAM_HARDMAX_CDCLRESTARTINNEROUTERINC LARGEFLOAT

int32_t param_cdclRestartLubyUnit;			//This is the base value for the Luby restart series.
#define PARAM_DEFAULT_CDCLRESTARTLUBYUNIT 10
#define PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT 1
#define PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT 512
#define PARAM_HARDMIN_CDCLRESTARTLUBYUNIT 1
#define PARAM_HARDMAX_CDCLRESTARTLUBYUNIT INT32_MAX

float_ty param_cdclRestartVarAgiIncBump;	//This is the value that bumps the variable agility increase.
#define PARAM_DEFAULT_CDCLRESTARTVARAGIINCBUMP 1.00010001
#define PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP 1.00001
#define PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP 1.05
#define PARAM_HARDMIN_CDCLRESTARTVARAGIINCBUMP 1.0
#define PARAM_HARDMAX_CDCLRESTARTVARAGIINCBUMP LARGEFLOAT

float_ty param_cdclRestartMaxAgiFrac;		//The fraction of variables with high agility allowed when doing a restart.
#define PARAM_DEFAULT_CDCLRESTARTMAXAGIFRAC 0.20
#define PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC 0.0
#define PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC 1.0
#define PARAM_HARDMIN_CDCLRESTARTMAXAGIFRAC 0.0
#define PARAM_HARDMAX_CDCLRESTARTMAXAGIFRAC 1.0

float_ty param_cdclRestartMaxAgiOutFrac;	//The fraction of variables with high agility allowed when doing an outer restart.
#define PARAM_DEFAULT_CDCLRESTARTMAXAGIOUTFRAC 0.25
#define PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC 0.2
#define PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC 1.0
#define PARAM_HARDMIN_CDCLRESTARTMAXAGIOUTFRAC 0.0
#define PARAM_HARDMAX_CDCLRESTARTMAXAGIOUTFRAC 1.0

float_ty param_cdclSelectVarActIncBump;		//This is the value that bumps the variable activities increase.
#define PARAM_DEFAULT_CDCLSELECTVARACTINCBUMP 1.05
#define PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP 1.0
#define PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP 1.06
#define PARAM_HARDMIN_CDCLSELECTVARACTINCBUMP 1.0
#define PARAM_HARDMAX_CDCLSELECTVARACTINCBUMP LARGEFLOAT

int32_t param_cdclSelectVarActInit;			//Determines how the SVR plug-in initializes the variable activities.
#define PARAM_DEFAULT_CDCLSELECTVARACTINIT 2
#define PARAM_FUZZMIN_CDCLSELECTVARACTINIT CDCL_SELECTVARRULE_VARACTINIT_OCCNUM
#define PARAM_FUZZMAX_CDCLSELECTVARACTINIT CDCL_SELECTVARRULE_VARACTINIT_BINOCC

float_ty param_cdclSelectVarRandProb;		//Determines the probability of SVR plug-in to pick a random variable.
#define PARAM_DEFAULT_CDCLSELECTVARRANDPROB 0.001
#define PARAM_FUZZMIN_CDCLSELECTVARRANDPROB 0.0
#define PARAM_FUZZMAX_CDCLSELECTVARRANDPROB 0.05
#define PARAM_HARDMIN_CDCLSELECTVARRANDPROB 0.0
#define PARAM_HARDMAX_CDCLSELECTVARRANDPROB 1.0

int32_t param_cdclSelectDirMinFlipDist;		//Minimum decision distance for flipping a DL1 decision.
#define PARAM_DEFAULT_CDCLSELECTDIRMINFLIPDIST 1024
#define PARAM_FUZZMIN_CDCLSELECTDIRMINFLIPDIST 100
#define PARAM_FUZZMAX_CDCLSELECTDIRMINFLIPDIST 100000
#define PARAM_HARDMIN_CDCLSELECTDIRMINFLIPDIST 0
#define PARAM_HARDMAX_CDCLSELECTDIRMINFLIPDIST INT32_MAX

int32_t param_cdclSelectDirDirInit;			//Determines how the SDR plug-in initializes the variable directions.
#define PARAM_DEFAULT_CDCLSELECTDIRDIRINIT 4
#define PARAM_FUZZMIN_CDCLSELECTDIRDIRINIT CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST
#define PARAM_FUZZMAX_CDCLSELECTDIRDIRINIT CDCL_SELECTDIRRULE_DIRINIT_BINFAIL

float_ty param_cdclMaintenanceMinDev;  		//If scaled LBD standard deviation is below this value, we pick INACTIVITY instead.
#define PARAM_DEFAULT_CDCLMAINTENANCEMINDEV 0.10
#define PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV 0.05
#define PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV 0.50
#define PARAM_HARDMIN_CDCLMAINTENANCEMINDEV 0.0
#define PARAM_HARDMAX_CDCLMAINTENANCEMINDEV 1.0

float_ty param_cdclMaintenanceMaxAvg;  		//If scaled LBD average is above this value, we pick INACTIVITY instead.
#define PARAM_DEFAULT_CDCLMAINTENANCEMAXAVG 12.0
#define PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG 5.0
#define PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG 20.0
#define PARAM_HARDMIN_CDCLMAINTENANCEMAXAVG 0.0
#define PARAM_HARDMAX_CDCLMAINTENANCEMAXAVG LARGEFLOAT

float_ty param_cdclMaintenanceActIncBump;	//This is the value that bumps the clause activities increase.
#define PARAM_DEFAULT_CDCLMAINTENANCEACTINCBUMP 1.001
#define PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP 1.00001
#define PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP 1.05
#define PARAM_HARDMIN_CDCLMAINTENANCEACTINCBUMP 1.0
#define PARAM_HARDMAX_CDCLMAINTENANCEACTINCBUMP LARGEFLOAT

float_ty param_cdclMaintenanceInitial;  	//This is the fraction of initial clauses that we want to learn before CDBM in INACTIVITY.
#define PARAM_DEFAULT_CDCLMAINTENANCEINITIAL 1.3
#define PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL 1.0
#define PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL 2.0
#define PARAM_HARDMIN_CDCLMAINTENANCEINITIAL 0.0
#define PARAM_HARDMAX_CDCLMAINTENANCEINITIAL LARGEFLOAT

float_ty param_cdclMaintenanceIncrease;  	//This bumps the number of learned clauses before we do the next CDBM in INACTIVITY.
#define PARAM_DEFAULT_CDCLMAINTENANCEINCREASE 1.1
#define PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE 1.01
#define PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE 2.0
#define PARAM_HARDMIN_CDCLMAINTENANCEINCREASE 1.0
#define PARAM_HARDMAX_CDCLMAINTENANCEINCREASE LARGEFLOAT

int32_t param_cdclMaintenanceBase;  		//This is LBD-scheme number of clauses that we want to learn before CDBM.
#define PARAM_DEFAULT_CDCLMAINTENANCEBASE 20000
#define PARAM_FUZZMIN_CDCLMAINTENANCEBASE 10000
#define PARAM_FUZZMAX_CDCLMAINTENANCEBASE 50000
#define PARAM_HARDMIN_CDCLMAINTENANCEBASE 1
#define PARAM_HARDMAX_CDCLMAINTENANCEBASE INT32_MAX

int32_t param_cdclMaintenanceBoost;  		//This bumps the number of learned clauses before we do the next CDBM in LBD.
#define PARAM_DEFAULT_CDCLMAINTENANCEBOOST 20000
#define PARAM_FUZZMIN_CDCLMAINTENANCEBOOST 10000
#define PARAM_FUZZMAX_CDCLMAINTENANCEBOOST 50000
#define PARAM_HARDMIN_CDCLMAINTENANCEBOOST 2
#define PARAM_HARDMAX_CDCLMAINTENANCEBOOST INT32_MAX

int32_t param_cdclIRUNHRounds;				//The number of UNHIDING rounds to perform in a single call to it.
#define PARAM_DEFAULT_CDCLIRUNHROUNDS 10
#define PARAM_FUZZMIN_CDCLIRUNHROUNDS 1
#define PARAM_FUZZMAX_CDCLIRUNHROUNDS 15
#define PARAM_HARDMIN_CDCLIRUNHROUNDS 1
#define PARAM_HARDMAX_CDCLIRUNHROUNDS INT32_MAX

int32_t param_cdclIRTERNARYLitMax;			//Maximum number of resolvents for a literal in TERNARY resolution.
#define PARAM_DEFAULT_CDCLIRTERNARYLITMAX 500
#define PARAM_FUZZMIN_CDCLIRTERNARYLITMAX 20
#define PARAM_FUZZMAX_CDCLIRTERNARYLITMAX 1000
#define PARAM_HARDMIN_CDCLIRTERNARYLITMAX 0
#define PARAM_HARDMAX_CDCLIRTERNARYLITMAX INT32_MAX

int32_t param_cdclIRTERNARYMax;				//Maximum number of resolvents total in a run of TERNARY resolution.
#define PARAM_DEFAULT_CDCLIRTERNARYMAX 1000
#define PARAM_FUZZMIN_CDCLIRTERNARYMAX 100
#define PARAM_FUZZMAX_CDCLIRTERNARYMAX 5000
#define PARAM_HARDMIN_CDCLIRTERNARYMAX 0
#define PARAM_HARDMAX_CDCLIRTERNARYMAX INT32_MAX

int32_t param_cdclIRNIVERMax;				//Maximum number of occurrences to check a variable with NIVER.
#define PARAM_DEFAULT_CDCLIRNIVERMAX 25
#define PARAM_FUZZMIN_CDCLIRNIVERMAX 1
#define PARAM_FUZZMAX_CDCLIRNIVERMAX 50
#define PARAM_HARDMIN_CDCLIRNIVERMAX 0
#define PARAM_HARDMAX_CDCLIRNIVERMAX INT32_MAX

int32_t param_cdclIRGEMaxAONN;				//Maximum number of new resolvents when removing an AND/OR/NAND/NOR gate in IR GE.
#define PARAM_DEFAULT_CDCLIRGEMAXAONN 8
#define PARAM_FUZZMIN_CDCLIRGEMAXAONN 2
#define PARAM_FUZZMAX_CDCLIRGEMAXAONN 50
#define PARAM_HARDMIN_CDCLIRGEMAXAONN 0
#define PARAM_HARDMAX_CDCLIRGEMAXAONN INT32_MAX

int32_t param_cdclIRGEMaxXOR;				//Maximum number of new resolvents when removing an XOR gate in IR GE.
#define PARAM_DEFAULT_CDCLIRGEMAXXOR 10000
#define PARAM_FUZZMIN_CDCLIRGEMAXXOR 2
#define PARAM_FUZZMAX_CDCLIRGEMAXXOR 25000
#define PARAM_HARDMIN_CDCLIRGEMAXXOR 0
#define PARAM_HARDMAX_CDCLIRGEMAXXOR INT32_MAX

int32_t param_cdclIRStrMaxSize;				//Maximum clause size to use clause in strengthening.
#define PARAM_DEFAULT_CDCLIRSTRMAXSIZE 4
#define PARAM_FUZZMIN_CDCLIRSTRMAXSIZE 3
#define PARAM_FUZZMAX_CDCLIRSTRMAXSIZE 6
#define PARAM_HARDMIN_CDCLIRSTRMAXSIZE 0
#define PARAM_HARDMAX_CDCLIRSTRMAXSIZE INT32_MAX

int32_t param_cdclIRNHBRMin;				//Minimum number of implications a literal must have to be used for NHBR.
#define PARAM_DEFAULT_CDCLIRNHBRMIN 10
#define PARAM_FUZZMIN_CDCLIRNHBRMIN 1
#define PARAM_FUZZMAX_CDCLIRNHBRMIN 50
#define PARAM_HARDMIN_CDCLIRNHBRMIN 0
#define PARAM_HARDMAX_CDCLIRNHBRMIN INT32_MAX

float_ty param_cdclIRMinConfDistance;		//This times the number of remaining variables gives the minimum conflict distance.
#define PARAM_DEFAULT_CDCLIRMINCONFDISTANCE 0.0
#define PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE 0.0
#define PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE 3.0
#define PARAM_HARDMIN_CDCLIRMINCONFDISTANCE 0.0
#define PARAM_HARDMAX_CDCLIRMINCONFDISTANCE LARGEFLOAT

int32_t param_cdclSelectVarRule;			//Determines what variable selection implementation to use.
#define PARAM_DEFAULT_CDCLSELECTVARRULE 3
#define PARAM_FUZZMIN_CDCLSELECTVARRULE CDCL_SELECTVARRULE_RRVSIDS
#define PARAM_FUZZMAX_CDCLSELECTVARRULE CDCL_SELECTVARRULE_RRVSIDS

int32_t param_cdclSelectDirRule;			//Determines what variable direction selection implementation to use.
#define PARAM_DEFAULT_CDCLSELECTDIRRULE 3
#define PARAM_FUZZMIN_CDCLSELECTDIRRULE CDCL_SELECTDIRRULE_PHASESAVING
#define PARAM_FUZZMAX_CDCLSELECTDIRRULE CDCL_SELECTDIRRULE_PHASEFLIP

int32_t param_cdclConflictAnalysisRule;		//Determines what conflict analysis implementation to use.
#define PARAM_DEFAULT_CDCLCONFLICTANALYSISRULE 1
#define PARAM_FUZZMIN_CDCLCONFLICTANALYSISRULE CDCL_CONFLICTANALYSISRULE_FIRSTUIP
#define PARAM_FUZZMAX_CDCLCONFLICTANALYSISRULE CDCL_CONFLICTANALYSISRULE_ALLUIP

int32_t param_cdclStrLearnedRule;			//Determines what strengthen learned clauses implementation to use.
#define PARAM_DEFAULT_CDCLSTRLEARNEDRULE 5
#define PARAM_FUZZMIN_CDCLSTRLEARNEDRULE CDCL_STRLEARNEDRULE_LOCAL
#define PARAM_FUZZMAX_CDCLSTRLEARNEDRULE CDCL_STRLEARNEDRULE_LOCALREC

int32_t param_cdclStrOtherRule;				//Determines what strengthen other clauses implementation to use.
#define PARAM_DEFAULT_CDCLSTROTHERRULE 1
#define PARAM_FUZZMIN_CDCLSTROTHERRULE CDCL_STROTHERRULE_NULL
#define PARAM_FUZZMAX_CDCLSTROTHERRULE CDCL_STROTHERRULE_WTWO

int32_t param_cdclComputeBJLRule;			//Determines what back-jump level computation implementation to use.
#define PARAM_DEFAULT_CDCLCOMPUTEBJLRULE 0
#define PARAM_FUZZMIN_CDCLCOMPUTEBJLRULE 0
#define PARAM_FUZZMAX_CDCLCOMPUTEBJLRULE 0

int32_t param_cdclRestartRule;				//Determines what restart implementation to use.
#define PARAM_DEFAULT_CDCLRESTARTRULE 3
#define PARAM_FUZZMIN_CDCLRESTARTRULE CDCL_RESTARTRULE_MINVALUE
#define PARAM_FUZZMAX_CDCLRESTARTRULE CDCL_RESTARTRULE_MAXVALUE

int32_t param_cdclMaintenanceRule;			//Determines what clause database maintenance implementation to use.
#define PARAM_DEFAULT_CDCLMAINTENANCERULE 4
#define PARAM_FUZZMIN_CDCLMAINTENANCERULE CDCL_MAINTENANCERULE_MINVALUE
#define PARAM_FUZZMAX_CDCLMAINTENANCERULE CDCL_MAINTENANCERULE_MAXVALUE

int32_t param_cdclInprocessingRule;			//Determines what in-processing implementation to use.
#define PARAM_DEFAULT_CDCLINPROCESSINGRULE 2
#define PARAM_FUZZMIN_CDCLINPROCESSINGRULE CDCL_INPROCESSINGRULE_MINVALUE
#define PARAM_FUZZMAX_CDCLINPROCESSINGRULE CDCL_INPROCESSINGRULE_MAXVALUE

int32_t param_cdclAbortRule;		 		//Determines what abort rule implementation to use.
#define PARAM_DEFAULT_CDCLABORTRULE 0
#define PARAM_FUZZMIN_CDCLABORTRULE CDCL_ABORTRULE_MINVALUE
#define PARAM_FUZZMAX_CDCLABORTRULE CDCL_ABORTRULE_MAXVALUE


//HANDLERESULT
int32_t param_handleresultCoarsen;		//Coarsens the satisfying assignment by unassigning variables.
#define PARAM_DEFAULT_HANDLERESULTCOARSEN 0
#define PARAM_HARDMIN_HANDLERESULTCOARSEN 0
#define PARAM_HARDMAX_HANDLERESULTCOARSEN 1

//SOLWRITER
int32_t param_solwriterPrintAssignment;	//Set this to 0 if you do not want to output the satisfying assignment if one was found.
#define PARAM_DEFAULT_SOLWRITERPRINTASSIGNMENT 1

//SHUTDOWN


#endif /* PARAMETERS_H_ */
