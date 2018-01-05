#include <cstdio>
#include <iostream>
using namespace std;
int main(){
  int n = 250; // numero de coefficentes
  int v1=0,v2=250,v3=2*250,v4=3*250,v5=4*250,v6=5*250,v7=6*250,v8=7*250,v9=8*250,v10=9*250,v11=10*250,v12=11*250;
  for(int i=0;i<n;i++){
    int v[12];
    v[0]=v1;
    v[1]=v2;
    v[2]=v3;
    v[3]=v4;
    v[4]=v5;
    v[5]=v6;
    v[6]=v7;
    v[7]=v8;
    v[8]=v9;
    v[9]=v10;
    v[10]=v11;
    v[11]=v12;
    for(int j=0;j<12;j++){
      if(j==5) printf("%d",v[j]),putchar('\n');
      else
	printf("%d%c",v[j],char(j==11?10:','));
    }
    v1++;
    v2++;
    v3++;
    v4++;
    v5++;
    v6++;
    v7++;
    v8++;
    v9++;
    v10++;
    v11++;
    v12++;
  }
  return 0;
}
