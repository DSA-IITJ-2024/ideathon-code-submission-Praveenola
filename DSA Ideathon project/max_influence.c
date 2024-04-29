
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LENGTH 100
#define MAX_SIZE 4039 // Maximum number of nodes in the Facebook dataset

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

// Function to generate a random double between 0 and 1
double randomDouble() {
    return (double)rand() / RAND_MAX;
}

// Function to calculate the upper bound of influence for a node at distance n
int upperBoundOfInfluence(int node, Node* graph[], int n) {
    int influenceCount = 0;
    int visited[MAX_SIZE] = {0}; // Array to track visited nodes
    int queue[MAX_SIZE]; // Queue for BFS traversal
    int front = 0, rear = 0;

    queue[rear++] = node; // Enqueue the starting node
    visited[node] = 1; // Mark the starting node as visited

    // Perform BFS traversal up to distance n
    while (front != rear && n > 0) {
        int levelNodes = rear - front;
        while (levelNodes > 0 && n > 0) {
            int currentNode = queue[front++];
            Node* current = graph[currentNode];
            while (current != NULL) {
                int neighbor = current->userId;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                    influenceCount++; // Increment influence count for each newly visited node
                }
                current = current->next;
            }
            levelNodes--;
        }
        n--; // Decrement distance
    }

    return influenceCount;
}

// Function to calculate the Maximum Influence Degree (MID) for a node
int maximumInfluenceDegree(int node, Node* graph[], int D) {
    int MID = 0;
    for (int n = 1; n <= D; n++) {
        MID += upperBoundOfInfluence(node, graph, n);
    }
    return MID;
}

// Function to read the file and construct the graph
void constructGraph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Initialize an array of linked lists to represent the graph (adjacency list)
    Node* graph[MAX_SIZE]; // The number of nodes in the Facebook dataset is known
    for (int i = 0; i < MAX_SIZE; i++) {
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

    // Define the network diameter (D) for the upper bound calculation
    int D = 5; // This is just a placeholder, replace it with the actual diameter of your graph

    // Open the output file for writing
    FILE *outputFile = fopen("output_graph.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Calculate and print the Maximum Influence Degree (MID) for each node
    fprintf(outputFile, "Maximum Influence Degrees (MID):\n");
    for (int i = 0; i < MAX_SIZE; i++) {
        int MID = maximumInfluenceDegree(i, graph, D);
        fprintf(outputFile, "Node %d: %d\n", i, MID);
    }

    fclose(outputFile);
}

int main() {
    printf("Graph_fb program started.\n");
    const char *filename = "facebook_combined.txt";
    constructGraph(filename);
    return 0;
}
