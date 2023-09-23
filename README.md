# Representacao_Grafos
Este repositório é destinado ao desenvolvimento do trabalho de implementação de representação de grafos 

Para executar esse software use: ./a.out read inst-5 show -la 0
Assim você vera o grafo que está no arquivo inst-5 dentro das Instancias_teste representado por lista de adjacências sendo não direcionado.

Você também pode usar outras instância de teste ou mudar a forma de representação para:
-ma -> matriz de adjacências;
-mi -> matriz de incidências;
-mp -> matriz de pesos;
-la -> lista de adjacências;
-rv -> representação vetorial.

Obs: A instância de teste deve estar no formato descrito a seguir. A primeira linha contém o número n
de vértices. A partir de então, é considerado que o conjunto de vértices do grafo é V = {1,
2, . . . ,n}. A partir da segunda linha, cada linha contém três valores separados por espaços em
branco. Se são os primeiros valores, devemos ter que u, v pertence ao conjuto de Vértices, pois os mesmos
representam a existência de uma aresta entre estes vértices. Já o terceiro valor, p, deve ser um
número real, e representa o peso da aresta.
