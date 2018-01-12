#include <bits/stdc++.h>
#include <dirent.h> // libreria de linux se descargo i pego en include.
#include <sys/types.h>
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

/*Lee un archivo y los puntos NroFrame, id,x y*/
std::vector<std::string> ReadFile2(std::string cadena) {
	std::cout << cadena << std::endl;
	std::string line;
	std::ifstream myfile(cadena);
	std::vector<std::string> lista;

	while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
									    // Eliminando la coma para tokenizar
		 // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.
		while((int)line.find(',')!=-1) line[(int)line.find(',')] = ' ';
		
		std::istringstream iss(line);
		//0,D:\DataSet_Laboratory\people2\1\aprimeros500\2017-04-24_14-19-53~14-20-00_0.trk,23,155
		int idTrajectory;
		std::string nameTrajectory;
		int nroPointsTrajectory;
		int idCluster;
		
		iss >> idTrajectory >> nameTrajectory >> nroPointsTrajectory >> idCluster;
		lista.pb(nameTrajectory);
	}
	
	myfile.close();
	return lista;
}

// clean and save trajectoriesNames
std::vector<std::string> cleanSave(std::vector<std::string> v){

  std::set<std::string> arbol;
  
  
  std::vector<std::string> ans;
  REP(i,v.size()){
    std::string s1 = v[i];    
    int cont=0;
    REP(j,s1.size()){
      if(s1[j]=='\\'){
	cont++;
	if(cont==5){
	  s1 = s1.substr(j+1,s1.size()-j);
	  s1 = s1.substr(0,s1.find('.'));
	  std::reverse(ALL(s1));
	  s1 = s1.substr(s1.find('_')+1,s1.size()-s1.find('_'));
	  std::reverse(ALL(s1));
	  
	  if(arbol.find(s1)==arbol.end()){
	    arbol.insert(s1);
	    s1 += ".avi";
	    ans.pb(s1);
	  }

	  
	  break;
	}
      }
    }
  }
  return ans;
}

// Funcion para leer files de ReadFiles
int getdir(std::string dir, std::vector<std::string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(std::string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

/*Recibe una direccion y lista todos los archivos dentro de esa carpeta*/
std::vector<std::string> ReadFiles(std::string str) {
	std::string dir = std::string(str); // Direccion desde la cual vamos a leer todos los archivos
	std::vector<std::string> files = std::vector<std::string>();
	getdir(dir, files);
	std::vector<std::string> ans;
	for (unsigned int i = 0; i < files.size(); i++) {
		if (files[i].size() != 1 && files[i].size() != 2)
			ans.push_back(files[i]);
	}
	return ans;
}

int main(){
  
  std::string cadenaEntrada = "C:\\Users\\gquis\\Documents\\C++\\IdNamesTrajectories.csv";
  std::vector<std::string> ans = ReadFile2(cadenaEntrada);

  // Devuelve el nombres de los archivos
  std::vector<std::string> Recortado = cleanSave(ans);

  // leer todos los videos como texto
  // Lista todos los archivos de una carpeta sin excepcion
  std::string Endereco = "E:\\Videos";
  std::vector<std::string> listDir = ReadFiles(Endereco);
  
  int cont=0;
  REP(i,listDir.size()){
    REP(j,Recortado.size()){
      if(listDir[i]==Recortado[j]){
	dbg(listDir[i]);
	//std::string src = "xcopy E:\\Videos\\";
	//src += listDir[i];
	//src += " C:\\Users\\gquis\\Documents\\C++\\videoscopy";
	//src += " /E /C /H /R /K /O /Y ";
	//system(src.c_str());
	cont++;
	break;
      }
    }
  }
  
  dbg(cont);
  dbg(Recortado.size());
  
  // Escribiendo archivo donde se coloca las trayectorias
  ofstream myfile;
  myfile.open("trajectories.csv");
  for (int i = 0; i < Recortado.size(); i++) {
    myfile << i2s(i+1)<< "," << Recortado[i] << endl;
  }
  myfile.close();
  
  printf("Hola Mundo desde c++");
  
  return 0;
}
