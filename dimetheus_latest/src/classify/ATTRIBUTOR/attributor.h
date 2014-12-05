/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ATTRIBUTOR_H_
#define ATTRIBUTOR_H_

#include "../classify.h"

#define ATTRIBUTOR_VERSION_MA 2
#define ATTRIBUTOR_VERSION_MI 125
#define ATTRIBUTOR_VERSION_NAME "ATTRIBUTOR"

//GLOBAL data-structures for ATTRIBUTOR.
int32_t attributor_returnCode;				//The return-code of the call to the attribute computation component.
float_ty attributor_timeStartForAttSet;		//The time in seconds for when the computation for an attribute set started.

float_ty* attributor_prioQa;				//The first float_ty element priority queue.
uint32_t attributor_prioQa_size;			//The total size it has.
uint32_t attributor_prioQa_leaf;			//At what position its leaf is.
float_ty* attributor_prioQb;				//The second float_ty element priority queue.
uint32_t attributor_prioQb_size;			//The total size it has.
uint32_t attributor_prioQb_leaf;			//At what position its leaf is.

typedef struct {							//This structure represents a single adjacency list.
	int32_t numAvail;						//The number of element memory positions available in the list.
	int32_t numUsed;						//The number of element memory positions used in the list.
	int32_t* elts;							//The element list.
} attributor_adjacencyList;

typedef struct {							//This structure represents a set of adjacency lists, i.e. a graph.
	int32_t numListsAvail;					//The number of list memory positions available.
	int32_t numListsUsed;					//The number of list memory positions used.
	attributor_adjacencyList** lists;		//The adjacency lists list.
} attributor_adjacencyListSet;

attributor_adjacencyListSet* attributor_graph;	//A graph represented as list of adjacency lists.
#define ATTRIBUTOR_GRAPH_GET_NUMLISTS_AVAIL() 			( attributor_graph->numListsAvail )
#define ATTRIBUTOR_GRAPH_SET_NUMLISTS_AVAIL( __d )		( attributor_graph->numListsAvail = __d )

#define ATTRIBUTOR_GRAPH_GET_NUMLISTS_USED()  			( attributor_graph->numListsUsed )
#define ATTRIBUTOR_GRAPH_SET_NUMLISTS_USED( __d )		( attributor_graph->numListsUsed = __d )

#define ATTRIBUTOR_GRAPH_GET_LISTS()					( attributor_graph->lists )
#define ATTRIBUTOR_GRAPH_SET_LISTS( __d )				( attributor_graph->lists = __d )

#define ATTRIBUTOR_GRAPH_ADD_LIST( __s ) {\
	attributor_graph->lists[attributor_graph->numListsUsed] = malloc(sizeof(attributor_adjacencyList)); \
	attributor_graph->lists[attributor_graph->numListsUsed]->elts = malloc(sizeof(int32_t)*(__s +1)); \
	attributor_graph->lists[attributor_graph->numListsUsed]->numUsed = 0; \
	attributor_graph->lists[attributor_graph->numListsUsed]->numAvail = __s; \
	if (++attributor_graph->numListsUsed == attributor_graph->numListsAvail) { \
		attributor_graph->numListsAvail += 1; \
		attributor_graph->lists = realloc(attributor_graph->lists, sizeof(attributor_adjacencyList*) * attributor_graph->numListsAvail); \
		attributor_graph->lists[attributor_graph->numListsUsed] = NULL; \
	} \
}

#define ATTRIBUTOR_GRAHP_ENSURE_LIST_SIZE( __size ) { \
	while (attributor_graph->numListsUsed < __size +1 ) { \
		ATTRIBUTOR_GRAPH_ADD_LIST( BLOCKSIZE ); \
	} \
}

#define ATTRIBUTOR_GRAPH_GET_LIST_NUM( __i )					( attributor_graph->lists[__i] )
#define ATTRIBUTOR_GRAPH_SET_LIST_NUM( __i, __d )				( attributor_grahp->lists[__i] = __d )

#define ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMAVAIL( __i )			( attributor_graph->lists[__i]->numAvail )
#define ATTRIBUTOR_GRAPH_SET_LIST_NUM_NUMAVAIL( __i, __d )		( attributor_graph->lists[__i]->numAvail = __d )

#define ATTRIBUTOR_GRAPH_GET_LIST_NUM_NUMUSED( __i )			( attributor_graph->lists[__i]->numUsed )
#define ATTRIBUTOR_GRAPH_SET_LIST_NUM_NUMUSED( __i, __d )		( attributor_graph->lists[__i]->numUsed = __d )

#define ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTS( __i )				( attributor_graph->lists[__i]->elts )
#define ATTRIBUTOR_GRAPH_SET_LIST_NUM_ELTS( __i, __d )			( attributor_graph->lists[__i]->elts = __d )

#define ATTRIBUTOR_GRAPH_GET_LIST_NUM_ELTNUM( __i, __j )		( attributor_graph->lists[__i]->elts[__j] )
#define ATTRIBUTOR_GRAPH_SET_LIST_NUM_ELTNUM( __i, __j, __d )	( attributor_graph->lists[__i]->elts[__j] = __d )

#define ATTRIBUTOR_GRAPH_ADD_LIST_NUM_ELT( __i , __d ) {\
	attributor_graph->lists[__i]->elts[attributor_graph->lists[__i]->numUsed] = __d; \
	if (++attributor_graph->lists[__i]->numUsed == attributor_graph->lists[__i]->numAvail) { \
		attributor_graph->lists[__i]->numAvail += BLOCKSIZE; \
		attributor_graph->lists[__i]->elts = realloc(attributor_graph->lists[__i]->elts, sizeof(int32_t)*attributor_graph->lists[__i]->numAvail); \
	} \
}

#define ATTRIBUTOR_GRAPH_RESET_LIST_NUM( __i )					( attributor_graph->lists[__i]->numUsed = 0 )

#define ATTRIBUTOR_GRAPH_RESET_ALL_LISTS( __counter ) { \
	for (__counter = 0; __counter < attributor_graph->numListsUsed; ++__counter) { \
		ATTRIBUTOR_GRAPH_RESET_LIST_NUM( __counter ); \
	} \
}

#define ATTRIBUTOR_GRAPH_RESET_LIST_SIZE( __i ) {\
	attributor_graph->lists[__i]->numUsed = 0;\
	attributor_graph->lists[__i]->numAvail = BLOCKSIZE;\
	attributor_graph->lists[__i]->elts = realloc(attributor_graph->lists[__i]->elts, sizeof(int32_t)*attributor_graph->lists[__i]->numAvail);\
}

literal*** attributor_ELS_classes;			//Equivalence classes of literals found during ELS.
uint32_t attributor_ELS_classes_avail;		//The number of ELS classes that have been initialized.
uint32_t attributor_ELS_classes_used;		//The current number of ELS classes used.
uint32_t* attributor_ELS_classes_memAvail;	//The size of each used class.
uint32_t* attributor_ELS_classes_memUsed;	//The amount of elements for each class.

//The following macro enlarges the amount of ELS classes available, if necessary.
#define ATTRIBUTOR_ENLARGE_ELS_CLASSES_IF_NECESSARY( __counter ) {\
		if (attributor_ELS_classes_used == attributor_ELS_classes_avail){\
			__counter = attributor_ELS_classes_avail;\
			attributor_ELS_classes_avail += BLOCKSIZE;\
			attributor_ELS_classes 			= realloc(attributor_ELS_classes, sizeof(literal**) * attributor_ELS_classes_avail);\
			attributor_ELS_classes_memUsed 	= realloc(attributor_ELS_classes_memUsed, sizeof(uint32_t) * attributor_ELS_classes_avail);\
			attributor_ELS_classes_memAvail 	= realloc(attributor_ELS_classes_memAvail, sizeof(uint32_t) * attributor_ELS_classes_avail);\
			while (__counter < attributor_ELS_classes_avail){\
				attributor_ELS_classes[__counter] = malloc(sizeof(literal*) * BLOCKSIZE);\
				attributor_ELS_classes_memAvail[__counter] = BLOCKSIZE;\
				attributor_ELS_classes_memUsed[__counter] = 0;\
				++__counter;\
			}\
		}\
}

//The following macro adds a literal to the current equivalence class and enlarges its size, if necessary.
#define ATTRIBUTOR_ADD_LIT_TO_CURRENT_ELS_CLASS(__l) {\
		attributor_ELS_classes[attributor_ELS_classes_used][attributor_ELS_classes_memUsed[attributor_ELS_classes_used]++] = __l;\
		if (attributor_ELS_classes_memUsed[attributor_ELS_classes_used] == attributor_ELS_classes_memAvail[attributor_ELS_classes_used]){\
			attributor_ELS_classes_memAvail[attributor_ELS_classes_used] *= 2;\
			attributor_ELS_classes[attributor_ELS_classes_used] = realloc(attributor_ELS_classes[attributor_ELS_classes_used], \
								sizeof(literal*) * attributor_ELS_classes_memAvail[attributor_ELS_classes_used]);\
		}\
}

//The following macro computes the time that has elapsed for computing the current attribute set.
#define ATTRIBUTOR_GET_TIME_IN_SEC() 			((float_ty)clock())/((float_ty)CLOCKS_PER_SEC)
#define ATTRIBUTOR_SET_START_TIME_IN_SEC()		( attributor_timeStartForAttSet = ATTRIBUTOR_GET_TIME_IN_SEC() )
#define ATTRIBUTOR_GET_ELAPSED_TIME_IN_SEC() 	( ATTRIBUTOR_GET_TIME_IN_SEC() - attributor_timeStartForAttSet )
#define ATTRIBUTOR_IS_TIME_ELAPSED()			( ATTRIBUTOR_GET_ELAPSED_TIME_IN_SEC() > param_classifyAttSetTime )

//Methods exclusively accessible to the ATTRIBUTOR module (INTERN).
void attributor_intern_prioQa_sinkRoot();			//To sink down the root element as far as possible.
void attributor_intern_prioQa_raiseLeaf();			//To raise the leaf as far as possible.
void attributor_intern_prioQa_add(float_ty elt);	//The class priority queue add element.
void attributor_intern_prioQa_flush();				//To flush the priority queue.
void attributor_intern_prioQa_resize(uint32_t size);//To re-size the priority to the given value. The leaf will be reset.
float_ty attributor_intern_prioQa_dequeueBest();	//To get the element with most priority (largest value).

void attributor_intern_prioQa_computeDescription(float_ty, float_ty*, float_ty*, float_ty*, float_ty*, float_ty*,
		float_ty*, float_ty*, float_ty*);//Computes for the current priority queue (with the given mean value) the median,
										//standard deviation and entropy (Shannon, Renyi, and Minimum), and PGF values.
										//It ERASES the priority queue doing that.

void attributor_intern_prioQa_computeQuantiles(float_ty,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*);
										//It computes the quantiles for 5, 10, 25, 50, 75, 90, and 95 percent for the
										//first queue. The very first value is the total mass collected in the queue.

void attributor_intern_prioQb_sinkRoot();			//To sink down the root element as far as possible.
void attributor_intern_prioQb_raiseLeaf();			//To raise the leaf as far as possible.
void attributor_intern_prioQb_add(float_ty elt);	//The class priority queue add element.
void attributor_intern_prioQb_flush();				//To flush the priority queue.
void attributor_intern_prioQb_resize(uint32_t size);//To re-size the priority to the given value. The leaf will be reset.
float_ty attributor_intern_prioQb_dequeueBest();	//To get the element with most priority (largest value).

void attributor_intern_prioQb_computeDescription(float_ty, float_ty*, float_ty*, float_ty*, float_ty*, float_ty*,
		float_ty*, float_ty*, float_ty*);//Computes for the current priority queue (with the given mean value) the median,
										//standard deviation and entropy (Shannon, Renyi, and Minimum), and PGF values.
										//It ERASES the priority queue doing that.

void attributor_intern_prioQb_computeQuantiles(float_ty,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*,float_ty*);
										//It computes the quantiles for 5, 10, 25, 50, 75, 90, and 95 percent for the
										//second queue. The very first value is the total mass collected in the queue.

//The following deals with the non-recursive depth first search stack implementation.
typedef struct {
	uint32_t numChildren;						//The number of children not yet visited for this node.
	uint32_t node;								//The node in the graph.
	uint32_t parent;							//A node we came from to put this on the stack.
	uint32_t depth;								//That the graph node will have, if it is visited via this stack node.
	uint32_t reach;								//Is a local variable to store the current reach back level.
} nrdfsStackNode;

nrdfsStackNode** attributor_nrdfsStack;				//The stack used for simulating the recursive depth first search.
uint32_t attributor_nrdfsStack_size;				//What is the current size of the stack (number of allocated elements).
uint32_t attributor_nrdfsStack_leaf;				//What is the current position to add new elements to the stack.

void attributor_intern_nrdfsStack_push(nrdfsStackNode*);//To push a new element on the stack. May increase the stack-size.
nrdfsStackNode* attributor_intern_nrdfsStack_pop();	//To read and remove the topmost element from the stack.
nrdfsStackNode* attributor_intern_nrdfsStack_read();//To just read the topmost element from the stack.
void attributor_intern_nrdfsStack_remove();			//To just remove the top element from the stack.
void attributor_intern_nrdfsStack_resize(uint32_t);	//To resize the stack to the given size (will be increased by 2).
void attributor_intern_nrdfsStack_flush();			//To flush all elements from the stack (just resets the leaf to 1).

void attributor_intern_compute_BASIC_allAttributes();//Computes several basic properties.
void attributor_intern_compute_VCG_allAttributes();  //Computes the variable-clause graph variable node degree attributes.
void attributor_intern_compute_VG_components(int32_t);//Computes the component information of the graph given in the
												//attributor_graph. It assumes that the elements are variable IDs.
float_ty attributor_intern_compute_VG_articulationPoints(int32_t);//Computes the number of articulation points in the graph. It
												//assumes that the elements are variable IDs.
void attributor_intern_compute_VG_allAttributes();	//Computes the variable graph attributes.
void attributor_intern_compute_CG_allAttributes();	//Computes the clause graph attributes.
void attributor_intern_compute_B_allAttributes();	//Computes the balance attributes.
void attributor_intern_compute_HP_allAttributes();	//Computes the Horn proximity attributes.
void attributor_intern_compute_BIG_components(int32_t);//Computes the component information of the binary implication graph
												//given in the attributor_graph. It assumes that the elements are variable IDs.
float_ty attributor_intern_compute_BIG_articulationPoints(int32_t);//Computes the number of articulation points in the graph.
												//It assumes that the elements are variable IDs.
void attributor_intern_compute_BIG_allAttributes();	//Computes the binary implication graph attributes.
void attributor_intern_compute_UP_allAttributes();	//Computes the unit propagation attributes.
void attributor_intern_compute_RW_allAttributes();	//Computes the recursive weight attributes.
uint32_t attributor_intern_compute_ELS_strongConnect(literal*,uint32_t);//Performs Tarjan's algorithm to detect components.
void attributor_intern_compute_ELS_allAttributes();	//Computes the equivalent literal attributes.

void attributor_intern_computeGenericAttributes();	//Computes all attributes for the generic domain (all of them).
void attributor_intern_computeRandomAttributes();	//Computes all attributes for the random domain (just a small subset).
void attributor_intern_computeCraftedAttributes();	//Computes all attributes for the generic domain (all of them).
void attributor_intern_computeApplicationAttributes();//Computes all attributes for the generic domain (all of them).

//PLUGINS for ATTRIBUTOR.
//None.

//Methods PROVIDED by this module (EXTERN).
void attributor_extern_printVersion();			//Just print the version information of this module.
void attributor_extern_computeAttributes();		//Performs the attribute computations for all attributes in the current domain.

//Methods to initialize this module (called by the one who wants to use the provided method).
void attributor_resetModule();					//To reset the ATTRIBUTOR module.
void attributor_initModule();					//To initialize all its data structures.
void attributor_disposeModule();				//To dispose all its data structures.

#endif /* ATTRIBUTOR_H_ */
