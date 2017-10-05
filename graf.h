#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
class ActorPairs {
    int actor1_id_graf;
    string actor1_id;
    int actor2_id_graf;
    string actor2_id;
    int movies_number;
public:
    explicit ActorPairs(int actor1_id_graf, int actor2_id_graf,
    int movies_number, string actor1_id, string actor2_id) {
        this->actor1_id_graf = actor1_id_graf;
        this->actor2_id_graf = actor2_id_graf;
        this->movies_number = movies_number;
        this->actor1_id = actor1_id;
        this->actor2_id = actor2_id;
    }
    ActorPairs(const ActorPairs& obj) {
        this->actor1_id_graf = obj.actor1_id_graf;
        this->actor2_id_graf = obj.actor2_id_graf;
        this->movies_number = obj.movies_number;
        this->actor1_id = obj.actor1_id;
        this->actor2_id = obj.actor2_id;
    }
    ActorPairs operator=(const ActorPairs& obj) {
        this->actor1_id_graf = obj.actor1_id_graf;
        this->actor2_id_graf = obj.actor2_id_graf;
        this->movies_number = obj.movies_number;
        this->actor1_id = obj.actor1_id;
        this->actor2_id = obj.actor2_id;
        return *this;
    }
    int get_movies_number() const{
        return movies_number;
    }
    int get_actor1_id_graf() const{
        return actor1_id_graf;
    }
    int get_actor2_id_graf() const{
        return actor2_id_graf;
    }
    string get_actor1_id() const{
        return actor1_id;
    }
    string get_actor2_id() const{
        return actor2_id;
    }
    bool operator==(const ActorPairs& obj) const{
        return (movies_number == obj.movies_number);
    }

    bool operator<(const ActorPairs& obj) const{
        if (movies_number != obj. movies_number) {
            return (movies_number > obj.movies_number);
        } else {
            return actor1_id + actor2_id < obj.actor1_id + obj.actor2_id;
        }

    }

    bool operator>(const ActorPairs& obj) {
        return (movies_number > obj.movies_number);
    }

    bool operator!=(const ActorPairs& obj) {
        return (movies_number != obj.movies_number);
    }

    bool operator>=(const ActorPairs& obj) {
        return (movies_number >= obj.movies_number);
    }

    bool operator<=(const ActorPairs& obj) {
        return (movies_number <= obj.movies_number);
    }
};
struct Node {
    std::vector<int> neighbors;
    std::vector<int> movies_played;
};

class Graph {
 private:
   std::vector<Node> nodes;

 public:

  void add_node(int node){
    Node aux;
    nodes.push_back(aux) ;
  }
  void add_edge(int a, int b) {
    std::vector<int>::iterator it ;
    it = find(nodes[a].neighbors.begin(),nodes[a].neighbors.end(),b);
	   if (it == nodes[a].neighbors.end()) {
	   		nodes[a].neighbors.push_back(b);
	   		nodes[a].movies_played.push_back(1);
	   	} else {
	   		unsigned int dist = std::distance(nodes[a].neighbors.begin(), it);
	   		nodes[a].movies_played[dist] += 1;
	   	}
    it = find(nodes[b].neighbors.begin(),nodes[b].neighbors.end(),a);
     	if (it == nodes[b].neighbors.end()) {
	   		nodes[b].neighbors.push_back(a);
	   		nodes[b].movies_played.push_back(1);
	   	} else {
	   		unsigned int dist = std::distance(nodes[b].neighbors.begin(), it);
	   		nodes[b].movies_played[dist] += 1;
	   	}
  }

  std::vector<ActorPairs> return_all_edges(std::map<int, string> mapForIdGraph) {
      std::vector<ActorPairs> tempVector;
      for (unsigned int i = 0; i < nodes.size(); i++) {
          for (unsigned j = 0; j < nodes[i].neighbors.size(); j++) {
              int iInt = (int) i;

              int idNeig = nodes[iInt].neighbors[j];

              if (mapForIdGraph.find(i) != mapForIdGraph.end()
              && mapForIdGraph.find(idNeig) != mapForIdGraph.end()) {
                  string actor1 = mapForIdGraph.find(i)->second;
                  string actor2 = mapForIdGraph.find(idNeig)->second;
                  std::vector<Node>::iterator it;

                  int movie_num = nodes[iInt].movies_played[j];

                  if (actor1 < actor2) {
                      ActorPairs tempActorPairs(iInt, idNeig, movie_num, actor1,
                      actor2);
                      tempVector.push_back(tempActorPairs);
                  }

              }
          }
      }
      return tempVector;
  }
  // Daca da timeout e din cauza copierii grafului
  std::vector<Colleague> return_colleagues(int id, std::map<int, string> mapForIdGraph) {
      std::vector<Colleague> tempVector;
      for (unsigned int i = 0; i < nodes[id].neighbors.size()
      && i < nodes[id].movies_played.size(); i++) {
          Colleague tempColleague(mapForIdGraph.find(nodes[id].neighbors[i])->second,
          nodes[id].movies_played[i]);
          tempVector.push_back(tempColleague);
      }
      return tempVector;
  }
  void print() {
  	unsigned int i;
  	unsigned int j;
  	for (i = 0; i < nodes.size(); i++) {
  		std::cout<<i<<" = ";
  		for (j = 0; j < nodes[i].neighbors.size(); j++) {
  			std::cout<<nodes[i].neighbors[j]<<" - "<<nodes[i].movies_played[j]<<"\n";
  		}
  	}
  }

  int return_edge(int a, int b) {
      std::vector<int>::iterator it;

      it = find(nodes[a].neighbors.begin(),nodes[a].neighbors.end(),b);
      unsigned int dist= -1;
      if(it != nodes[a].neighbors.end()) {
           //std::cout<<dist<<" ";
        dist = std::distance(nodes[a].neighbors.begin(), it);
        return nodes[a].movies_played[dist];}
      //std::cout<<a<<" "<<b<<" "<<dist<<"DA ";
      return 0;
  }
  std::vector<int> get_movies_played(int node) {
      return nodes[node].movies_played;
  }

  std::vector<int> get_neighbors(int node) {
     return nodes[node].neighbors;
  }

  std::vector<int> bfs(int node) {
     unsigned int i;
     int element;
     std::queue<int> Q;
     std::vector<int> a2nd;
     int *distanta = new int[nodes.size()];
     for (i = 0; i < nodes.size(); i++) {
       distanta[i] = -1;
     }
     distanta[node] = 0;
     element = node;
     Q.push(node);
     while (!Q.empty() && distanta[element] < 2) {
       element = Q.front();
       //std::cout<<element<<" "<<distanta[element]<<std::flush;
       Q.pop();
       std::vector<int> vecini = get_neighbors(element);
       if (distanta[element] < 2) {
         for (i = 0; i < vecini.size(); i++) {
            if (distanta[vecini[i]] == -1) {
              distanta[vecini[i]] = distanta[element] + 1;
              if (distanta[vecini[i]] == 2)
                a2nd.push_back(vecini[i]);
              Q.push(vecini[i]);
            }
         }
      }

     }

    delete[] distanta;
    return a2nd;

  }
};
