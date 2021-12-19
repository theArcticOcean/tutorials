#include <iostream>
#include<cstdio>
#include<algorithm>
using namespace std; 
int A[5]={1,1,3,4,5};
void show(){
	for(int i=0;i<5;i++){
		printf("%d ",A[i]);
	}
	cout<<endl;
}
int main(){
	int sum=0;
	do{
		show();
		sum++;
	}while(next_permutation(A,A+5));
	cout<<"sum= "<<sum<<endl;
	return 0;
}
