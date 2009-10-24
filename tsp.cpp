#include <iostream>
#include <math.h>

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



float distance(float const x1, float const y1, float const x2, float const y2); 
float distance(const node & n1, const node & n2);

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

float distance(int a, int b, node * nodes[], int path[]) {
    float sum = 0;
    for (;a < b; a++) {
        sum += distance(*nodes[path[a]], *nodes[path[a+1]]);
    }
    return sum;
}

void swap(int x, int y, int  path[]) {
    // path[x]^=(path[y]^=(path[x]^=path[y]));
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

// using namespace std;

int main() {
    int numNodes;
    scanf("%d", &numNodes);
    // std::cerr << "numNodes:\t" << numNodes << std::endl;
    node * nodes[numNodes];
    int used[numNodes];

    float x,y;
    for (int i = 0; i < numNodes; i++) {
        scanf("%f", &x);
        scanf("%f", &y);
        nodes[i] = new node(x,y);
        used[i] = false;
    }

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
    for (int i = 0; i < numNodes-1; i++) {
        for (int n = i+3; n < numNodes-1; n++) {
            if (distance(*nodes[path[i+0]], *nodes[path[i+1]]) + distance(*nodes[path[n+0]], *nodes[path[n+1]]) >
                    distance(*nodes[path[i+0]], *nodes[path[n+0]]) + distance(*nodes[path[i+1]], *nodes[path[n+1]])) {

                if (DEBUG) {
                    std::cerr << "\treverse(" << i+1 << "," << n << ")\t=> " << pathLength(nodes, path, numNodes);
                }
                reverse(i+1, n+0, path);
                if (DEBUG) {
                    std::cerr << " - " << pathLength(nodes, path, numNodes) << std::endl;
                    if (DEBUG >= 2)
                        printPath(nodes, path, numNodes);
                }
                // i = 0;
            }
        }
    }

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
