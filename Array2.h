/*array2.h file*/

#ifndef ARRAY_2
#define ARRAY_2

#include <iostream>
#include <vector>
#include <algorithm> // using fill(,,)
#include <assert.h>
using namespace std;

typedef unsigned int Index;

template< class T, class Y=vector<T> > class array2
{
	public:
		Y p;
		Index h1, h2;

		array2(Index _h1, Index _h2): p(_h1*_h2), h1(_h1), h2(_h2) {};
		array2(Index _h1, Index _h2, const T& _init): p(_h1*_h2, _init), h1(_h1), h2(_h2) {};
		array2(): h1(0), h2(0) {};

		inline T& operator () (Index i, Index j){
			assert(i<h1);assert(j<h2);
			return p[i*h2+j];
		}
		inline const T& operator () (Index i, Index j) const{
			assert(i<h1);assert(j<h2);
			return p[i*h2+j];
		}

		inline T getval(Index i, Index j){
			assert(i<h1);assert(j<h2);
			return p[i*h2+j];
		}
		inline const T getval(Index i, Index j) const{
			assert(i<h1);assert(j<h2);
			return p[i*h2+j];
		}

		inline void setval(Index i, Index j, T x){
			assert(i<h1);assert(j<h2);
			p[i*h2+j] = x;
		}

		inline Index getlen1() const{ return h1; }
		inline Index getlen2() const{ return h2; }

		inline T* begin(){
			if(h1==0 || h2==0)
				return 0;
			return &(p[0]);
		}
		inline const T* begin() const{
			if(h1==0 || h2==0)
				return 0;
			return &(p[0]);
		}

		inline T* end(){
			if(h1==0 || h2==0)
				return 0;
			return &(p[0]) + p.size();
		}
		inline const T* end() const{
			if(h1==0 || h2==0)
				return 0;
			return &(p[0]) + p.size();
		}
		 
		inline void resize(Index a, Index b){
			if(h1!=a || h2!=b){
				h1 = a;
				h2 = b;
				p.resize(h1*h2);
			}
		}
		inline void resize(Index a, Index b, const T& x){
			if(h1!=a || h2!=b){
				h1 = a;
				h2 = b;
				p.resize(h1*h2);
				fill(p.begin(), p.end(), x);
			}
		}
		
};

#endif
