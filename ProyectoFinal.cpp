#include "Grafo.hpp"

int main()
{
	Node* A = new Node("A", 2);
    Node* B = new Node("B", 5);
    Node* C = new Node("C", 1);
    Node* D = new Node("D", 6);
    Node* E = new Node("E", 3);
    Node* F = new Node("F", 4);

    A->connect({ B, C });
    B->connect({ A, D });
    C->connect({ A, D, E });
    D->connect({ B, C, F });
    E->connect({ C, F });
    F->connect({ D, E });

    Graph g({ A, B, C, D, E, F });

    //Simulacion de contagio inicial
	std::cout << "Orden de contagio inicial:\n";
	g.simulateContagion("C");

	//Calcular betweenness centrality y elegir 3 ciudades para vacunar
	auto bc = g.betweennessCentrality();
	auto to_vaccinate = g.chooseCity(bc);

	std::cout << "\nCiudades seleccionadas para vacunar:\n";
	for (Node* n : to_vaccinate) {
		std::cout << "- " << n->city << "\n";
	}

	//Aplicar vacunas y re-simular
	g.vaccinate(to_vaccinate);
	std::cout << "\nOrden de contagio tras vacunacion:\n";
	g.simulateContagion("C");




	return 0;
}


