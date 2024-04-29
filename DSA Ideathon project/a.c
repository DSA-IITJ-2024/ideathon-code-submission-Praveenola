#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h" // Include graph structure definition

// Function to create a new node
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    // Add edge from src to dest
    Node* newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Add edge from dest to src (assuming undirected graph)
    newNode = createNode(src);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

#define NUM_PARTITIONS 4 

// Function to perform random graph partitioning
void partitionGraph(Graph* graph, int numNodes, FILE* outputFile){
    srand(time(NULL)); // Seed random number generator

    // Assign each node to a random partition
    int partition[numNodes];
    for (int i = 0; i < numNodes; i++) {
        partition[i] = rand() % NUM_PARTITIONS; // Random partition assignment
    }

    // Output partition information to the file
    fprintf(outputFile, "Partitioned Graph:\n");
    for (int i = 0; i < numNodes; i++) {
        fprintf(outputFile, "Node %d -> Partition %d\n", i, partition[i]);
    }

    printf("Partitioned Graph:\n");
    for (int i = 0; i < numNodes; i++) {
        printf("Node %d -> Partition %d\n", i, partition[i]);
    }
}


// Function to read graph data from file and preprocess
void preprocessGraph(const char* filename, Graph* graph, int* numNodes, int* numEdges) {
    // Initialize graph with empty adjacency lists
    for (int i = 0; i < MAX_NODES; i++) {
        graph->adjList[i] = NULL;
    }

    // Open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Read graph data from file and add edges to the graph
    int source, target;
    while (fscanf(file, "%d %d", &source, &target) != EOF) {
        addEdge(graph, source, target);
    }

    // Close the file
    fclose(file);

    // Update number of nodes and edges
    *numNodes = MAX_NODES;
    *numEdges = MAX_EDGES;
}

int main() {
    // Declare graph structure to represent the graph
    Graph graph;
    int numNodes, numEdges;

    // Preprocess the graph data
    preprocessGraph("facebook_combined.txt", &graph, &numNodes, &numEdges);

    // Print some basic statistics
    printf("Graph preprocessing complete.\n");
    printf("Number of nodes: %d\n", numNodes);
    printf("Number of edges: %d\n", numEdges);

    // Open the output file for writing
    FILE* outputFile = fopen("Partitioning_output.txt", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Unable to open file for writing\n");
        exit(1);
    }

    // Partition the graph using functions from Partitioning_of_graph.c
    partitionGraph(&graph, numNodes, outputFile);

    // Close the output file
    fclose(outputFile);

    // Inform the user about the completion of the process
    printf("Graph partitioning complete. Output saved in Partitioning_output.txt\n");

    return 0;
}
