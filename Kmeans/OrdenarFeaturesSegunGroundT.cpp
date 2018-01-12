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

struct trajectory{
  int idTrajectory;
  std::string nameTrajectory;
  int idClusterTrajectory;
  int nroPointsTrajectory;
  int Ground1;
  int Ground2;
  trajectory(){}
  trajectory(int pidTrajectory,std::string pnameTrajectory,int pidClusterTrajectory,int pnroPointsTrajectory,
	     int pGround1, int pGround2){
    idTrajectory=pidTrajectory;
    nameTrajectory=pnameTrajectory;
    idClusterTrajectory=pidClusterTrajectory;
    nroPointsTrajectory=pnroPointsTrajectory;
    Ground1=pGround1;
    Ground2=pGround2;
  }
  
};

/*Lee un archivo con los features*/
std::vector<std::string> ReadFile(std::string cadena) {
  std::cout << cadena << std::endl;
  std::string line;
  std::ifstream myfile(cadena);
  std::vector<std::string> lista;

  while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
    // Eliminando la coma para tokenizar
    // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.
    while((int)line.find(';')!=-1) line[(int)line.find(';')] = ' ';
    
    std::istringstream iss(line);
    //0;D:\DataSet_Laboratory\people2\1\aprimeros500\2017-04-24_14-19-53~14-20-00_0.trk;155;23;0;0
    
    std::string inputstr;
    iss >> inputstr;
    
    lista.pb(inputstr);
  }
  
  myfile.close();
  return lista;
}

/*Lee un archivo con los atributos de las trajectorias*/
std::vector<trajectory> ReadFile2(std::string cadena) {
  std::cout << cadena << std::endl;
  std::string line;
  std::ifstream myfile(cadena);
  std::vector<trajectory> lista;

  while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
    // Eliminando la coma para tokenizar
    // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.
    while((int)line.find(';')!=-1) line[(int)line.find(';')] = ' ';
    
    std::istringstream iss(line);
    //0;D:\DataSet_Laboratory\people2\1\aprimeros500\2017-04-24_14-19-53~14-20-00_0.trk;155;23;0;0
    
    int idTrajectory;
    std::string nameTrajectory;
    int idClusterTrajectory;
    int nroPointsTrajectory;
    int Ground1;
    int Ground2;
    
    iss >> idTrajectory >> nameTrajectory >> idClusterTrajectory >> nroPointsTrajectory >> Ground1 >> Ground2;
    trajectory T1(idTrajectory, nameTrajectory, idClusterTrajectory, nroPointsTrajectory, Ground1, Ground2);
    lista.pb(T1);
  }
  
  myfile.close();
  return lista;
}

int main(){
  
  std::string cadenaEntrada = "C:\\Users\\gquis\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Summarization\\Kmeans\\IdNamesTrajectoriesOutput.csv";
  std::string cadenaEntrada2 = "C:\\Users\\gquis\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Summarization\\Kmeans\\coeficientes.txt";
  
  std::vector<trajectory> ans = ReadFile2(cadenaEntrada); // Leyendo trajectorias
  ans.erase(ans.begin()); // Eliminando el primer elemento por que esta vacio
  dbg(ans.size());
  
  std::vector<std::string> features = ReadFile(cadenaEntrada2); // Leyendo trajectorias
  std::vector<std::string> features2;

  std::string cadena = "";
  REP(i,features.size()){
    if(i&1){
      cadena += ",";
      cadena += features[i];
      features2.pb(cadena);
    } else {
      cadena = "";
      cadena += features[i];
    }
  }
  //dbg(features2.size());
  //REP(i,features2.size()){
  //dbg(features2[i]);
  //}
  // Hacer dos arreglos de pair primer elmentos features, idG1 o IdG2
  // Leer features deste coeficientes.txt
  std::vector<std::pair<int,std::string> > G1;
  std::vector<std::pair<int,std::string> > G2;  
  std::vector<std::pair<int,std::string> > clusters;    
  
  REP(i,ans.size()){
    G1.pb(mp(ans[i].Ground1,features2[i]));
    G2.pb(mp(ans[i].Ground2,features2[i]));
    clusters.pb(mp(ans[i].idClusterTrajectory,features2[i]));
  }

  sort(ALL(G1));
  sort(ALL(G2));
  sort(ALL(clusters));

  
  // Escribir en coeficientes1
  ofstream myfile;
  myfile.open("coeficientesG1.txt");
  REP(i,G1.size()){
    //dbg2(G1[i].f,G1[i].s);
    int cont=0;
    REP(j,G1[i].s.size()){
      if(G1[i].s[j]==','){
	cont++;
	if(cont==6){
	  G1[i].s[j] = '\n';
	  break;
	}
      }
    }
    myfile << G1[i].s << std::endl;
  }
  myfile.close();
  
  // Escribir en coeficientes2
  ofstream myfile2;
  myfile2.open("coeficientesG2.txt");
  REP(i,G2.size()){  
    // dbg2(G2[i].f,G2[i].s);
    int cont=0;
    REP(j,G2[i].s.size()){
      if(G2[i].s[j]==','){
	cont++;
	if(cont==6){
	  G2[i].s[j] = '\n';
	  break;
	}
      }
    }
    myfile2 << G2[i].s << std::endl;
  }

  // Escribir en coeficientesClusters
  ofstream myfile3;
  myfile3.open("coeficientesClusters.txt");
  REP(i,clusters.size()){
    //dbg2(G1[i].f,G1[i].s);
    int cont=0;
    REP(j,clusters[i].s.size()){
      if(clusters[i].s[j]==','){
	cont++;
	if(cont==6){
	  clusters[i].s[j] = '\n';
	  break;
	}
      }
    }
    myfile3 << clusters[i].s << std::endl;
  }
  myfile3.close();
  
  return 0;
}
