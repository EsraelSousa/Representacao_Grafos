#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>
#include <math.h>
#include <iomanip>
#include <set>
#include <map>

using namespace std;

// vamos definir uma estrutura para representar as arestas usando o pair do C++
typedef pair<int, double> edge;

bool isUmaEstruturaValida(string &s){
    if(s == "-la") return 1;
    if(s == "-ma") return 1;
    if(s == "-mi") return 1;
    if(s == "-mp") return 1;
    if(s == "-rv") return 1;
    return 0;
}

void showComandosEstruturas(){
    cout << "Veja os comandos validos\n";
    cout << "\033[1;34m Comando -> Estrutura\n\033[0m";
    cout << "\033[1;34m  -ma    -> matriz de adjacencias\n\033[0m";
    cout << "\033[1;34m  -mi    -> matriz de incidencias\n\033[0m";
    cout << "\033[1;34m  -la    -> lista de adjacencias\n\033[0m";
    cout << "\033[1;34m  -rv    -> representacao vetorial\n\033[0m";
    cout << "\033[1;34m  -mp    -> matriz de pesos\n\033[0m";
}

void checkPossivelConverterMatrriz(vector<vector<edge>> &listaAdjacencia){
    /* não é possivel fazer matriz nesse programa com mais de 10^6 celulas, para matriz
       vai passar um pouco, mas se o grafo for direcionado e completo não vai dar para 
       alocar espaço na memoria, pois precisa O(9 * 10^8) de memoria [(1000 * 999) * 1000] */ 
    if(listaAdjacencia.size() > 1000){
        cout << "O grafo tem muitos vertices, nao eh possivel exibir '-'\n";
        exit(1);
    }
    // se não entrou no if, continua o programa
}

template <typename T> /* isso vai deixar generico para qualquer tipo, assim não faz uma função 
                        para o tipo int e outra para o tipo double */
void checkAlocouMemoria(vector<vector< T >> &variavel){
    if(variavel.empty()){
        cout << "Nao foi possivel alocar memoria! '-'\n";
        exit(1);
    }
}

int countDigitos(int n){
    int ans = 1;
    while(-9 > n || n > 9){
        ans++;
        n /= 10;
    }
    return ans + (n < 0); // (n < 0) contamos o sinal de menos também
}

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
    checkAlocouMemoria(listaAdjacencia);
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

vector<vector<int>> listaAdjacencia2matrizAdjacencia(vector<vector<edge>> &listaAdjacencia){
    checkPossivelConverterMatrriz(listaAdjacencia);
    int n = listaAdjacencia.size();
    vector<vector<int>> matrizAdjacencia(n, vector<int>(n, 0)); // cria a matriz e seta com 0's
    checkAlocouMemoria(matrizAdjacencia);
    for(int i=1; i<listaAdjacencia.size(); i++){
        for(auto &par: listaAdjacencia[i])
            matrizAdjacencia[i][ par.first ] = 1;
    }
    return matrizAdjacencia;
}

vector<vector<double>> listaAdjacencia2matrizPesos(vector<vector<edge>> &listaAdjacencia){
    checkPossivelConverterMatrriz(listaAdjacencia);
    int n = listaAdjacencia.size();
    vector<vector<double>> matrizAdjacencia(n, vector<double>(n, 0)); // cria a matriz e seta com 0's
    checkAlocouMemoria(matrizAdjacencia);
    for(int i=1; i<listaAdjacencia.size(); i++){
        for(auto &par: listaAdjacencia[i])
            matrizAdjacencia[i][ par.first ] = par.second;
    }
    return matrizAdjacencia;
}

vector<vector<int>> listaAdjacencia2matrizIncidencia(vector<vector<edge>> &listaAdjacencia, bool isDirecionado){
    checkPossivelConverterMatrriz(listaAdjacencia);
    int n = listaAdjacencia.size(); // numero de vertices
    int m = 0; // numero de arestas
    int idAresta = 0;
    // contamos quantas arestas existem no grafo
    for(int i=1; i<n; i++)
        m += listaAdjacencia[i].size();
    if(isDirecionado == false) // estamos contando cada aresta duas vezes
        m /= 2;
    vector<vector<int>> matrizIncidencia(n, vector<int>(m, 0)); // as arestas serão indexadas de 0 a m-1
    checkAlocouMemoria(matrizIncidencia);
    if(isDirecionado){
        for(int i=1; i<n; i++){
            for(auto &par: listaAdjacencia[i]){
                matrizIncidencia[i][idAresta] = -1;
                matrizIncidencia[par.first][idAresta] = 1;
                idAresta++;
            }
        }
    }
    /* Se não é direcionado temos alguns problemas para a matriz de incidencia, na lista de adjacencia 
    temos as arestas repetidas e temos que diferenciar arestas paralelas. Para corrigir isso vamos 
    fazer um mapeamento das arestas */
    else{
        map<pair<int, int>, int> identificacaoAresta;
        for(int i=1; i<n; i++){
            for(auto &par: listaAdjacencia[i]){
                // temos um aresta de u para v, onde o idenficador de u <= v
                pair<int, int> aresta = make_pair(min(i, par.first), max(i, par.first));
                // checo se essa aresta não apareceu nenhuma vez, para colocar ela no mapeamento
                if(identificacaoAresta.find(aresta) == identificacaoAresta.end())
                    identificacaoAresta[aresta] = 0; // a aresta não apareceu nenhuma vez
                // aumento a quantidade de vez que a aresta apareceu
                identificacaoAresta[aresta]++;
            }
        }
        // agora vamos colocar na matriz de incidencias
        for(auto &par: identificacaoAresta){
            while(par.second){ // devido a arestas paralelas
                matrizIncidencia[ par.first.first ][idAresta] = 1;
                matrizIncidencia[ par.first.second ][idAresta] = 1;
                idAresta++;
                par.second -= 2; // -2 devido a duplicação
            }
        }
    }
    return matrizIncidencia;
}

void showGrafoListaAdjacencia(vector<vector<edge>> &listaAdjacencia){
    cout << "\033[1;33mLista de Adjacencias:\n\033[0m";
    for(int i=1; i < (int)listaAdjacencia.size(); i++){
        cout << "\033[1;33m" << i << " -> \033[0m";
        bool isFirst = 1;
        for(auto &par: listaAdjacencia[i]){
            if(!isFirst) cout << ", ";
            cout << "{" << par.first << ", " << fixed << setprecision(2) << par.second << "}";
            isFirst = 0;
        }
        cout << "\n";
    }
}

void showGrafoMatrizAdjacencia(vector<vector<int>> &matrizAdjacencia){
    int n = matrizAdjacencia.size();
    int qtdDigitos = countDigitos(n-1);
    cout << "\033[1;33mMatriz de Adjacencias:\n\033[0m";
    for(int i=1; i<n; i++){
        if(i == 1)
            // um espaço para colocar os vertices na primeira coluna
            cout << "\033[1;32m" << setw(qtdDigitos+2) << setfill(' ') << i << "\033[0m" << ' ';
        else
            cout << "\033[1;32m" << setw(qtdDigitos) << setfill(' ') << i << "\033[0m" << (i+1 == n? '\n' : ' ');
    }
    for(int i=1; i<n; i++){
        cout << "\033[1;32m" << setw(qtdDigitos) << setfill(' ') << i << "\033[0m" << ' ';
        for(int j=1; j<n; j++)
            cout << setw(qtdDigitos) << setfill(' ') << matrizAdjacencia[i][j] << (j+1 == n? '\n' : ' ');
    }
}

void showGrafoMatrizPesos(vector<vector<double>> &matrizPesos){
    int n = matrizPesos.size();
    int qtdDigitos = countDigitos(n-1);
    // vamos ver o maior pesso para formatar a saida
    for(int i=1; i<n; i++)
        for(int j=1; j<n; j++)
            qtdDigitos = max(qtdDigitos, countDigitos(matrizPesos[i][j])+ 3);
    cout << "\033[1;33mMatriz de Pesos:\n\033[0m";
    for(int i=1; i<n; i++){
        if(i == 1)
            // um espaço para colocar os vertices na primeira coluna
            cout << "\033[1;32m" << setw(qtdDigitos+2) << setfill(' ') << right << i << "\033[0m" << ' ';
        else
            cout << "\033[1;32m" << setw(qtdDigitos) << setfill(' ') << right << i << "\033[0m" << (i+1 == n? '\n' : ' ');
    }
    for(int i=1; i<n; i++){
        cout << "\033[1;32m" << setw(qtdDigitos) << setfill(' ') << left << i << "\033[0m" << ' ';
        for(int j=1; j<n; j++)
            cout << setw(qtdDigitos) << setfill(' ') << left << fixed << setprecision(2) << matrizPesos[i][j] << (j+1 == n? '\n' : ' ');
    }
}

void showGrafoMatrizIncidencia(vector<vector<int>> &matrizIncidencia){
    int n = matrizIncidencia.size(); // numero vertices
    int m = matrizIncidencia[1].size(); // numeros arestas
    // pegamos o maximo de digitos que precisamos para representar um numero, (2 devido ao -1)
    int qtdEspacos = max(2, max(countDigitos(n-1), countDigitos(m-1)));
    cout << "\033[1;33mMatriz de Incidencias:\n\033[0m";
    for(int i=0; i<m; i++){
        if(i == 0)
            // um espaço para colocar os vertices na primeira coluna
            cout << "\033[1;34m" << setw(qtdEspacos+2) << setfill(' ') << right << i << "\033[0m" << ' ';
        else
            cout << "\033[1;34m" << setw(qtdEspacos) << setfill(' ') << right << i << "\033[0m" << (i+1 == m? '\n' : ' ');
    }
    for(int i=1; i<n; i++){
        cout << setw(qtdEspacos) << setfill(' ') << left << "\033[1;34m" << i << " \033[0m";
        /*Não foi possivel usar cores diferentes para -1, 1 e 0. O uso de cores bagunçava a formatação '-' */
        for(int j=0; j<m; j++){
            cout << setw(qtdEspacos) << setfill(' ') << right << matrizIncidencia[i][j] << ((j+1 == m)? '\n' : ' ');
        }
    }
}

void showGrafoRepresentacaoVetorial(vector<vector<edge>> &listaAdjacencia){
    /*Não vamos usar os vetores em si, mas se aproveitar da lista de adjacencia,
    já que essa representação é a concatenação das listas*/
    int n = listaAdjacencia.size();
    vector<int> qtdEspacos(n, 0); // todas as posições com 0
    // vamos contabilizar os espaços necessários para formatar
    cout << "\033[1;33mRepresentacao Vetorial:\n\033[0m";
    for(int i=1; i<n; i++){
        for(auto &par: listaAdjacencia[i])
            qtdEspacos[i] += countDigitos(par.first)+1;
        qtdEspacos[i] -= 1; // retira um espaço a mais que tem
        
    }
    // 1º vetor
    for(int i=1; i < n; i++){
        if(i & 1){ // é impar
            cout << "\033[1;32m" << setw(qtdEspacos[i]) << setfill(' ') << listaAdjacencia[i].size() << "\033[0m" << (i+1 == n? '\n' : ' ');
        }
        else{
            cout << "\033[1;34m" << setw(qtdEspacos[i]) << setfill(' ') << listaAdjacencia[i].size() << "\033[0m" << (i+1 == n? '\n' : ' ');
        }
    }
    // 2º vetor
    for(int i=1; i < n; i++){
        bool isFirst = 1;
        for(auto &par: listaAdjacencia[i]){
            if(!isFirst) cout << ' ';
            if(i & 1){ // é impar
                cout << "\033[1;32m" << par.first << "\033[0m";
            }
            else{
                cout << "\033[1;34m" << par.first << "\033[0m";
            }
            isFirst = 0;
        }
        if(listaAdjacencia[i].size() == 0) cout << ' '; // corrigir espaços para vertices sem vizinhos
        cout << (i+1 == n? '\n' : ' ');
    }
}


int main(int argc, char* argv[]){
    ios_base::sync_with_stdio(0); cin.tie(nullptr); // faz uma otimização de leitura
    // checar se são fornecidos todos os argumentos
    if (argc < 6 || argc > 6) {
        cerr << "\033[1;31m Para executar esse programa use:\033[0m " << argv[0] <<
        " <read> <nomeTeste(inst-*)> <show> <estrutura(-[ma, mi, la, rv, mp])> <isDirecionado(0/1)>\n"; return 1;
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
        "\033[1;31m nao eh valido\n\033[0m"; 
        showComandosEstruturas();
        return 1;
    }
    if(strcmp(argv[5], "0") != 0 && strcmp(argv[5], "1") != 0){
        cerr << "\033[1;31m O comando:\033[0m " << argv[5] <<
        "\033[1;31m nao eh valido\n\033[0m"; return 1;
    }
    ifstream arquivoInput(nomeArquivo);
    if(arquivoInput.is_open() == false){
        cerr << "\033[1;31m Nao foi possivel abrir o arquivo:\033[0m " << argv[2] <<
        "\033[1;33m verifique se o arquivo existe!\n\033[0m"; return 1;
    }
    // processar os comandos
    vector<vector<edge>> listaAdjacencia = readGrafo(nomeArquivo, isDirecionado);
    if(tipoEstrutura == "-la")
        showGrafoListaAdjacencia(listaAdjacencia);
    else if(tipoEstrutura == "-ma"){
        vector<vector<int>> matrizAdjacencia = listaAdjacencia2matrizAdjacencia(listaAdjacencia);
        showGrafoMatrizAdjacencia(matrizAdjacencia);
    }
    else if(tipoEstrutura == "-mi"){
        vector<vector<int>> matrizIncidencia = listaAdjacencia2matrizIncidencia(listaAdjacencia, isDirecionado);
        showGrafoMatrizIncidencia(matrizIncidencia);
    }
    else if(tipoEstrutura == "-rv"){
        showGrafoRepresentacaoVetorial(listaAdjacencia);
    }
    else if(tipoEstrutura == "-mp"){
        vector<vector<double>> matrizPesos = listaAdjacencia2matrizPesos(listaAdjacencia);
        showGrafoMatrizPesos(matrizPesos);
    }
    return 0;
}
