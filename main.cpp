#include <iostream>
#include <vector>

using namespace std;

// vamos definir uma estrutura para representar as arestas usando o pair do C++
typedef pair<int, double> edge;

void readGraph(vector<vector<edge>> &listaAdjacencia){
    // vamos fazer a leitura do grafo
    int u, v; // vertices
    double peso;
    while(cin >> u >> v >> peso){ // enquanto não chegar no EOF
        listaAdjacencia[u].push_back(make_pair(v, peso));
        listaAdjacencia[v].push_back(make_pair(u, peso));
    }
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(nullptr);
    int n; // numero de vertices
    cin >> n;
    vector<vector<edge>> listaAdjacencia[n+1]; // precizamos de +1, pois é indexado em 0;
    
    return 0;
}