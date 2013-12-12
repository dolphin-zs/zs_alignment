/*test Array.h*/

#include "Array2.h"

void print1(const array2<double>& kk){
	for(int i=0;i<kk.getlen1();i++){
		for(int j=0;j<kk.getlen2();j++)
			cout<<kk(i,j)<<" ";
		cout<<endl;
	}
}

void print2(const array2<double>& kk){
	for(const double* p=kk.begin();p!=kk.end();p++)
		cout<<*p<<" ";
	cout<<endl;
}
	
int main(){
	array2<double> a1(3,3);
	array2<double> a2(3,3, 2.0);
	print1(a1);
	print2(a1);
	a2.setval(1,1,9.0);
	print1(a2);
	print2(a2);

	return 0;
}
	
