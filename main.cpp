#include <iostream>
#include <vector>

using namespace std;

// vamos definir uma estrutura para representar as arestas usando o pair do C++
typedef pair<int, double> edge;

vector<vector<edge>> readGrafo(){
    int n; // numero de vertices
    cin >> n;
    vector<vector<edge>> listaAdjacencia(n+1); // precizamos de +1, pois é indexado em 0.
    // vamos fazer a leitura do grafo
    int u, v; // vertices
    double peso;
    while(cin >> u >> v >> peso){ // enquanto não chegar no EOF
        listaAdjacencia[u].push_back(make_pair(v, peso));
        listaAdjacencia[v].push_back(make_pair(u, peso));
    }
    return listaAdjacencia;
}

void showGrafo(vector<vector<edge>> &listaAdjacencia){
    for(int i=1; i < (int)listaAdjacencia.size(); i++){
        cout << i << " -> ";
        bool isFirst = 1;
        for(auto &par: listaAdjacencia[i]){
            if(!isFirst) cout << ", ";
            cout << "{" << par.first << ", " << par.second << "}";
            isFirst = 0;
        }
        cout << "\n";
    }
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(nullptr);
    vector<vector<edge>> listaAdjacencia = readGrafo();
    showGrafo(listaAdjacencia);
    return 0;
}