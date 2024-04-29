#include <stdio.h>
#include <stdlib.h>

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
void constructGraph(const char *filename) {
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

    // Print the graph (for demonstration purposes)
    for (int i = 0; i < 4039; i++) {
        printf("Node %d:", i);
        Node* current = graph[i];
        while (current != NULL) {
            printf(" -> %d", current->userId);
            current = current->next;
        }
        printf("\n");
    }

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
    constructGraph(filename);
    return 0;
}
