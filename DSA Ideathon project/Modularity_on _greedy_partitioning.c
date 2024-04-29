#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NUM_NODES 4039  // Number of nodes in the Facebook dataset

// Structure to represent a node in the graph
typedef struct Node {
    int userId;
    struct Node *next;
} Node;

// Function to create a new node
Node* createNode(int userId) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->userId = userId;
    newNode->next = NULL;
    return newNode;
}

// Function to add a new edge (friendship) to the graph
void addEdge(Node* graph[], int source, int target) {
    Node* newNode = createNode(target);
    newNode->next = graph[source];
    graph[source] = newNode;
}

// Function to read the file and construct the graph
void constructGraph(const char *filename, Node* graph[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Initialize an array of linked lists to represent the graph (adjacency list)
    for (int i = 0; i < NUM_NODES; i++) {
        graph[i] = NULL;  // Initialize all pointers to NULL
    }

    // Read the file line by line
    int source, target;
    while (fscanf(file, "%d %d", &source, &target) != EOF) {
        // Add the friendship (edge) to the graph
        addEdge(graph, source, target);
        addEdge(graph, target, source); // Add reverse edge for undirected graph
    }

    fclose(file);
}

// Function to calculate the degree of a node
int nodeDegree(Node* graph[], int nodeId) {
    int degree = 0;
    Node* current = graph[nodeId];
    while (current != NULL) {
        degree++;
        current = current->next;
    }
    return degree;
}

// Function to calculate the modularity of a partition
float calculateModularity(Node* graph[], bool partition[]) {
    float modularity = 0.0;
    int totalEdges = 0;
    for (int i = 0; i < NUM_NODES; i++) {
        totalEdges += nodeDegree(graph, i);
    }
    totalEdges /= 2;

    for (int i = 0; i < NUM_NODES; i++) {
        Node* current = graph[i];
        while (current != NULL) {
            if (partition[i] == partition[current->userId]) {
                modularity += 1.0 - ((float)(nodeDegree(graph, i) * nodeDegree(graph, current->userId)) / (2 * totalEdges));
            }
            current = current->next;
        }
    }
    return modularity / (2 * totalEdges);
}

// Function to find the node with maximum influence centrality
int findMaxInfluenceCentrality(Node* graph[], bool visited[]) {
    float maxCentrality = -1.0;
    int maxNode = -1;

    for (int i = 0; i < NUM_NODES; i++) {
        if (!visited[i]) {
            float centrality = (float)nodeDegree(graph, i);
            Node* current = graph[i];
            while (current != NULL) {
                centrality += nodeDegree(graph, current->userId);
                current = current->next;
            }
            if (centrality > maxCentrality) {
                maxCentrality = centrality;
                maxNode = i;
            }
        }
    }

    return maxNode;
}

// Function to perform greedy partitioning
void greedyPartitioning(Node* graph[], bool partition[]) {
    bool visited[NUM_NODES] = {false};
    int numCommunities = 0;

    while (numCommunities < NUM_NODES) {
        int seedNode = findMaxInfluenceCentrality(graph, visited);
        if (seedNode == -1) break;

        partition[seedNode] = true;
        visited[seedNode] = true;

        Node* current = graph[seedNode];
        while (current != NULL) {
            partition[current->userId] = true;
            visited[current->userId] = true;
            current = current->next;
        }

        numCommunities++;
    }
}

int main() {
    printf("Graph partitioning program started.\n");
    const char *filename = "facebook_combined.txt";
    Node* graph[NUM_NODES]; // The number of nodes in the Facebook dataset is known
    constructGraph(filename, graph);

    // Perform greedy partitioning
    bool partition[NUM_NODES] = {false}; // Initialize partition
    greedyPartitioning(graph, partition);

    // Calculate modularity
    float modularity = calculateModularity(graph, partition);
    printf("Modularity of the graph partitioning: %.4f\n", modularity);

    printf("Graph construction and partitioning completed.\n");
    return 0;
}
