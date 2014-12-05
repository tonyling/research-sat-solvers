/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RAND_H_
#define RAND_H_

#include "tools.h"

uint32_t rand_randomNumber;		//The current random number.

#ifdef NEWSCHOOLRAND
	//Using the new random number generator based on Galois LFSR. It can produce numbers with up to 31 1s.
	static const uint32_t RANDMAX = 2147483647;					//Equals 2^31 - 1;
	static const float_ty OORANDMAX = 1.0f / 2147483647.0f; 	//Equals 1.0 / (2^31 - 1);
	uint32_t rand_tap;			//The current tap of the Galois LFSR.
	static const uint32_t rand_taps[16] = {
			1426313224u,1074316258u,1913951248u,1887441564u,
			1082143757u,1150431505u,1611067410u,1614813539u,
			2047148069u,1546387844u,1120937136u,1881278092u,
			1342179018u,1090987153u,1092035092u,1837178976u
	};


	inline static void rand_reset(){
		rand_randomNumber = param_seed | 1U;
		if (rand_randomNumber >= RANDMAX) rand_randomNumber = RANDMAX - 1U;
		rand_tap = 1837178976u;
	}

	inline static void rand_init(){
		rand_randomNumber = rand();
		if (rand_randomNumber == 0u) ++rand_randomNumber;
		if (rand_randomNumber >= RANDMAX) rand_randomNumber = RANDMAX - 1U;
		rand_tap = rand_taps[rand() % 16u];
	}

	inline static uint32_t rand_random(){
		rand_randomNumber = (rand_randomNumber >> 1) ^ (-(rand_randomNumber & 1u) & rand_tap);
		return rand_randomNumber;
	}

	inline static uint32_t rand_mod(uint32_t theMod){
		rand_randomNumber = (rand_randomNumber >> 1) ^ (-(rand_randomNumber & 1u) & rand_tap);
		return (rand_randomNumber % theMod);
	}

	inline static uint32_t rand_intMinMax(uint32_t theMin, uint32_t theMax){
		return (rand_mod(theMax - theMin) + theMin);
	}

	inline static float_ty rand_real(float_ty maxValue){
		rand_randomNumber = (rand_randomNumber >> 1) ^ (-(rand_randomNumber & 1u) & rand_tap);
		return (rand_randomNumber * OORANDMAX * maxValue);
	}

	inline static float_ty rand_realOne(){
		rand_randomNumber = (rand_randomNumber >> 1) ^ (-(rand_randomNumber & 1u) & rand_tap);
		return (rand_randomNumber * OORANDMAX);
	}

	inline static float_ty rand_floatMinMax(float_ty theMin, float_ty theMax){
		return ((theMax - theMin)*rand_realOne() + theMin);
	}

	inline static void rand_dispose(){
		//Nothing to do.
	}

	inline static void rand_printConfig_inline(){
		printf("RAND LFSR: rand_randomNumber: %u, rand_tap: %u", rand_randomNumber, rand_tap);
	}

#else

	//Using the old random number generator from C.
	static const uint32_t RANDMAX = RAND_MAX;
	static const float_ty OORANDMAX = 1.0f / ((float_ty)RAND_MAX);
	inline static void rand_reset(){
		rand_randomNumber = param_seed;
	}

	inline static void rand_init(){
		srand(param_seed);
	}

	inline static uint32_t rand_random(){
		return rand();
	}

	inline static uint32_t rand_mod(uint32_t theMod){
		return (rand()%theMod);
	}

	inline static uint32_t rand_intMinMax(uint32_t theMin, uint32_t theMax){
		return (rand_mod(theMax - theMin) + theMin);
	}

	inline static float_ty rand_real(float_ty maxValue){
		return (((float_ty)rand()) * OORANDMAX * maxValue);
	}

	inline static float_ty rand_realOne(){
		return (((float_ty)rand()) * OORANDMAX);
	}

	inline static float_ty rand_floatMinMax(float_ty theMin, float_ty theMax){
		return ((theMax - theMin)*rand_realOne() + theMin);
	}

	inline static void rand_dispose(){
		//Nothing to do.
	}

	inline static void rand_printConfig_inline(){
		printf("RAND C: rand_randomNumber: %u", rand_randomNumber);
	}
#endif

#endif /* RAND_H_ */
