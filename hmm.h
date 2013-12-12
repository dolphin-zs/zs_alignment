/*hidden markov model*/

#ifndef HMM
#define HMM

#include "model12.h"

class hmm : public model12
{
	public:
		map<WordIndex, double> count_jj_jl;
		map<WordIndex, double> count_jl;
		map<WordIndex, double> p_jj_jl;

		hmm(VcbList& el, VcbList& fl, SentenceHandle& sh): model12(el, fl, sh) {}
		hmm(model12& tt): model12(tt) {}
		void hmm_initialization();
		void hmm_algo(int noIterations);
		void hmm_print_alignment(ofstream& of);
};

#endif
