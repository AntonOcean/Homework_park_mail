//Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
//Требуемое время работы O((N + M)log N),
// где N – количество городов, M – известных дорог между ними.
//N ≤ 10000, M ≤ 250000.
//Длина каждой дороги ≤ 10000.

#include <vector>
#include <set>
#include <iostream>

using namespace std;

const int INFINITY_WAY = 100010;

// Храним как лист
class Graph {
 public:
  explicit Graph(size_t vertices_count);
  Graph(const Graph &) = delete;
  Graph(Graph &&) = delete;
  Graph &operator=(const Graph &) = delete;
  Graph &operator=(Graph &&) = delete;

  void addEdge(int from, int to, int weight);
  size_t getSize() const { return edges_.size(); };
  void getVertices(int vertex, vector<pair<int, int>> &vertices) const;

 private:
  // вектор пар - вершина, вес
  vector<vector<pair<int, int>>> edges_;
};

Graph::Graph(size_t vertices_count) :
    edges_(vertices_count, vector<pair<int, int>>()) {}

void Graph::addEdge(int from, int to, int weight) {
  pair<int, int> for_to(from, weight);
  pair<int, int> for_from(to, weight);

  edges_[to].push_back(for_to);
  edges_[from].push_back(for_from);
}

void Graph::getVertices(int vertex, vector<pair<int, int>> &vertices) const {
//  vertices = edges_[vertex];
  vertices.clear();
  for (const pair<int,int> &i : edges_[vertex])
    vertices.push_back(i);
}

// Алгоритм Дейкстры
int find_best_way(Graph &graph, int from, int to) {
  // Длина путей, по умолчанию - бесконечность
  vector<int> ways_length(graph.getSize(), INFINITY_WAY);
  ways_length[from] = 0;

  // мн-во пар - веc, вершина
  set<pair<int, int>> queue;
  pair<int, int> first_init(0, from);
  queue.insert(first_init);

  while (!queue.empty()) {

    int current_vertex = (queue.begin())->second;
    queue.erase(queue.begin());
//    cout << "Берем вершину " << current_vertex << endl;

    // Релаксируем
    vector<pair<int, int>> vertices;
    graph.getVertices(current_vertex, vertices);

    for (pair<int, int> current_pair : vertices) {

      if (ways_length[current_pair.first] > ways_length[current_vertex] + current_pair.second) {

        if (ways_length[current_pair.first] != INFINITY_WAY) {
          pair<int, int> vertex_del(ways_length[current_pair.first], current_pair.first);
//          cout << "- вершина:" << vertex_del.second << " вес:" << vertex_del.first << endl;
          queue.erase(vertex_del);
        }

        ways_length[current_pair.first] = ways_length[current_vertex] + current_pair.second;

        pair<int, int> vertex_in(ways_length[current_pair.first], current_pair.first);
//        cout << "+ вершина:" << vertex_in.second << " вес:" << vertex_in.first << endl;
        queue.insert(vertex_in);

      }

    }

  }

  if (ways_length[to] != INFINITY_WAY) {
    return ways_length[to];
  }
  return -1;
}

int main() {
  size_t graph_size = 0;
  int count_edge = 0;
  cin >> graph_size >> count_edge;
  Graph graph(graph_size);

  int from = 0;
  int to = 0;
  int weight = 0;

  for (int i = 0; i < count_edge; i++) {
    cin >> from >> to >> weight;
    graph.addEdge(from, to, weight);
  }

  cin >> from >> to;
  cout << find_best_way(graph, from, to);
  return 0;
}


//6
//9
//0 3 1
//0 4 2
//1 2 7
//1 3 2
//1 4 3
//1 5 3
//2 5 3
//3 4 4
//3 5 6
//0 2

//Берем вершину 0
//+ вершина:3 вес:1
//+ вершина:4 вес:2
//Берем вершину 3
//+ вершина:1 вес:3
//+ вершина:5 вес:7
//Берем вершину 1
//+ вершина:2 вес:10
//- вершина:5 вес:7
//+ вершина:5 вес:6
//Берем вершину 2
//Берем вершину 4
//Берем вершину 5
//- вершина:2 вес:10
//+ вершина:2 вес:9
//Берем вершину 2