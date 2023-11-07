#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;

class Graph {
public:
    Graph(int vertices);
    void addEdge(int from, int to);
    vector<int> getSCC();
    
private:
    int vertices;
    vector<vector<int>> adj;
    vector<vector<int>> transpose;
    vector<bool> visited;
    stack<int> order;
    unordered_set<int> currentComponent;
    vector<int> components;
    
    void DFS(int v, bool saveState);
    void fillOrder(int v);
};

Graph::Graph(int vertices) {
    this->vertices = vertices;
    adj.resize(vertices);
    transpose.resize(vertices);
    visited.assign(vertices, false);
    components.resize(vertices);
}

void Graph::addEdge(int from, int to) {
    adj[from].push_back(to);
    transpose[to].push_back(from);
}

vector<int> Graph::getSCC() {
    for (int i = 0; i < vertices; i++) {
        if (!visited[i]) {
            fillOrder(i);
        }
    }
    
    visited.assign(vertices, false);
    int componentId = 0;
    
    while (!order.empty()) {
        int v = order.top();
        order.pop();
        
        if (!visited[v]) {
            currentComponent.clear();
            DFS(v, true);
            for (int vertex : currentComponent) {
                components[vertex] = componentId;
            }
            componentId++;
        }
    }
    
    return components;
}

void Graph::DFS(int v, bool saveState) {
    visited[v] = true;
    if (saveState) {
        currentComponent.insert(v);
    }
    
    vector<int>& neighbors = (saveState) ? transpose[v] : adj[v];
    
    for (int neighbor : neighbors) {
        if (!visited[neighbor]) {
            DFS(neighbor, saveState);
        }
    }
}

void Graph::fillOrder(int v) {
    visited[v] = true;
    
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            fillOrder(neighbor);
        }
    }
    
    order.push(v);
}

int main() {
    int vertices = 8;
    Graph g(vertices);
    
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 3);
    g.addEdge(5, 4);
    g.addEdge(6, 5);
    g.addEdge(7, 6);
    g.addEdge(6, 7);
    
    vector<int> components = g.getSCC();
    
    for (int i = 0; i < vertices; i++) {
        cout << "Vertex " << i << " is in component " << components[i] << endl;
    }
    
    return 0;
}
