#include <iostream>
#include <math.h>
#include <map>

#define DEBUG 0

struct node {
    float x;
    float y;
    node(float a, float b) : x(a), y(b) {}

};

std::ostream & operator<<(std::ostream & os, const node & d) {
    os << d.x << ',' << d.y;
    return os;
}       

// lite headers, titta l�ngst ner i filen :)
float distance(float const x1, float const y1, float const x2, float const y2); 
float distance(const node & n1, const node & n2);
void swap(int x, int y, int  path[]);
void printPath(node * nodes[], int path[], int numNodes);
void reverse(int a, int b, int path[]);


float pathLength(node * nodes[], int path[], int numNodes) {
    float sum = 0;
    for (int i = 1; i < numNodes; i++) {
        sum += distance(
                *nodes[path[i-1]], 
                *nodes[path[  i]]
                );
    }
    return sum;
}

float pathLength(int a, int b, node * nodes[], int path[]) {
    float sum = 0;
    for (;a < b; a++) {
        sum += distance(*nodes[path[a]], *nodes[path[a+1]]);
    }
    return sum;
}

inline float distance(int a, int b, node * nodes[], int path[]) {
    return distance(*nodes[path[a]], *nodes[path[b]]);
}


// using namespace std;

int main() {
    int numNodes;
    scanf("%d", &numNodes);
    // std::cerr << "numNodes:\t" << numNodes << std::endl;
    node * nodes[numNodes];
    int used[numNodes];
    std::multimap<float, int> neighbors[numNodes];

    unsigned int static neighborsToCheck = 100;

    float x,y;
    for (int i = 0; i < numNodes; i++) {
        scanf("%f", &x);
        scanf("%f", &y);
        nodes[i] = new node(x,y);
        used[i] = false;
    }

    // Vi letar reda p� alla grannar vi har
    for (int i = 0; i < numNodes; i++) {
        for (int n = 0; n < numNodes; n++) {
            neighbors[i].insert(std::pair<float, int>(
                        distance(*nodes[i], *nodes[n]),
                        n));
            if (neighbors[i].size() > neighborsToCheck) {
                // std::cout << "LOL:\t" << neighbors[i].size();
                neighbors[i].erase(--neighbors[i].end());
                // std::cout << "\tLOL:\t" << neighbors[i].size() << std::endl;
                // sleep(1);
            }
        }
    }



    /*
     * Printar ut alla grannar
     *
     */
     // std::multimap<float, int>::iterator iter;
// 
     // for (int i = 0; i < numNodes; i++) {
     // std::cout << "\t\t\t\t" << i << std::endl;
     // for (iter = neighbors[i].begin(); iter != neighbors[i].end(); iter++) {
     // std::cout << iter->first << " => " << iter->second << std::endl;
     // }
     // }

    // testa n�got greedy


    int path[numNodes];

    path[0] = 0;
    used[0] = true;
    int best;
    // greedy
    for (int i = 1; i < numNodes; i++) {
        best = -1;
        for (int j = 0; j < numNodes; j++) {
            if (!used[j] && (best == -1 || distance(*nodes[path[i-1]], *nodes[j]) < distance(*nodes[path[i-1]], *nodes[best]))) {
                best = j;
            }
        }
        path[i] = best;
        used[best] = true;
    }

    // just some path
    // for (int i = 0; i < numNodes; i++) {
    // path[i] = i;
    // }


    int pos[numNodes];
    /*
     * H�ll koll p� vart i en tur en nod �r
     *
     * Vi beh�ver detta f�r att kunna sl� upp saker i O(1) tid 
     * n�r vi ska titta p� grannar, iom att vi inte vet vart i turen
     * en granne finns
     */
    for (int i = 0; i < numNodes; i++) {
        pos[path[i]] = i;
    }


    // Random start

    // std::cout << rand() << std::endl;

    /* 2-opt, i hope
     *
     * Testa k�r tills vi inte kan f�rb�ttra n�got mer
     *
     * om vi bara s�tter i = 0 n�r vi har en f�rb�ttring s� f�r vi TLE
     *
     * Kan testa att varje g�ng vi hittar en f�rb�ttring s�tter vi en bool till true
     * s� att n�r vi har loopat igenom i->n s� g�r vi det igen, tills vi inte har n�gra f�rb�ttringar kvar
     *
     * En uppsnabbning man kan g�ra �r med grannlistor, som man kalkylerar i b�rjan och sedan bara g�r 
     * f�rb�ttringar med de n�rmaste grannarna �s kanske vi kan hinna :)
     */

    std::multimap<float, int>::iterator it;
    int a, b, tmp1, tmp2;
    unsigned int n = 0;
    bool improvement = true;
    while (improvement) {
        improvement = false;
        for (int i = 0; i < numNodes-1; i++) {
            // check the nearest neighbors
            for (it = neighbors[i].begin(); it != neighbors[i].end() && n != neighborsToCheck; it++, n++) {
                if (!(pos[it->second] >= numNodes) && !(pos[i]+1 >= numNodes) &&
                        distance(pos[i], pos[i]+1, nodes, path) + distance(pos[it->second], (pos[it->second]+1)%numNodes, nodes, path) >
                        distance(pos[i], pos[it->second], nodes, path) + distance(pos[i]+1, (pos[it->second]+1)%numNodes, nodes, path)) {
                    // reverse
                    improvement = true;

                    a = pos[i]+1;
                    b = pos[it->second];
                    while (a < b) {
                        // swap 
                        tmp1 = path[a];
                        tmp2 = path[b];

                        path[a] = tmp2;
                        path[b] = tmp1;
                        // end swap

                        // deras positioner i v�r path byter ocks� plats
                        pos[tmp1] = b;
                        pos[tmp2] = a;
                        a++; b--;
                    }
                }
            }
        }
    }

    // for (int i = 0; i < numNodes-1; i++) {
    // for (int n = i+3; n < numNodes-1; n++) {
    // if (distance(*nodes[path[i+0]], *nodes[path[i+1]]) + distance(*nodes[path[n+0]], *nodes[path[n+1]]) >
    // distance(*nodes[path[i+0]], *nodes[path[n+0]]) + distance(*nodes[path[i+1]], *nodes[path[n+1]])) {
    // 
    // if (DEBUG) {
    // std::cerr << "\treverse(" << i+1 << "," << n << ")\t=> " << pathLength(nodes, path, numNodes);
    // }
    // reverse(i+1, n+0, path);
    // if (DEBUG) {
    // std::cerr << " - " << pathLength(nodes, path, numNodes) << std::endl;
    // if (DEBUG >= 2)
    // printPath(nodes, path, numNodes);
    // }
    // // i = 0;
    // }
    // }
    // }

    // reverse(2,5, path);
    // printPath(nodes, path, numNodes);

    // output result
    for (int i = 0; i < numNodes; i++) {
        std::cout << path[i] << std::endl;
    }
    std::cerr << "tour length:\t" << pathLength(nodes, path, numNodes) << std::endl;

}

float distance(float const x1, float const y1, float const x2, float const y2)  { 
    return sqrt(pow((x2-x1), 2) + pow((y2-y1),2));
}

float distance(const node & n1, const node & n2)  {
    return distance(n1.x, n1.y, n2.x, n2.y);
}


void swap(int x, int y, int  path[]) {
    int tmp = path[x];
    path[x] = path[y];
    path[y] = tmp;
}

void printPath(node * nodes[], int path[], int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        std::cout << "path[" << i << "] => " << path[i] << std::endl;
    }
}

void reverse(int a, int b, int path[]) {
    b++;
    for (;a<--b;a++) {
        if (DEBUG >= 2) 
            std::cerr << "swap(" << a << "," << b << ")" << std::endl;
        swap(a,b, path);
    }
}
