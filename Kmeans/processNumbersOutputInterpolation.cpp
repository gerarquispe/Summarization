#include <bits/stdc++.h>
#define ll long long
#define vi vector<int>
#define ii pair<int,int>
#define vii vector<ii>
#define REPN(i,x,y) for(int i=x;i<y;i++)
#define REP(i,y) REPN(i,0,y)
#define REPR(i,y,x) for(int i=y;i>=x;i--)
#define CLR(A,x) memset(A,x,sizeof(A))
#define INF (1<<30)
#define EPS (1e-9)
#define ones(x) __builtin_popcount(x)
#define ALL(v) (v).begin(),(v).end()
#define RALL(v) (v).rbegin(),(v).rend()
#define pb push_back
#define mp make_pair
#define sqr(x) (x)*(x)
#define dbg(x) cout << #x << " = " << x << endl
#define dbg2(x,y)cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<endl
#define dbg3(x,y,z)cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<" "<<#z<<"="<<z<<endl
#define S(x)scanf("%d\n",&x)
#define SS(str) scanf("%[^\n]\n",str)
#define S2(x,y)scanf("%d %d\n",&x,&y)
#define SC(x)scanf("%d",&x)
#define SC2(x,y)scanf("%d %d",&x,&y)
#define P(x)printf("%d\n",x)
#define SZ(v) v.size()
#define f first
#define s second
#define MOD 100007
#define MAXN 100005
using namespace std;
string i2s(int x) { stringstream ss; ss << x; return ss.str();}
int s2i(string str) { istringstream ss(str);int nro; ss >> nro; return nro;}

struct polinomio{
  double p0,p1,p2,p3,p4,p5;

  polinomio(){}
  polinomio(double pp0,double pp1,double pp2,double pp3,double pp4,double pp5 ){
    p0 = pp0;
    p1 = pp1;
    p2 = pp2;
    p3 = pp3;
    p4 = pp4;
    p5 = pp5;
  }
};

/*Lee un archivo y los puntos NroFrame, x, y, w, h*/
std::vector<polinomio> ReadFile(std::string nameCadena) {
  std::string line;
  std::ifstream myfile(nameCadena.c_str());
  
  std::vector<polinomio> dataPoint;
  
  while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
    int j=0;
    while(j<5){
      // Eliminando la coma para tokenizar
      line[(int)line.find(',')]=' '; // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.
      j++;
    }
    
    std::istringstream iss(line);
    int nroFrame;
    double p0, p1, p2, p3,p4, p5;
    iss >> p0>> p1>> p2>> p3>> p4>> p5;
    
    polinomio pnt = polinomio(p0,p1,p2,p3,p4,p5);

    dataPoint.push_back(pnt);
    
  }
  
  myfile.close();
  return dataPoint;
}


void writeFile(std::vector<polinomio> dataset ,std::string nameCadena){
  ofstream myfile;
  myfile.open(nameCadena.c_str());
  for (int i = 0; i < dataset.size(); i++) {
    myfile << dataset[i].p0<<" " << dataset[i].p1<<" " <<dataset[i].p2<<" " <<dataset[i].p3<<" " <<dataset[i].p4<<" " <<dataset[i].p5 <<std::endl;
    std::cout << dataset[i].p0<<" " << dataset[i].p1<<" " <<dataset[i].p2<<" " <<dataset[i].p3<<" " <<dataset[i].p4<<" " <<dataset[i].p5 <<std::endl;
  }
  myfile.close();
}

int main(){


  std::string nameFile = "coeficientes.txt";

  std::vector<polinomio> dataset=ReadFile(nameFile);

  writeFile(dataset,nameFile);
  
  
  return 0;
}
