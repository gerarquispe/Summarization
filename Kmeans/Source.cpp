#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <dirent.h> // libreria de linux se descargo i pego en include.
#include <errno.h>
#include <string>
#include <cstdio>
#include <map>
#include <vector>
#include <cstdlib>
#include "Geometria.h"

#define dbg(x)std::cout<< #x <<" = "<<x<<std::endl
#define dbg2(x,y)std::cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<std::endl	
#define dbg3(x,y,z)std::cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<" "<<#z<<"="<<z<<std::endl
#define MY_COLOR_YELLOW CV_RGB(255,255,0)
#define MY_COLOR_RED CV_RGB(90, 11, 10)
#define MY_COLOR_WHITE CV_RGB(255,255,255)
#define MY_COLOR_ORANGE CV_RGB(255,69,0)
#define MY_COLOR_ORANGE1 CV_RGB(100,100,0)
#define MY_COLOR_GREEN CV_RGB(2, 74, 64)
#define MY_COLOR_BLUE CV_RGB(0,0,205)
#define MY_COLOR_BLACK CV_RGB(0,0,0) 
#define MY_COLOR_MARRON CV_RGB(77,29,59)
#define MY_COLOR_SIENNA CV_RGB(160,82,45)

std::string i2s(int nro) { std::stringstream ss; ss << nro; return ss.str(); }
int s2i(string str) { istringstream ss(str); int nro; ss >> nro; return nro; }

// Estructura para una trayectoria
struct trayectoria {
	int id;
	int clusterId;
	int nroPuntos;
	std::string name;
	std::vector<cv::Point2f> puntos;
};

// Variables globales
std::vector<trayectoria> vTrajectories;// vector con trayectorias

cv::VideoCapture video;
string pathVideo;
int contTrajectories = 0;

// vector features[[magnitud,anguloVar],cadena]
std::vector < pair< pair<double, unsigned long long int>, std::string > > features; // Vector con los features de cada video
cv::Scalar colorTab[] =
{
	 MY_COLOR_BLACK,
	 MY_COLOR_MARRON,
	 MY_COLOR_SIENNA,
	 MY_COLOR_GREEN,
	 MY_COLOR_BLUE,
	 MY_COLOR_RED,
	 MY_COLOR_ORANGE,
}; // Esto es para dar color.

/*Lee un archivo y los puntos NroFrame, id,x y*/
std::vector<cv::Point2f> ReadFile2(std::string cadena) {
	// std::cout << cadena << std::endl;
	std::string line;
	std::ifstream myfile(cadena);
	std::vector<cv::Point2f> dataPoint;

	while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
										 // Eliminando la coma para tokenizar
		line[(int)line.find(',')] = ' '; // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.
		std::istringstream iss(line);
		int nroFrame;
		double x, y;
		iss >> nroFrame >> x >> y;
		cv::Point2f pnt = cv::Point2f(x, y);
		dataPoint.push_back(pnt);
	}
	myfile.close();
	return dataPoint;
}

/*Lee un archivo y los puntos NroFrame, id, x y w h*/
std::vector<cv::Point2f> ReadFile(std::string cadena) {
	//std::cout << cadena << std::endl;
	std::string line;
	std::ifstream myfile(cadena);
	
	std::vector<cv::Point2f> dataPoint;

	while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo

		// Eliminando la coma para tokenizar
		line[(int)line.find(',')]=' '; // <<------ ojo si el formato de la ultima linea del file de entrada esta mal da error.

		std::istringstream iss(line);
		int nroFrame;
		double x, y, w, h;
		iss >> nroFrame >> x >> y >> w >> h;
		cv::Point2f pnt = cv::Point2f(x,y);
		dataPoint.push_back(pnt);
	}

	myfile.close();
	return dataPoint;
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

// Retorna la lista de los archivos .txt ya para leer
std::vector<std::string> readDir2(std::string str) {
	std::vector<std::string> listFiles = ReadFiles(str);

	std::vector<std::string> ans;
	for (int i = 0; i < listFiles.size(); i++) {
		if (listFiles[i].find(".txt") != -1) {
			//std::cout << listFiles[i] << std::endl;
			ans.push_back(listFiles[i]);
		}
	}
	return ans;
}

// Retorna la lista de los archivos .trk ya para leer
std::vector<std::string> readDir(std::string str) {
	std::vector<std::string> listFiles = ReadFiles(str);

	std::vector<std::string> ans;
	for (int i = 0; i < listFiles.size(); i++) {
		if (listFiles[i].find(".trk") != -1) {
			//std::cout << listFiles[i] << std::endl;
			ans.push_back(listFiles[i]);
		}
	}
	return ans;
}

// Modulo para leer la direccion del video
bool loadVideo(std::string path)
{
	pathVideo = path;
	video.open(path);
	if (!video.isOpened())
		return false;
	return true;
}

/* Modulo que permite leer los files .txt y ademas devuelve una trayectoria */
trayectoria getTrajectory(std::string inputFile, cv::Mat &sumScreen, int i_th) { //sumScreen es para mostrar acumulados de trayectorias

	trayectoria traAux;

	// Preguntamos que tipo de file es .trk o .txt
	//dbg(inputFile);
	if(inputFile.find(".txt")!=-1)
		traAux.puntos = ReadFile(inputFile); // Lee el file de arriba
	else
		traAux.puntos = ReadFile2(inputFile); // Lee el file de arriba

	traAux.nroPuntos = traAux.puntos.size();

	// Seccion de código para escribir sumScreen
	/*
	cv::circle(sumScreen, traAux.Puntos[0], 3, colorTab[0], cv::FILLED, cv::LINE_AA);
	for (int i = 1; i < traAux.Puntos.size(); i++) {
		cv::line(sumScreen, traAux.Puntos[i-1], traAux.Puntos[i], colorTab[i_th % 7]);
		cv::circle(sumScreen, traAux.Puntos[i], 6, colorTab[i_th % 7], cv::FILLED, cv::LINE_AA);
	}
	*/

	// Aqui todos los puntos a un array 2D para interpolar
	// Empieza a escribir en files para interpolar outputX.txt
	//Validando numero de puntos
	
	return traAux;
}

void main3() {
	// leer la data de los archivos con las trayectorias de todas las carpetas
	for (int k = 1; k <= 4; k++) { // Para cada carpeta donde encuentran los videos
		cv::Mat sumScreen(800, 1280, CV_8UC3);  // Definicion de tamaño y tipo sumScreen
		sumScreen = cv::Scalar::all(0);

		//std::vector<trayectoria> vTrajectories - vector con trayectorias ahora es global
		// Leer todos los files de esta dir
		std::string Endereco = "D:\\DataSet_Laboratory\\people2\\" + i2s(k);

		//lista todos los archivos de una carpeta sin excepcion de tipo
		std::vector<std::string> listDir = ReadFiles(Endereco);
		dbg(listDir.size());
		for (int i = 0; i < listDir.size(); i++) {
			// Listando los directorios
			if (listDir[i].find(".") == -1) { // Entra solo a los que son carpetas
											  // Entrando a cada carpeta y listando, lisDir tiene el nombre de las carpetas
											  //std::cout << listDir[i] << std::endl;

											  // readDir Regresa una lista con los archivos .trk para leer de lisDir_i
				std::vector<std::string> archivos = readDir(Endereco + "\\" + listDir[i]);

				// Pasta tiene lista de archivos de  una carpeta
				std::string enderecoSum = "";
				enderecoSum += Endereco;
				enderecoSum += "\\";
				enderecoSum += listDir[i];

				std::string enderecoCompleto = "";

				for (int j = 0; j < archivos.size(); j++) { // Loop que pasa todos los archivos
															// Aqui tenemos el archivo a ser procesado para lectora de puntos de cada tracklet
															//std::cout << archivos[j] << std::endl;
					enderecoCompleto = enderecoSum + "\\" + archivos[j];

					// Das una direccion i de un file.txt recibes un vector de una trayectoria
					trayectoria ans = getTrajectory(enderecoCompleto, sumScreen, j);
					// Das una direccion y recibes una trayectoria 
					// hacer validacion de numero de puntos, escribir trajectorias e imagenes 
					if (ans.nroPuntos <= 6)
						continue;
					else {

						// Escribe en files puntos de la trajectoria
						//ofstream myfile;
						//myfile.open("output" + i2s(contTrajectories) + ".txt");
						//for (int i = 0; i < ans.Puntos.size(); i++) {
						//myfile << (int)ans.Puntos[i].x << " " << (int)ans.Puntos[i].y << endl;
						//}
						//myfile.close();

						// Escribir una trayectoria individual
						cv::Mat Screen(800, 1280, CV_8UC3);  // Definicion de tamaño y tipo Screen
						Screen = cv::Scalar::all(255);
						cv::circle(Screen, ans.puntos[0], 6, colorTab[j % 7], cv::FILLED, cv::LINE_AA);
						for (int i = 1; i < ans.puntos.size(); i++) {
							cv::line(Screen, ans.puntos[i - 1], ans.puntos[i], colorTab[j % 7], 3);
							cv::circle(Screen, ans.puntos[i], 6, colorTab[j % 7], cv::FILLED, cv::LINE_AA);
						}

						// Escribir la imagen de Screen (una trayectoria) en la direccion dada
						cv::imwrite("trajectories/" + i2s(contTrajectories) + ".png", Screen);

						//cv::imshow("Screen", Screen);
						//cv::waitKey(800);
						//cv::waitKey(0);
						ans.id = contTrajectories;
						ans.name = enderecoCompleto;
						vTrajectories.push_back(ans);
						contTrajectories++; // Trajectory ID 
					}

					//std::cout << enderecoCompleto << std::endl;

					// Escribiendo con nombre del video sin txt para escribirlo como imagen .jpg
					//std::string enderecoSinTxt = archivos[j];
					//enderecoSinTxt = enderecoSinTxt.substr(0, (int)enderecoSinTxt.find("."));
					//cv::imwrite("trajectories\\"+ enderecoSinTxt +".jpg",sumScreen);
				}

			}
		} // End listDir files

		  // Mostrando imagen con todas las trayectorias escogidas en un solo frame
		  //cv::imwrite("Clustering" + i2s(k) + ".png", sumScreen);
		  //cv::imshow("SumScreen", sumScreen);
		  //cv::waitKey(0);

	} // End 4 files

	// Escribiendo archivo donde se coloca las trayectorias
	ofstream myfile;
	myfile.open("idTrajectories.txt");
	for (int i = 0; i < vTrajectories.size(); i++) {
		for (int j = 0; j < vTrajectories[i].nroPuntos; j++) {
			myfile << (int)vTrajectories[i].puntos[j].x << " ";
		}
		myfile << endl;
		for (int j = 0; j < vTrajectories[i].nroPuntos; j++) {
			myfile << (int)vTrajectories[i].puntos[j].y << " ";
		}
		myfile << endl;
		// Escribiendo los ID de todas las trayectorias
		//myfile << vTrajectories[i].Id << std::endl;
	}
	myfile.close();
}

void main2() {
	// leer la data de los archivos con las trayectorias de todas las carpetas
	for (int k = 1; k <= 4; k++) { // Para cada carpeta donde encuentran los videos
		cv::Mat sumScreen(800, 1280, CV_8UC3);  // Definicion de tamaño y tipo sumScreen
		sumScreen = cv::Scalar::all(0);

		//std::vector<trayectoria> vTrajectories - vector con trayectorias ahora es global
											   // Leer todos los files de esta dir
		// main2 funciona con people y people1 pero no con poeople2, ojo tiene que ser archivos .txt
		std::string Endereco = "D:\\DataSet_Laboratory\\peopleDigitos\\" + i2s(k);
		//lista todos los archivos de una carpeta sin excepcion de tipo
		std::vector<std::string> listDir = ReadFiles(Endereco);
		dbg(listDir.size());
		for (int i = 0; i < listDir.size(); i++) {
			// Listando los directorios
			if (listDir[i].find(".") == -1) { // Entra solo a los que son carpetas
											  // Entrando a cada carpeta y listando, lisDir tiene el nombre de las carpetas
				dbg(listDir[i]);

				// readDir Regresa una lista con los archivos .txt para leer de lisDir_i
				std::vector<std::string> archivos = readDir2(Endereco + "\\" + listDir[i]);

				// Pasta tiene lista de archivos de  una carpeta
				std::string enderecoSum = "";
				enderecoSum += Endereco;
				enderecoSum += "\\";
				enderecoSum += listDir[i];

				std::string enderecoCompleto = "";	
				//dbg(archivos.size());
				for (int j = 0; j < archivos.size(); j++) { // Loop que pasa todos los archivos
															// Aqui tenemos el archivo a ser procesado para lectora de puntos de cada tracklet
															//std::cout << archivos[j] << std::endl;
					enderecoCompleto = enderecoSum + "\\" + archivos[j];
					dbg(enderecoCompleto);
					// Das una direccion i de un file.txt recibes un vector de una trayectoria
					trayectoria ans = getTrajectory(enderecoCompleto, sumScreen, j);
					// Das una direccion y recibes una trayectoria 
					// hacer validacion de numero de puntos, escribir trajectorias e imagenes 
					if (ans.nroPuntos <= 6)
						continue;
					else {
						
						// Escribe en files puntos de la trajectoria
						//ofstream myfile;
						//myfile.open("output" + i2s(contTrajectories) + ".txt");
						//for (int i = 0; i < ans.Puntos.size(); i++) {
							//myfile << (int)ans.Puntos[i].x << " " << (int)ans.Puntos[i].y << endl;
						//}
						//myfile.close();

						// Escribir una trayectoria individual
						cv::Mat Screen(800, 1280, CV_8UC3);  // Definicion de tamaño y tipo Screen
						Screen = cv::Scalar::all(255);
						cv::circle(Screen, ans.puntos[0], 6, colorTab[j % 7], cv::FILLED, cv::LINE_AA);
						for (int i = 1; i < ans.puntos.size(); i++) {
							cv::line(Screen, ans.puntos[i - 1], ans.puntos[i], colorTab[j % 7], 3);
							cv::circle(Screen, ans.puntos[i], 6, colorTab[j % 7], cv::FILLED, cv::LINE_AA);
						}

						// Escribir la imagen de Screen (una trayectoria) en la direccion dada
						cv::imwrite("trajectories/" + i2s(contTrajectories) + ".png", Screen);

						//cv::imshow("Screen", Screen);
						//cv::waitKey(800);
						//cv::waitKey(0);
						ans.id = contTrajectories;
						vTrajectories.push_back(ans);
						contTrajectories++; // Trajectory ID 
					}

					//std::cout << enderecoCompleto << std::endl;
					
					// Escribiendo con nombre del video sin txt para escribirlo como imagen .jpg
					//std::string enderecoSinTxt = archivos[j];
					//enderecoSinTxt = enderecoSinTxt.substr(0, (int)enderecoSinTxt.find("."));
					//cv::imwrite("trajectories\\"+ enderecoSinTxt +".jpg",sumScreen);
					
				}
			}
		} // End listDir files
		
		// Mostrando imagen con todas las trayectorias escogidas en un solo frame
		//cv::imwrite("Clustering" + i2s(k) + ".png", sumScreen);
		//cv::imshow("SumScreen", sumScreen);
		//cv::waitKey(0);

	} // End 4 files

	// Escribiendo archivo donde se coloca las trayectorias
	ofstream myfile;
	myfile.open("idTrajectories.txt");
	for (int i = 0; i < vTrajectories.size(); i++) {
		for (int j = 0; j < vTrajectories[i].nroPuntos; j++) {
			myfile << (int)vTrajectories[i].puntos[j].x << " ";
		}
		myfile << endl;
		for (int j = 0; j < vTrajectories[i].nroPuntos; j++) {
			myfile << (int)vTrajectories[i].puntos[j].y << " ";
		}
		myfile << endl;
		// Escribiendo los ID de todas las trayectorias
		//myfile << vTrajectories[i].Id << std::endl;
	}
	myfile.close();
}

void setClusterParamenters(int dimFeatures) {
	// Escribir el numero de trajectorias extraidas correctamente
	ofstream myfile;
	myfile.open("numberTrajectory.txt");
	myfile << contTrajectories << " " << dimFeatures << endl;
	myfile.close();
}

/*Lee un archivo y los puntos NroFrame, x, y, w, h*/
std::vector<std::pair<int, int> > ReadFileCluster(std::string nameCadena) {
	std::string line;
	std::ifstream myfile(nameCadena.c_str());

	std::vector<std::pair<int, int> > dataPoint;

	while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo

		std::istringstream iss(line);
		int nroFrame;
		int cluster, ID;
		iss >> cluster >> ID;
		std::pair<int, int> pnt = make_pair(cluster, ID);
		dataPoint.push_back(pnt);
	}

	myfile.close();
	return dataPoint;
}

void processOutputClustering(int dimFeature) {
	std::string nameCadena = "ClustersOutPut.txt";
	std::vector<std::pair<int, int> > data = ReadFileCluster(nameCadena);
	std::map<int, std::vector<int> > mapa;

	for (int i = 0; i < data.size();i++) {
		//dbg2(data[i].first,data[i].second);
		mapa[data[i].first].push_back(data[i].second);
	}

	std::vector<std::pair<int, int> > frecuenciaClustersElements;
	std::map<int, std::vector<int> >::iterator it;

	for (it = mapa.begin(); it != mapa.end(); it++) {
		// para cada cluster escribimos un file
		ofstream myfile;
		myfile.open("clusters/cluster" + i2s((*it).first) + ".cluster");
		printf("Cluster: %d size: %d\n", (*it).first, (*it).second.size());
		frecuenciaClustersElements.push_back(make_pair((*it).second.size(), (*it).first));

		// Escribir size of clustering
		ofstream fileSalida;
		fileSalida.open("clusters/sizecluster"+i2s((*it).first)+".txt");
		fileSalida << (*it).second.size() << " " << dimFeature << endl;
		fileSalida.close();

		for (int j = 0; j<(*it).second.size(); j++) {
			// Escribimos cada uno de los Id en el file
			myfile << (*it).second[j] << endl;
			printf(" %d", (*it).second[j]);
		}
		myfile.close();
		putchar('\n');
	}

	sort(frecuenciaClustersElements.begin(), frecuenciaClustersElements.end());

	// Ordenar por frecuencia a los clusters
	ofstream myfile;
	myfile.open("clusters/sortedClusters.txt");
	for (int i = 0; i < frecuenciaClustersElements.size(); i++) {
		printf("Cluster Id:%d Nro.Elements:%d\n", frecuenciaClustersElements[i].second, frecuenciaClustersElements[i].first);
		myfile << "Cluster Id:"<< frecuenciaClustersElements[i].second <<" Nro.Elements:"<< frecuenciaClustersElements[i].first << endl;
	}
	myfile.close();
	return;
}

void firstSol() {
	int dimFeature = 12;
	setClusterParamenters(dimFeature); // creando parametros numero de trayectorias y dimension de features
	// interpolacion de puntos en matlab
	// Process interpolation output 
	system("interpolation.exe");

	// Call clustering method
	// system("python ClusteringMeanShift.py");
	// system("python ClusteringDBSCAN.py");
	// system("python ClusteringAffinity.py ");
	// system("python ClusteringKmeans.py ");

	system("activate tensorflow && python SOM2.py");

	// Process clustering output ClustersOutPut.txt
	processOutputClustering(dimFeature); // Escribimos los clusters en files

	// SI CLUSTERING DE CLUSTERING, se tendria que omitir la parte de present clustering y 
	// la parte de generarPDF.py ya que las respuestas, se encontrarian en archivos diferentes de cada carpeta cluster
	// Aqui vendría el clustering de clustering.
	// se podria eliminar los archivos .cluster o carpetas (sugerencia)
	system("python ClusteringDeClustering.py");

	// Aqui llamamos al archivo en python para escribir los .tex
	//system("python PresentClusters.py"); // construye el archivo .tex

	// Aqui si es posible ejecutamos los .tex comandos
	//system("cd clusters\\  && python generarPDF.py"); // Sólo genera pdf's

	return;
}

double getAnglesBetweenVec(Vector A, Vector B) {
	double piValue = 3.14159265;
	return atan2(cross(A, B), dot(A, B)) * 180 / piValue;
}

//Modulo con shape descriptor
void secondSol() {
	// Dimension del histograma
	int const Hsize = 12;
	int const Wsize = 12;

	int hist[Hsize][Wsize];
	double limits[Hsize+1];

	for (int i = 0; i <= Hsize;i++) {
		limits[i] = 360*i/ Hsize;
	}

	ofstream myfile;
	myfile.open("coeficientes.txt");

	// obtenemos el arreglo con todas las trayectorias
	for (int i = 0; i < vTrajectories.size(); i++) {
		std::vector<GPoint> VG;
		// transfer data to GPoint structure
		int vsize = vTrajectories[i].puntos.size();

		// Transferir la data de Pointf a GPoint
		for (int j = 0; j < vsize; j++) VG.push_back(GPoint(vTrajectories[i].puntos[j].x, vTrajectories[i].puntos[j].y));

		memset(hist,0,sizeof(hist));
		Vector U = Vector(1,0);
		// Combinatoria
		for (int j = 0; j < vsize; j++) {
			for (int k = j+1; k < vsize; k++) {
				Vector P = Vector(VG[k] - VG[j]);

				// obtener angulo
				double ang = getAnglesBetweenVec(U, P);
				if (ang < 0) ang += 360;

				// mapeamos en nuestro histograma
				for (int l = 1; l <= Hsize; l++) {
					//dbg2(limits[l - 1], limits[l]);
					//dbg(ang);
					if (limits[l - 1]<=ang && ang<limits[l]) {
						int nro = (int)log2(P.mod());
						if (nro<1) {
							hist[l - 1][0]++;
						}
						else {
							hist[l - 1][nro]++;
						}
						break;
					}
				}
			}
		}


		// Revisar id de cada trayectoria esta imprimiendo doble.
		// vectorizar
		for (int j = 0; j<Hsize/2; j++) {
			for (int k = 0; k < Wsize; k++) {
				if(j == Hsize / 2 - 1 && Wsize-1 == k)
					myfile << hist[j][k];
				else
					myfile << hist[j][k] << ",";
			}
		}
		myfile << std::endl;
		for (int j = Hsize/2; j < Hsize; j++) {
			for (int k = 0; k < Wsize; k++) {
				if (j == Hsize-1 && Wsize - 1 == k)
					myfile << hist[j][k];
				else
					myfile << hist[j][k] << ",";
			}
		}
		myfile << std::endl;
	}
	myfile.close(); // fin de escritura para files
	
	int dimfeature = 144;
	setClusterParamenters(dimfeature);
					// Call clustering method
	//system("python ClusteringMeanShift.py");
	//system("python ClusteringDBSCAN.py");
	//system("python ClusteringAffinity.py ");
	system("python ClusteringKmeans.py");

	// Process clustering output ClustersOutPut.txt
	processOutputClustering(dimfeature); // Escribimos los clusters en files

	// Aqui llamamos al archivo en python para escribir los .tex
	system("python PresentClusters.py");

	// Aqui si es posible ejecutamos los .tex comandos
	system("cd clusters\\  && python generarPDF.py");

	return;
}

void createCSV() {

	// Recoger id's de elementos de clustering 
	std::vector<int> clustersIDs((int)vTrajectories.size(), 0);
	dbg(vTrajectories.size());
	dbg(clustersIDs.size());
	// vamos a leer los archivos de cada cluster
	std::string Endereco = "C:\\Users\\gquis\\Documents\\Visual Studio 2015\\Projects\\Kmeans\\Kmeans\\clusters\\";

	//lista todos los archivos de una carpeta sin excepcion de tipo
	std::vector<std::string> listDir = ReadFiles(Endereco);

	for (int i = 0; i < listDir.size(); i++) {
		int id, clusterId;
		if (listDir[i].find(".") == -1 && listDir[i].find("cluster") != -1) { // Entra solo a los que son carpetas cluster
										  // Entrando a cada carpeta y listando, lisDir tiene el nombre de las carpetas
			// Se tiene que leer el archivo donde estan los clusters
				std::string line;
				std::string inputCadena = Endereco + listDir[i] + "\\ClustersOutPut.txt";
				std::ifstream myfile(inputCadena);
				dbg(true);
				while (std::getline(myfile, line)) { // Ojo lee hasta fin de archivo
													
					std::istringstream iss(line);
					iss >> clusterId >> id;
					dbg2(clusterId,id);

					clustersIDs[id] = clusterId;
				}
				myfile.close();

		}
	}

	ofstream myfile;
	myfile.open("IdNamesTrajectories.csv");
	myfile << "Id, Nombre, Tiempo, Forma, Orientacion" << std::endl;
	for (int i = 0; i < vTrajectories.size(); i++) {
		dbg(vTrajectories[i].id);
		myfile << vTrajectories[i].id << "," << vTrajectories[i].name << ","<< vTrajectories[i].nroPuntos<< ","<< clustersIDs[i] << std::endl;
	}
	myfile.close();

	return;
}

int main()
{
	// La primera partes llama a main2() donde se procesa los archivos de trayectorias
	//main2(); // En esta parte escribimos files para ser procesados output.txt
	main3(); // para texto .trk, NO funciona con .txt

	// Escogemos el modelo a utilizar
	// firstSol(); // Clustering de clustering
	// secondSol();
	
	// Creando la tabla de excel para su configuracion
	createCSV(); // recorriendo archivos.

	return 0;
}
