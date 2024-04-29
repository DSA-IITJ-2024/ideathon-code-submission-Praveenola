#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_NODES 4039
#define MAX_LINE_LENGTH 100

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

    // Initialize the graph
    for (int i = 0; i < NUM_NODES; i++) {
        graph[i] = NULL;  // Initialize all pointers to NULL
    }

    // Read the file line by line
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Parse the line to extract source and target user IDs
        int source, target;
        sscanf(line, "%d %d", &source, &target);
        
        // Add the friendship (edge) to the graph
        addEdge(graph, source, target);
    }

    fclose(file);
}

// Function to compute the cut size between two partitions
int computeCutSize(Node* graph[], bool partition[]) {
    int cutSize = 0;
    for (int i = 0; i < NUM_NODES; i++) {
        Node* current = graph[i];
        while (current != NULL) {
            if (partition[i] != partition[current->userId]) {
                cutSize++;
            }
            current = current->next;
        }
    }
    return cutSize / 2;  // Each edge is counted twice
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

// Function to perform partitioning
void performPartitioning(Node* graph[], bool partition[]) {
    // Dummy partitioning for demonstration purposes
    for (int i = 0; i < NUM_NODES; i++) {
        partition[i] = (i < NUM_NODES / 2); // Partitioning the nodes into two halves
    }
}

int main() {
    const char *filename = "facebook_combined.txt";
    Node* graph[NUM_NODES];
    constructGraph(filename, graph);

    // Perform partitioning
    bool partition[NUM_NODES];
    performPartitioning(graph, partition);

    // Calculate modularity
    float modularity = calculateModularity(graph, partition);
    printf("Modularity of the graph partitioning: %.4f\n", modularity);

    // Clean up memory
    for (int i = 0; i < NUM_NODES; i++) {
        Node* current = graph[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    return 0;
}
