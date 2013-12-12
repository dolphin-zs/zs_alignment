#include<iostream>
#include<map>

using namespace std;

int main(){
	map<int, double> he;
	he[0] = 0.0;
	he[4] = 4.4;
	he[8] = 8.8;
	he[9] = 9.9;
	if(he[2] == 0)
		cout<<"success!"<<endl;

	return 0;
}
