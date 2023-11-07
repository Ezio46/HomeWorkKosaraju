#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;

class Graph {
public:
    Graph(int vertices);
    void addEdge(int from, int to, int weight);
    vector<int> getSCC();
    
private:
    int vertices;
    vector<vector<pair<int, int>> > adj; 
    vector<vector<pair<int, int>> > transposed;
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
    transposed.resize(vertices);
    visited.assign(vertices, false);
    components.resize(vertices);
}

void Graph::addEdge(int from, int to, int weight) {
    adj[from].push_back(make_pair(to, weight));
    transposed[to].push_back(make_pair(from, weight));
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
    
    vector<pair<int, int>>& neighbors = (saveState) ? transposed[v] : adj[v];
    
    for (const pair<int, int>& neighbor : neighbors) {
        if (!visited[neighbor.first]) {
            DFS(neighbor.first, saveState);
        }
    }
}

void Graph::fillOrder(int v) {
    visited[v] = true;
    
    for (const pair<int, int>& neighbor : adj[v]) {
        if (!visited[neighbor.first]) {
            fillOrder(neighbor.first);
        }
    }
    
    order.push(v);
}

int main() {
    int vertices = 8;
    Graph g(vertices);
    
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 0, 3);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 5);
    g.addEdge(4, 3, 6);
    g.addEdge(5, 4, 7);
    g.addEdge(6, 5, 8);
    g.addEdge(7, 6, 9);
    g.addEdge(6, 7, 10);
    
    vector<int> components = g.getSCC();
    
    for (int i = 0; i < vertices; i++) {
        cout << "Vertex " << i << " is in component " << components[i] << endl;
    }
    
    return 0;
}
