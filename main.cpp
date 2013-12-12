#undef  __DEPRECATED //neglect the warnings in using hash_map

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cstdlib>
#include<limits.h>
#include<utility>

#include "gl_defs.h"
#include "vocab.h"
#include "sentence.h"
#include "model12.h"
#include "hmm.h"

using namespace std;


int main(int argc, char **argv)
{
	if(argc < 5){
		cout<<"HELP: Please call "<<argv[0]<<" in this format: "<<argv[0]<<" source.vcb target.vcb source_target.snt source_target.cooc"<<endl;
		exit(1);
	}

	VcbList evcb(argv[1]);
	VcbList fvcb(argv[2]);
	fvcb.readVocalList();
	evcb.readVocalList();
	SentenceHandle sentcorpus(argv[3]);
	//model12 test(evcb, fvcb, sentcorpus); 
	hmm htest(evcb, fvcb, sentcorpus); 
	htest.initialization();
	htest.em_algo1(5);
	ofstream out_tef1("t1");
	htest.print_tfe(argv[4],out_tef1);
	//htest.em_algo2(5);
	//ofstream out_ref2("t2");
	//htest.print_tfe(argv[4],out_ref2);
	ofstream out_alignment("alignment");
	htest.print_align(out_alignment);
	//hmm hmmtest(evcb, fvcb, sentcorpus);
	htest.hmm_initialization();
	htest.hmm_algo(5);
	ofstream out_hmmalign("hmm_alignment");
	htest.hmm_print_alignment(out_hmmalign);

	return 0;
}




