#include "Grafo.hpp"

Node::Node(const string c, const float r) {
	city = c;
	risk = r;
	is_vaccinated = false;
}

void Node::connect(const vector<Node*> c) {
	for (Node* n : c) {
		conections.push_back(n);
	}
}

bool CompareRisk::operator()(Node*& a, Node*& b) const {
	return a->risk > b->risk; // menor risk = mayor prioridad (min-heap)
}

bool CompareDijkstra::operator()(pair<Node*, float>& a, pair<Node*, float>& b) const {
	return a.second > b.second; //Menor riesgo acumulado tiene mayor prioridad
}

Graph::Graph(const vector<Node*> n) {
	for (Node* node : n) {
		nodes[node->city] = node;
	}
}

float Graph::calculateWeigth(const vector<Node*> path) {
	float w = 0.0;
	for (int i = 1; i < path.size(); i++) { //Calcular peso total de un camino ignorando el peso de start
		w += path[i]->risk;
	}
	return w;
}

vector<Node*> Graph::chooseCity(vector<pair<Node*, int>> cities) {
    // Orden descendente por el contador
    sort(cities.begin(), cities.end(),
         [](auto &a, auto &b){ return a.second > b.second; });

    vector<Node*> chosen;
    unordered_set<string> seen;
    chosen.reserve(3);

    for (auto& [node, cnt] : cities) {
        if (chosen.size() == 3) break;
        if(node->city == "C") continue;
        // Insert.test devuelve true la primera vez que ve este nombre
        if (seen.insert(node->city).second) {
            chosen.push_back(node);
        }
    }
    return chosen;
}


void Graph::vaccinate(const vector<Node*> cities) {
	for (Node* c : cities) { //Marcar como vacunadas las ciudades seleccionadas
		nodes[c->city]->is_vaccinated = true;
	}
}

void Graph::simulateContagion(const string s) {
	Node* start = nodes[s];

	if (start == nullptr) { //BUscar nodo de inicio
		cout << "Error no se encontro el nodo de inicio \n";
		return;
	}

	for (Node* n : BFS(start)) { //Mostrar camino de propagacion del virus
		cout << n->city << " (" << n->risk << ")\n";
	}
	cout << '\n';
}

vector<Node*> Graph::BFS(Node* s) {
	vector<Node*> path; //Vector que almacena el camino recorrido
	unordered_map<string, bool> visited;
	priority_queue<Node*,vector<Node*>, CompareRisk> pq;

	pq.push(s);

	while (!pq.empty()) {
		Node* current = pq.top();
		pq.pop();

		if (visited[current->city] || current->is_vaccinated) continue;

		visited[current->city] = true;
		path.push_back(current);

		for (Node* n : current->conections) { //Explorar vecinos y agregarlos segun la prioridad
			if (!visited[n->city] && !n->is_vaccinated) {
				pq.push(n);
			}
		}
	}

	return path;
}

vector<Node*> Graph::Dijkstra(Node* s, Node* g) {
	unordered_map<string, float> distance; //Mapa [nodo] = distancia acumulada
	unordered_map<string, string> previous; //Mapa [hijo] = padre para reconstruir camino
	priority_queue<pair<Node*, float>, vector<pair<Node*, float>>, CompareDijkstra> pq;

	for (pair<string, Node*> n : nodes) {
		distance[n.second->city] = FLT_MAX; //Inicializar en infinito todas las distancias
	}

	distance[s->city] = 0.0; //Iniciar en 0 start
	pq.push({ s, 0.0 });
	Node* current = nullptr; //Se inicializa fuera del while para cuando termine comprobar si es == a s, si no lo es quiere decir que no encontramos el camino

	while (!pq.empty()) {
		current = pq.top().first;
		float current_dist = pq.top().second;
		pq.pop();

		if (current == g) break;
		if (current->is_vaccinated) continue;

		for (Node* n : current->conections) {
			if (n->is_vaccinated) continue;
			// Calculamos la nueva distancia acumulada (riesgo total) desde el nodo origen hasta este vecino
            // Sumamos el riesgo del nodo vecino al riesgo total que llevamos acumulado hasta el nodo actual
            float new_distance = current_dist + n->risk;

            // Si esta nueva distancia es menor que la que teníamos registrada previamente para este vecino,
            // significa que encontramos un camino más corto (de menor riesgo total)
            if (new_distance < distance[n->city]) {
                distance[n->city] = new_distance;      // Actualizamos la distancia más corta encontrada hasta ahora
                previous[n->city] = current->city;     // Guardamos el nodo previo para reconstruir el camino luego
                pq.push({ n, new_distance });          // Añadimos el vecino a la cola de prioridad con su nuevo riesgo acumulado
            }
		}
	}

	if (previous.find(g->city) == previous.end() && current != s) { //No se encontro un camino
		return {};
	}

	return rebuildPath(previous, g);
}


vector<Node*>Graph::rebuildPath(unordered_map<string, string> previous, Node*& current) {
	vector<Node*> path;

	while (true) {
		path.push_back(current);
		auto it = previous.find(current->city);
		if (it == previous.end()) break;
		current = nodes[it->second];
	}

	reverse(path.begin(), path.end());
	return path;
}

vector<pair<Node*, int>> Graph::betweennessCentrality() {
	unordered_map<string, vector<float>> cityPathWeights; //Registro de los pesos de cada ruta en la que participo una ciudad
	vector<float> allWeights; //Almacena todos los pesos para calcular promedio

	//Evaluar todos los pares (source, target)
	for (pair<string, Node*> source : nodes) {
		for (pair<string, Node*> target : nodes) {
			//Obtener camino mas propenso a contagio (camino mas facil para el virus
			vector<Node*> path = Dijkstra(source.second, target.second);

			if (path.size() > 2) { //Si no es un camino directo
				float weight = calculateWeigth(path);
				allWeights.push_back(weight);

				//Guardar peso para cada nodo intermedio
				for (int i = 1; i < path.size() - 1; i++) {
					cityPathWeights[path[i]->city].push_back(weight);
				}
			}
		}
	}

	//Calcular promedio de peso de todos los caminos
	float total_weight = 0.0;
	for (float w : allWeights) total_weight += w;
	float averge_weight = total_weight / allWeights.size();

	//Determinar para cada ciudad cuantos caminos son mas peligrosos (risk chico, es decir mas facil moverse para el virus)
	vector<pair<Node*, int>> city_danger_count;
	for (pair<string, vector<float>> current : cityPathWeights) {
		int count = 0;

		for (float w : current.second) {
			//Si la facilidad de propagacion del virus es menor al promedio se considera un camino peligroso
			if (w < averge_weight) count++;
		}

		city_danger_count.push_back({ nodes[current.first], count });
	}

	return city_danger_count;
}
