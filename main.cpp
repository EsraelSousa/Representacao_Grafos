#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>

using namespace std;

// vamos definir uma estrutura para representar as arestas usando o pair do C++
typedef pair<int, double> edge;

vector<vector<edge>> readGrafo(string &nomeArquivo, bool isDirecionado){
    // Para facilitar, vamos redirecionar o buffer de entrada para o arquivo
    
    ifstream arquivoInput(nomeArquivo);
    if(arquivoInput.is_open() == false){
        cout << "Nao foi possivel abrir o arquivo\n";
        exit(1);
    }
    // Salva o estado atual da entrada padrão (cin)
    streambuf* originalCin = std::cin.rdbuf();

    // Redireciona a entrada padrão para o arquivo
    cin.rdbuf(arquivoInput.rdbuf());
    
    int n; // numero de vertices
    cin >> n;
    vector<vector<edge>> listaAdjacencia(n+1); // precizamos de +1, pois é indexado em 0.
    // vamos fazer a leitura do grafo
    int u, v; // vertices
    double peso;
    while(cin >> u >> v >> peso){ // enquanto não chegar no EOF
        listaAdjacencia[u].push_back(make_pair(v, peso));
        if(isDirecionado == false)
            listaAdjacencia[v].push_back(make_pair(u, peso));
    }

    // Restaura o estado original da entrada padrão
    cin.rdbuf(originalCin);

    // Fecha o arquivo após a leitura
    arquivoInput.close();

    return listaAdjacencia;
}

bool isUmaEstruturaValida(string &s){
    if(s == "-la") return 1;
    if(s == "-ma") return 1;
    if(s == "-mi") return 1;
    if(s == "-mp") return 1;
    if(s == "-rv") return 1;
    return 0;
}

void showGrafoListaAdjacencia(vector<vector<edge>> &listaAdjacencia){
    for(int i=1; i < (int)listaAdjacencia.size(); i++){
        cout << "\033[1;31m" << i << " -> \033[0m";
        bool isFirst = 1;
        for(auto &par: listaAdjacencia[i]){
            if(!isFirst) cout << ", ";
            cout << "{" << par.first << ", " << par.second << "}";
            isFirst = 0;
        }
        cout << "\n";
    }
}

int main(int argc, char* argv[]){
    ios_base::sync_with_stdio(0); cin.tie(nullptr); // faz uma otimização de leitura
    // checar se são fornecidos todos os argumentos
    if (argc < 6) {
        cerr << "\033[1;31m Para executar esse programa use:\033[0m " << argv[0] <<
        " read inst-10 show -la 0\n"; return 1;
    }
    // pegar argumentos importantes
    string nomeInput = argv[2];
    string nomeArquivo = "Instancias_teste/" + nomeInput;
    string tipoEstrutura = argv[4];
    bool isDirecionado = atoi(argv[5]);
    // checar se são comandos validos de execução
    if(strcmp(argv[1], "read") != 0){
        cerr << "\033[1;31m O comando:\033[0m " << argv[1] <<
        "\033[1;31m nao eh valido\n\033[0m"; return 1;
    }
    if(strcmp(argv[3], "show") != 0){
        cerr << "\033[1;31m O comando:\033[0m " << argv[3] <<
        "\033[1;31m nao eh valido\n\033[0m"; return 1;
    }
    if(isUmaEstruturaValida(tipoEstrutura) == false){
        cerr << "\033[1;31m O comando:\033[0m " << argv[4] <<
        "\033[1;31m nao eh valido\n\033[0m"; return 1;
    }
    ifstream arquivoInput(nomeArquivo);
    if(arquivoInput.is_open() == false){
        cerr << "\033[1;31m Nao foi possivel abrir o arquivo:\033[0m " << argv[2] <<
        "\033[1;33m verifique se o arquivo existe!\n\033[0m"; return 1;
    }

    vector<vector<edge>> listaAdjacencia = readGrafo(nomeArquivo, isDirecionado);
    showGrafoListaAdjacencia(listaAdjacencia);
    return 0;
}