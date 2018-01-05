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

/*Lee un archivo y los puntos NroFrame, x, y, w, h*/
std::vector<std::pair<int,int> > ReadFileCluster(std::string nameCadena) {
  std::string line;
  std::ifstream myfile(nameCadena.c_str());
  
  std::vector<std::pair<int,int> > dataPoint;
  
  while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
    
    std::istringstream iss(line);
    int nroFrame;
    int cluster,ID;
    iss >> cluster>> ID;
    
    std::pair<int,int> pnt = make_pair(cluster,ID);

    dataPoint.push_back(pnt);
    
  }
  
  myfile.close();
  return dataPoint;
}


int main(){
  
  std::string nameCadena = "ClustersOutPut.txt";
  
  std::vector<std::pair<int,int> > data = ReadFileCluster(nameCadena);

  std::map<int,std::vector<int> > mapa;
  
  REP(i,data.size()){
    //dbg2(data[i].first,data[i].second);
    mapa[data[i].first].pb(data[i].second);
  }

  std::vector<std::pair<int,int> > frecuenciaClustersElements;
  
  std::map<int,std::vector<int> >::iterator it;
  for(it=mapa.begin();it!=mapa.end();it++){
    printf("Cluster: %d size: %d\n",(*it).first,(*it).second.size());
    frecuenciaClustersElements.push_back(make_pair((*it).second.size(),(*it).first));
    
    for(int j=0;j<(*it).second.size();j++){
      printf(" %d",(*it).second[j]);
    }
    putchar('\n');
    
  }
  
  sort(frecuenciaClustersElements.begin(),frecuenciaClustersElements.end());
  
  // Ordenar por frecuencia a los clusters
  REP(i,frecuenciaClustersElements.size()){
    printf("Cluster Id:%d Nro.Elements:%d\n",frecuenciaClustersElements[i].second,frecuenciaClustersElements[i].first);
  }
  
  return 0;
}
