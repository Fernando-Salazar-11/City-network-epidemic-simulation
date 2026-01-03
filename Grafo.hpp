#ifndef grafo
#define grafo

#include<iostream>
#include<queue>
#include<vector>
#include<unordered_map>
#include<tuple>
#include<cfloat>
#include<algorithm>
#include<unordered_set>


using namespace std;

class Node { //Clase que representa una ciudad
public:
	string city; //Nombre de la ciudad
	float risk; //Riesgo de contagiar (inversamente proporcional al riesgo de contagio) risk bajo = alto riesgo de contagio (para que funcione Dijkstra)
	//Hay que ver risk como si fueras el virus vas a expandirte por donde te cueste menos trabajo por eso tomarias las ciudades con risk bajo
	bool is_vaccinated; //La ciudad esta vacunada? (ayuda a simular la propagacion del virus despues de aplicar vacunas)
	vector<Node*> conections; //Ciudades vecinas a esta ciudad

	Node(const string c, const float r); //Constructor
	void connect(const vector<Node*> c); //Metodo para realizar las conexiones
};

struct CompareRisk { //Estructura para comparar ciudades (para priority_queue)
	bool operator()(Node*& a, Node*& b) const;
};

struct CompareDijkstra { //Estructura para comparar riesgo acumulado de ciudades
	bool operator()(pair<Node*, float>& a, pair<Node*, float>& b) const;
};

class Graph {
private:
	vector<Node*> BFS(Node* s); //La logica BFS simula de mejor manera la propagacion de un virus
	vector<Node*> Dijkstra(Node* s, Node* g); //Encuentra el camino mas corto usando el risk de Node
	//Estos 2 metodos retornan un vector con la ruta
	vector<Node*> rebuildPath(unordered_map<string, string> previous, Node*& g); //Metodo que reconstruye el camino de Dijkstra
	float calculateWeigth(const vector<Node*> path); //Calcula es peso total del camino dado por Dijkstra

public:
	unordered_map<string, Node*> nodes; //Almacena todos los nodos (ciudades)

	Graph(vector<Node*> n); //Constructor

	void simulateContagion(const string s); //Llama a BFS para simular la propagacion de contagio
	vector<pair<Node*, int>> betweennessCentrality(); //Calcula el riesgo de contagio que tiene cada ciudad
	//Retorna un vector de pair de las ciudades con el riesgo calculado de cada una
	vector<Node*> chooseCity(vector<pair<Node*, int>> cities); //Elige las ciudades que seran vacunadas
	/* Recibe el vector calculado por betweennessCentrality
	   Retorna un vector con las ciudades elegidas */
	void vaccinate(const vector<Node*> cities); //Aplicar vacunas a las ciudades elegidas
	//Recibe el vector retornado por chooseCity
};

#endif // !grafo
