# Epidemic Propagation in City Networks

This project is a C++ simulation tool that models how a virus spreads through a network of cities. The primary goal is to identify which cities are critical for virus transmission and demonstrate how strategic vaccination at these points can mitigate the epidemic's expansion.

## 📋 Problem Description

In this model, each city is represented as a **node** in a graph. Each node has a **contagion risk** value, which is inversely proportional to its population (a low risk value indicates that it is easier for the virus to move through that city). The connections between nodes represent transmission routes through which the virus travels.

## ⚙️ Algorithm Functionality

To solve the problem and propose an effective solution, the project utilizes three fundamental graph theory algorithms:

### 1. Contagion Simulation (Best-First-Search - BFS)
A variant of Breadth-First Search (BFS) assisted by a priority queue is used to simulate the initial spread of the virus. The algorithm prioritizes expansion toward cities with lower "risk" values, simulating the virus seeking the paths of least resistance to propagate.

### 2. High-Risk Routes (Dijkstra's Algorithm)
Dijkstra's algorithm is employed to find the "lowest accumulated risk" paths between all possible pairs of cities. In this context, the shortest path calculated by Dijkstra represents the most likely or "easiest" route the virus would follow to get from point A to point B.

### 3. Identification of Critical Cities (Betweenness Centrality)
To decide which cities to vaccinate, a simplified **Betweenness Centrality** metric is implemented:
1. The optimal virus routes are calculated between all pairs of cities using Dijkstra.
2. The total weight (accumulated risk) of each route is measured, and an overall average is calculated.
3. The algorithm counts how many routes with a risk lower than the average pass through each city (acting as critical bridges).
4. The **top 3 cities** with the highest count are selected for vaccination.

## 🚀 Execution and Results

The program follows this workflow:
1. **Initial Simulation:** Displays the original contagion order without any intervention.
2. **Analysis:** Calculates centrality to identify the network's weak points.
3. **Intervention:** Applies vaccines to the 3 selected cities, marking them as "immune" nodes.
4. **Re-simulation:** Runs the contagion simulation again to demonstrate how vaccines block transmission routes and reduce the virus's reach.

## 🛠️ Technologies Used
* **Language:** C++
* **Data Structures:** Graphs (Adjacency Lists), Priority Queues, Unordered Maps.
