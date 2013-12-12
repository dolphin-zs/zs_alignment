/*hmm.cpp file*/

#include "hmm.h"
#include "Array2.h"
#include <set>
#include <time.h>
#include <cstdio>

void hmm::hmm_initialization(){
    WordIndex i, j, jj, l, m;	
	SentPair sent;
	cout<<"start hidden markov model initialization......"<<endl;
	sHander.new_start();
	set<WordIndex> l_set;
	while(sHander.getNextSentence(sent)){
		vector<WordIndex>& es = sent.esent;
		vector<WordIndex>& fs = sent.fsent;
		l = es.size() - 1; 
		m = fs.size() - 1;
		l_set.insert(l);
		for(i=1;i<=m-1;i++){
			for(j=0;j<=l;j++){
				//map<WordIndex, double>::iterator it1 = count_jl.find(j*G1+l);
				//if(it1 != count_jl.end())
				//	count_jl[j*G1+l] += q_jilm[j*G3+i*G2+l*G1+m];
				//else
				//	count_jl[j*G1+l] = q_jilm[j*G3+i*G2+l*G1+m];
					count_jl[j*G1+l] += q_jilm[j*G3+i*G2+l*G1+m];
				for(jj=0;jj<=l;jj++){
					//map<WordIndex, double>::iterator it2 = count_jj_jl.find(jj*G2+j*G1+l);
					//if(it2 != count_jj_jl.end())
					//	count_jj_jl[jj*G2+j*G1+l] += q_jilm[j*G3+i*G2+l*G1+m]*q_jilm[jj*G3+(i+1)*G2+l*G1+m];
					//else
					//	count_jj_jl[jj*G2+j*G1+l] = q_jilm[j*G3+i*G2+l*G1+m]*q_jilm[jj*G3+(i+1)*G2+l*G1+m];
					count_jj_jl[jj*G2+j*G1+l] += q_jilm[j*G3+i*G2+l*G1+m]*q_jilm[jj*G3+(i+1)*G2+l*G1+m];
				}
			}
		}
	}/*end of while*/

	for(set<WordIndex>::iterator it_l_set = l_set.begin();it_l_set != l_set.end();it_l_set++){
		l = *it_l_set;
        for(j=0;j<=l;j++){
			for(jj=0;jj<=l;jj++){
					p_jj_jl[jj*G2+j*G1+l] = count_jj_jl[jj*G2+j*G1+l] / count_jl[j*G1+l];
			}
		}
	}
	/*sHander.new_start();
	while(sHander.getNextSentence(sent)){
		vector<WordIndex>& es = sent.esent;
		vector<WordIndex>& fs = sent.fsent;
		l = es.size() - 1; 
		m = fs.size() - 1;
		for(j=0;j<=l;j++){
			for(jj=0;jj<=l;jj++){
				map<WordIndex, double>::iterator it0 = p_jj_jl.find(jj*G2+j*G1+l);
				if(it0 == p_jj_jl.end())
					p_jj_jl[jj*G2+j*G1+l] = count_jj_jl[jj*G2+j*G1+l] / count_jl[j*G1+l];
			}
		}
	}*/

}

void hmm::hmm_algo(int noIterations){
	WordIndex i, j, jj, l, m;
	SentPair sent;

	time_t a_time1, a_time2, b_time1, b_time2, y_time1, y_time2, k_time1, k_time2, d_time1, d_time2, e_time1, e_time2;
	double a_time, b_time, y_time, k_time, d_time, e_time;

	for(int it=1;it <= noIterations; it++){
		cout<<"hidden markov iteration ("<<it<<")"<<endl;
		cout<<"---------------------------------------"<<endl;

		//Initialization
		cout<<"...........Initializing..........."<<endl;
		sHander.new_start();
		while(sHander.getNextSentence(sent)){
			vector<WordIndex>& es = sent.esent;
			vector<WordIndex>& fs = sent.fsent;
			l = es.size() - 1;
			m = fs.size() - 1;
			for(j=0;j <= l;j++){
				count_e[es[j]] = 0;
				count_jl[j*G1+l] = 0;
				for(i=1;i <= m;i++)
					cal_ef[WordPairIds(es[j], fs[i])].count = 0;
				for(jj=0;jj <= l;jj++)
					count_jj_jl[jj*G2+j*G1+l] = 0;
			}
		}

		a_time = 0;b_time = 0;y_time = 0;k_time = 0;d_time = 0;e_time = 0;
		//backward-forward learning
		cout<<"...........backward-forward learning..........."<<endl;
		sHander.new_start();
		while(sHander.getNextSentence(sent)){
			vector<WordIndex>& es = sent.esent;
			vector<WordIndex>& fs = sent.fsent;
			l = es.size() - 1;
			m = fs.size() - 1;
			double uniform = 1.0/(double)(l+1);
			double temp_i, temp_max;

			time(&a_time1);
			//learning alpha parameters
			array2<double> alpha(m+1, l+1);
			for(j=0;j <= l;j++)
				alpha(1,j) = uniform * cal_ef[WordPairIds(es[j], fs[1])].prob;
			for(i=1;i <= m-1;i++){
				for(jj=0;jj <= l;jj++){
					temp_max = alpha(i, 0) * p_jj_jl[jj*G2+0*G1+l];
					for(j=1;j <= l;j++){
						temp_i = alpha(i, j) * p_jj_jl[jj*G2+j*G1+l];
						if(temp_i > temp_max)
							temp_max = temp_i;
					}
					alpha(i+1, jj) = temp_max * cal_ef[WordPairIds(es[jj], fs[i+1])].prob;
				}
			}
			time(&a_time2);
			a_time += difftime(a_time2, a_time1);

			time(&b_time1);
			//learning beta parameters
			array2<double> beta(m+1, l+1);
			for(j=0;j <= l;j++)
				beta(m, j) = 1.0;
			for(i=m;i >= 2;i--){
				for(j=0;j <= l;j++){
					temp_max = beta(i, 0) * cal_ef[WordPairIds(es[0], fs[i])].prob * p_jj_jl[0*G2+j*G1+l];
					for(jj=1;jj <= l;jj++){
						temp_i = beta(i, jj) * cal_ef[WordPairIds(es[jj], fs[i])].prob * p_jj_jl[jj*G2+j*G1+l];
						if(temp_i > temp_max)
							temp_max = temp_i;
					}
					beta(i-1, j) = temp_max;
				}
			}
			time(&b_time2);
			b_time += difftime(b_time2, b_time1);
			
			time(&y_time1);
			//learning yita parameters
			array2<double> yita(m+1, l+1);
			for(i=1;i <= m;i++){
			    double sum_yita = 0.0;
				for(j=0;j <= l;j++)
					sum_yita += alpha(i, j) * beta(i, j);
				for(j=0;j <= l;j++)
					yita(i, j) = alpha(i, j) * beta(i, j) / sum_yita;
			}
			time(&y_time2);
			y_time += difftime(y_time2, y_time1);

			time(&k_time1);
			//learning kesi parameters
			map<WordIndex, double> kesi;
			for(i=1;i <= m-1;i++){
				double sum_kesi = 0.0;
				for(j=0;j <= l;j++)
					for(jj=0;jj <= l;jj++)
						sum_kesi += alpha(i, j) * p_jj_jl[jj*G2+j*G1+l] * cal_ef[WordPairIds(es[jj], fs[i+1])].prob * beta(i+1, jj);
				for(j=0;j <= l;j++)
					for(jj=0;jj <= l;jj++)
						kesi[i*G2+j*G1+jj] = alpha(i, j) * p_jj_jl[jj*G2+j*G1+l] * cal_ef[WordPairIds(es[jj], fs[i+1])].prob * beta(i+1, jj) / sum_kesi; 
			}
			time(&k_time2);
			k_time += difftime(k_time2, k_time1);

			time(&d_time1);
			//calculate d_jj_jl, d_ij;
			array2<double> d_jj_jl(l+1, l+1);
			for(j=0;j <= l;j++){
				for(jj=0;jj <= l;jj++){
					double sum_nume = 0, sum_deno = 0;
					for(i=1;i <= m-1;i++){
						sum_nume += kesi[i*G2+j*G1+jj];
						sum_deno += yita(i, j);
					}
					d_jj_jl(jj, j) = sum_nume / sum_deno;
				}
			}
			array2<double> d_ij(m+1, l+1, 0.0);
			for(j=0;j <= l;j++){
				double sum_deno = 0;
				for(i=1;i <= m;i++)
					sum_deno += yita(i, j);
				for(i=1;i <= m;i++)
					d_ij(i, j) = yita(i, j) / sum_deno;
			}
			time(&d_time2);
			d_time += difftime(d_time2, d_time1);
			
			time(&e_time1);
			//em algorithm calculation count
			for(j=0;j <= l;j++){
				for(jj=0;jj <= l;jj++){
					count_jj_jl[jj*G2+j*G1+l] += d_jj_jl(jj, j);
					count_jl[j*G1+l] += d_jj_jl(jj, j);
				}
			}
			for(i=1;i <= m;i++){
				for(j=0;j <= l;j++){
					cal_ef[WordPairIds(es[j], fs[i])].count += d_ij(i, j);
					count_e[es[j]] += d_ij(i, j);
				}
			}
			time(&e_time2);
			e_time += difftime(e_time2, e_time1);
		}//end of backward-forward learning
		printf("alpha time:%.4fs beta time:%.4fs yita time:%.4fs kesi time:%.4fs d time:%.4fs em time:%.4fs", a_time, b_time, y_time, k_time, d_time, e_time);

		//calculate new probability cal_ef and p_jj_jl
		cout<<"...........calculate new probability..........."<<endl;
		sHander.new_start();
		while(sHander.getNextSentence(sent)){
			vector<WordIndex>& es = sent.esent;
			vector<WordIndex>& fs = sent.fsent;
			l = es.size() - 1;
			m = fs.size() - 1;
			for(j=0;j <= l;j++){
				for(i=1;i <= m;i++)
					cal_ef[WordPairIds(es[j], fs[i])].prob = cal_ef[WordPairIds(es[j], fs[i])].count / count_e[es[j]]; 
				for(jj=0;jj <= l;jj++)
					p_jj_jl[jj*G2+j*G1+l] = count_jj_jl[jj*G2+j*G1+l] / count_jl[j*G1+l]; 
			}
		}//end of update probability cal_ed and p_jj_jl
		
	}//end of Iterations

}//end of hmm_algo

void hmm::hmm_print_alignment(ofstream& of){
    WordIndex i, j, l, m, jj;
	SentPair sent;
	double temp, temp_yita;
	PositionIndex temp_index;
	sHander.new_start();
	while(sHander.getNextSentence(sent)){
		vector<PositionIndex> al_ij;
		al_ij.clear();
		al_ij.push_back(0);
		vector<WordIndex>& es = sent.esent;
		vector<WordIndex>& fs = sent.fsent;
		l = es.size() - 1;
		m = fs.size() - 1;
		
        double uniform = 1.0/(double)(l+1);
		double temp_i, temp_max;

		//learning alpha parameters
		array2<double> alpha(m+1, l+1);
		for(j=0;j <= l;j++)
			alpha(1,j) = uniform * cal_ef[WordPairIds(es[j], fs[1])].prob;
		for(i=1;i <= m-1;i++){
			for(jj=0;jj <= l;jj++){
				temp_max = alpha(i, 0) * p_jj_jl[jj*G2+0*G1+l];
				for(j=1;j <= l;j++){
					temp_i = alpha(i, j) * p_jj_jl[jj*G2+j*G1+l];
					if(temp_i > temp_max)
						temp_max = temp_i;
				}
				alpha(i+1, jj) = temp_max * cal_ef[WordPairIds(es[jj], fs[i+1])].prob;
			}
		}

		//learning beta parameters
		array2<double> beta(m+1, l+1);
		for(j=0;j <= l;j++)
			beta(m, j) = 1.0;
		for(i=m;i >= 2;i--){
			for(j=0;j <= l;j++){
				temp_max = beta(i, 0) * cal_ef[WordPairIds(es[0], fs[i])].prob * p_jj_jl[0*G2+j*G1+l];
				for(jj=1;jj <= l;jj++){
					temp_i = beta(i, jj) * cal_ef[WordPairIds(es[jj], fs[i])].prob * p_jj_jl[jj*G2+j*G1+l];
					if(temp_i > temp_max)
						temp_max = temp_i;
				}
				beta(i-1, j) = temp_max;
			}
		}
		
		//learning yita parameters
		array2<double> yita(m+1, l+1);
		for(i=1;i <= m;i++){
		    double sum_yita = 0.0;
			for(j=0;j <= l;j++)
				sum_yita += alpha(i, j) * beta(i, j);
			for(j=0;j <= l;j++)
				yita(i, j) = alpha(i, j) * beta(i, j) / sum_yita;
		}

		for(i=1;i <= m;i++){
			temp = 0.0;
			for(j=0;j <= l;j++){
				temp_yita = yita(i, j);
				if(temp_yita > temp){
					temp = temp_yita;
					temp_index = j;
				}
			}
			al_ij.push_back(temp_index);
		}

		of<<"#sentence pair ("<<sent.sentenceNo<<") f sentence length "<<m<<", e sentence length "<<l<<"\n";
		for(i=1;i <= m;i++)
			of<<FList[fs[i]]<<" ";
		of<<"\n";
		for(j=0;j <= l;j++){
			of<<EList[es[j]]<<" ({ ";
			for(i=1;i <= m;i++){
				if(al_ij[i] == j)
					of<<i<<" ";
			}
			of<<"}) ";
		}
		of<<endl;
	}
}
