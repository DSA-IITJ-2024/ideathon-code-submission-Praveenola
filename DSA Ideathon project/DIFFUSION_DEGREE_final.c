#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 10000

// Structure to represent a node in the graph
typedef struct Node {
    int userId;
    int diffusionDegree;
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
    newNode->diffusionDegree = 0; // Initialize diffusion degree to 0
    newNode->next = NULL;
    return newNode;
}

// Function to add a new edge (friendship) to the graph
void addEdge(Node* graph[], int source, int target) {
    Node* newNode = createNode(target);
    newNode->next = graph[source];
    graph[source] = newNode;
}

// Function to calculate diffusion degree for each node in the graph
void calculateDiffusionDegree(Node* graph[], int numNodes, FILE *outputFile) {
    for (int i = 0; i < numNodes; i++) {
        Node* currentNode = graph[i];
        
        // Check if the node has any neighbors
        if (currentNode == NULL) {
            fprintf(outputFile, "Node %d (Diffusion Degree: 0)\n", i);
            continue;
        }
        
        int diffusionDegree = 0;
        while (currentNode != NULL) {
            Node* neighborNode = graph[currentNode->userId];
            while (neighborNode != NULL) {
                if (neighborNode->userId != i) {
                    diffusionDegree++;
                }
                neighborNode = neighborNode->next;
            }
            currentNode = currentNode->next;
        }
        fprintf(outputFile, "Node %d (Diffusion Degree: %d)\n", i, diffusionDegree);
    }
}

// Function to read the file and construct the graph
void constructGraph(const char *filename, const char *outputFilename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Initialize an array of linked lists to represent the graph (adjacency list)
    Node* graph[4039]; // The number of nodes in the Facebook dataset is known
    for (int i = 0; i < 4039; i++) {
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

    // Open the output file
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Calculate diffusion degree for each node in the graph
    calculateDiffusionDegree(graph, 4039, outputFile);

    // Close the output file
    fclose(outputFile);

    // Free dynamically allocated memory
    for (int i = 0; i < 4039; i++) {
        Node* current = graph[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

int main() {
    printf("Graph_fb program started.\n");
    const char *filename = "facebook_combined.txt";
    const char *outputFilename = "harduk.txt";
    constructGraph(filename, outputFilename);
    printf("Diffusion degrees written to %s\n", outputFilename);
    return 0;
}
