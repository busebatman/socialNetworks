#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define MAX_PEOPLE_COUNT 100
#define MAX_STRING_LENGTH 100
#define MAX_TOKEN_LENGTH 50
#define MAX_VERTEX_COUNT 100
#define MAX_EDGE_COUNT 500

#define DEBUG 0

typedef struct Vertex {
    char name[MAX_TOKEN_LENGTH];
    int distance;
    char fromName[MAX_TOKEN_LENGTH];
    int betweennessScore;
} Vertex;

typedef struct Edge {
    Vertex from;
    Vertex to;
} Edge;

typedef struct Graph {
    Vertex vertexes[MAX_VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
    int vertexCount;
    int edgeCount;
} Graph;

Graph getGraph () {
    Graph graph;
    graph.vertexCount = 0;
    graph.edgeCount = 0;
    return graph;
}

Vertex getVertex (char vertexName[MAX_TOKEN_LENGTH]) {
    Vertex newVertex;
    strcpy(newVertex.name, "");
    strcpy(newVertex.fromName, "");
    strcpy(newVertex.name, vertexName);
    newVertex.distance = -1;
    newVertex.betweennessScore = 0;
    return newVertex;
}

/* FILE READ */
void readInputLines (char lines[MAX_PEOPLE_COUNT][MAX_STRING_LENGTH], int * lineCount) {
    FILE * fp;
    char line[500];

    fp = fopen("input.txt", "r");
    if (fp == NULL)
    	 exit(EXIT_FAILURE);
       
    *lineCount = 0;
    
	while(fgets(line, 500, (FILE*) fp) != NULL){
		strcpy(lines[*lineCount], line);
        *lineCount = *lineCount + 1;
    }

    fclose(fp);
}

/* CREATE GRAPH */
Vertex findVertex (Graph graph, char vertexName[MAX_TOKEN_LENGTH]) {
    int i;
    for (i = 0; i < graph.vertexCount; i++) {
        if (strcmp((graph.vertexes[i]).name, vertexName) == 0) {
            return graph.vertexes[i];
        }
    }
    printf("VERTEX NOT FOUND !!!!");
    exit(EXIT_FAILURE);
}

void addVertexToGraph (Graph * graph, char vertexName[MAX_TOKEN_LENGTH]) {
    Vertex vertex = getVertex(vertexName);
    (*graph).vertexes[(*graph).vertexCount] = vertex;
    (*graph).vertexCount = (*graph).vertexCount + 1;
}

void addEdgeToGraph (Graph * graph, Vertex from, char toName[MAX_TOKEN_LENGTH]) {
    Vertex to = findVertex((*graph), toName);
    Edge edge;
    edge.to = to;
    edge.from = from;
    (*graph).edges[(*graph).edgeCount] = edge;
    (*graph).edgeCount = (*graph).edgeCount + 1;
}

void addEdgesToGraphFromLine (Graph * graph, Vertex vertex, char line[MAX_STRING_LENGTH]) {
    int charIndex = 0;
    char c = line[charIndex];
    char token[MAX_TOKEN_LENGTH];
    int tokenCharIndex = 0;
    while (1) {
        if (c == ';') {
            tokenCharIndex = 0;
            strcpy(token, "");
        }
        else if (c == ',') {
            token[tokenCharIndex] = 0;
            addEdgeToGraph(graph, vertex, token);
            tokenCharIndex = 0;
            strcpy(token, "");
        }
        else if (c == ' ') {
            strcpy(token, "");
            tokenCharIndex = 0;
        }
        else if (c == 0) {
            token[tokenCharIndex] = 0;
            addEdgeToGraph(graph, vertex, token);
            break;
        }
        else if (c == '\n') {
        }
        else {
            token[tokenCharIndex++] = c;
        }
        charIndex++;
        c = line[charIndex];
    }
}

void addVertexToGraphFromLine (Graph * graph, char line[MAX_STRING_LENGTH]) {
    char c = line[0];
    int charIndex = 1;
    char token[MAX_TOKEN_LENGTH];
    int tokenCharIndex = 0;
    while (c != '\n' && c != 0) {
        if (c == ';') {
            token[tokenCharIndex] = 0;
            addVertexToGraph(graph, token);
            break;
        }
        token[tokenCharIndex++] = c;
        c = line[charIndex++];
    }
}

void createGraph (Graph * graph, char lines[MAX_PEOPLE_COUNT][MAX_STRING_LENGTH], int lineCount) {
    int i;
    for (i = 0; i < lineCount; i++) {
        addVertexToGraphFromLine(graph, lines[i]);
    }
    if (DEBUG == 1) {
        printf("vertexes added to graph\n");
    }
    for (i = 0; i < lineCount; i++) {
        addEdgesToGraphFromLine(graph, (*graph).vertexes[i], lines[i]);
    }
    if (DEBUG == 1) {
        printf("edges added to graph\n");
    }
}

/* ADJACENCY MATRIX */
void printGraph (Graph graph) {
    int i;
    printf("VERTEXES: \n");
    for (i = 0; i < graph.vertexCount; i++) {
        printf("%s\n", graph.vertexes[i].name);
    }
    printf("-----\n");
    printf("EDGES: \n");
    for (i = 0; i < graph.edgeCount; i++) {
        printf("from: %s to: %s\n", graph.edges[i].from.name, graph.edges[i].to.name);
    }
}

void printToken (char token[MAX_TOKEN_LENGTH]) {
    printf("%-7s |", token);
}

void printLongToken (char token[MAX_TOKEN_LENGTH]) {
    printf("%-25s |", token);
}

void printFirstRow (Vertex vertexes[MAX_VERTEX_COUNT], int vertexCount) {
    int i;
    printToken("");
    for (i = 0; i < vertexCount; i++) {
        printToken(vertexes[i].name);
    }
    printf("\n");
}

int vertexHasEdge (Vertex source, Vertex target, Edge edges[MAX_EDGE_COUNT], int edgeCount) {
    int i;
    for (i = 0; i < edgeCount; i++) {
        Edge edge = edges[i];
        if (strcmp(edge.from.name, source.name) == 0) {
            if (strcmp(edge.to.name, target.name) == 0) return 1;
        }   
    }
    return 0;
}

void printRow (Graph graph, Vertex vertex) {
    printToken(vertex.name);
    int vertexCount = graph.vertexCount;
    int i;
    for (i = 0; i < vertexCount; i++) {
        if (vertexHasEdge(vertex, graph.vertexes[i], graph.edges, graph.edgeCount)) {
            printToken("1");
        }
        else {
            printToken("0");
        }
    }
    printf("\n");
}

void printAdjacencyMatrix (Graph graph) {
    printf("ADJACENCY MATRIX:\n");
    int N = graph.vertexCount + 1;
    int r;
    printFirstRow(graph.vertexes, N - 1);
    for (r = 1; r < N; r++) {
        printRow(graph, graph.vertexes[r - 1]);
    }
    printf("----------\n");
}

/* DEGREE CENTRALITY*/
int getEdgeCountOfVertex (Edge edges[MAX_EDGE_COUNT], Vertex vertex, int totalEdgeCount) {
    int edgeCount = 0;
    int i;
    for (i = 0; i < totalEdgeCount; i++) {
        if (strcmp(edges[i].from.name, vertex.name) == 0) edgeCount++;
    }
    return edgeCount;
}

void printDegreeCentralityRow (Edge edges[MAX_EDGE_COUNT], Vertex vertex, int totalEdgeCount, int totalVertexCount) {
    printToken(vertex.name);
    int edgeCount = getEdgeCountOfVertex(edges, vertex, totalEdgeCount);
    char edgeCountStr[MAX_STRING_LENGTH];
    char standardizedStr[MAX_STRING_LENGTH];
    sprintf(edgeCountStr, "%d", edgeCount);
    sprintf(standardizedStr, "%.5f", edgeCount * 1.0 / (totalVertexCount - 1));
    printToken(edgeCountStr);
    printToken(standardizedStr);
    printf("\n");
}

void printDegreeCentrality (Graph graph) {
    printf("DEGREE CENTRALITY\n");
    printToken("Node");
    printToken("Score");
    printToken("Std. Score");
    printf("\n");
    int i;
    for (i = 0; i < graph.vertexCount; i++) {
        printDegreeCentralityRow(graph.edges, graph.vertexes[i], graph.edgeCount, graph.vertexCount);
    }
    printf("----------\n");
}

/* CLOSENESS CENTRALITY */
int getTotalDistance (Graph graph, Vertex vertex) {
    int totalDistance = 0;
    int curDistance = 0;
    int hasChange = 1;
    while (hasChange) {
        hasChange = 0;
        int i;
        for (i = 0; i < graph.vertexCount; i++) {
            if (graph.vertexes[i].distance == curDistance) {
                int j;
                for (j = 0; j < graph.vertexCount; j++) {
                    if (graph.vertexes[j].distance == -1 && vertexHasEdge(graph.vertexes[i], graph.vertexes[j], graph.edges, graph.edgeCount)) {
                        graph.vertexes[j].distance = curDistance + 1;
                        totalDistance += curDistance + 1;
                        hasChange = 1;
                    }
                }
            }
        }
        curDistance++;
    }
    return totalDistance;
}

void printClosenessCentralityRow (Graph graph, Vertex vertex) {
    printToken(vertex.name);
    int totalDistance = getTotalDistance(graph, vertex);
    char scoreStr[MAX_STRING_LENGTH];
    char standardizedStr[MAX_STRING_LENGTH];
    sprintf(scoreStr, "%.5f", 1.0 / totalDistance);
    sprintf(standardizedStr, "%.5f", (graph.vertexCount - 1) / (totalDistance * 1.0));
    printToken(scoreStr);
    printToken(standardizedStr);
    printf("\n");
}

void printClosenessCentrality (Graph graph) {
    printf("CLOSENESS CENTRALITY\n");
    printToken("Node");
    printToken("Score");
    printToken("Std. Score");
    printf("\n");
    int i;
    for (i = 0; i < graph.vertexCount; i++) {
        int j;
        for (j = 0; j < graph.vertexCount; j++) {
            graph.vertexes[j].distance = -1; // means unvisited yet
        }
        graph.vertexes[i].distance = 0;
        printClosenessCentralityRow(graph, graph.vertexes[i]);
    }
    printf("----------\n");
}

/* BETWEENNESS CENTRALITY */
void printBetweennessCentralityRow (Graph * graph, Vertex source, Vertex target) {
    printToken(source.name);
    printToken(target.name);
    char path[MAX_VERTEX_COUNT][MAX_STRING_LENGTH];
    strcpy(path[0], source.name);
    int pathLength = target.distance + 1;
    int i, j;
    Vertex curVertex = target;
    for (i = pathLength - 1; i > 0; i--) {
        strcpy(path[i], curVertex.name);
        if (i != 0 && i != pathLength -1) {
            for (j = 0; j < (*graph).vertexCount; j++) {
                if (strcmp((*graph).vertexes[j].name, curVertex.name) == 0) {
                    (*graph).vertexes[j].betweennessScore += 1;
                }
            }
        }
        curVertex = findVertex((*graph), curVertex.fromName);
    }
    char intermediate[MAX_STRING_LENGTH] = "";
    char pathStr[MAX_STRING_LENGTH] = "";
    for (i = 1; i < pathLength - 1; i++) {
        if (i == 1) {
            sprintf(intermediate, "%s", path[i]);
        }
        else {
            sprintf(intermediate, "%s-%s", intermediate, path[i]);
        }
    }
    for (i = 0; i < pathLength; i++) {
        if (i == 0) {
            sprintf(pathStr, "%s", path[i]);
        }
        else {
            sprintf(pathStr, "%s-%s", pathStr, path[i]);
        }
    }
    printLongToken(intermediate);
    printLongToken(pathStr);
    printf("\n");
}

void calculateBetweennessPathFromVertex (Graph * graph, Vertex source) {
    int curDistance = 0;
    int hasChange = 1;
    while (hasChange) {
        hasChange = 0;
        int i;
        for (i = 0; i < (*graph).vertexCount; i++) {
            if ((*graph).vertexes[i].distance == curDistance) {
                int j;
                for (j = 0; j < (*graph).vertexCount; j++) {
                    if ((*graph).vertexes[j].distance == -1 && vertexHasEdge((*graph).vertexes[i], (*graph).vertexes[j], (*graph).edges, (*graph).edgeCount)) {
                        (*graph).vertexes[j].distance = curDistance + 1;
                        strcpy((*graph).vertexes[j].fromName, (*graph).vertexes[i].name);
                        hasChange = 1;
                    }
                }
            }
        }
        curDistance++;
    }
}

void printBetweennessCentralityScoreRow (Vertex vertex, int vertexCount) {
    printToken(vertex.name);
    char scoreStr[MAX_STRING_LENGTH];
    char stdScoreStr[MAX_STRING_LENGTH];
    float score = vertex.betweennessScore / ((vertexCount - 1) * (vertexCount - 2) * 0.5);
    sprintf(scoreStr, "%.5f", score);
    sprintf(stdScoreStr, "%.5f", score / ((vertexCount - 1) * (vertexCount - 2) * 0.5));
    printToken(scoreStr);
    printToken(stdScoreStr);
    printf("\n");
}

void printBetweennessCentralityScore (Graph graph) {
    printf("BETWEENNESS CENTRALITY SCORES\n");
    printToken("Node");
    printToken("Score");
    printToken("Std. Score");
    printf("\n");
    int i;
    for (i = 0; i < graph.vertexCount; i++) {
        printBetweennessCentralityScoreRow(graph.vertexes[i], graph.vertexCount);
    }
    printf("----------\n");
}

void prindBetweennessCentrality (Graph graph) {
    printf("BETWEENNESS CENTRALITY\n");
    printToken("Source");
    printToken("Target");
    printLongToken("Intermediade-Nodes");
    printLongToken("Path");
    printf("\n");
    int i, j;
    for (i = 0; i < graph.vertexCount; i++) {
        for (j = 0; j < graph.vertexCount; j++) {
            graph.vertexes[j].distance = -1; // means unvisited yet
            strcpy(graph.vertexes[j].fromName, "");
        }
        graph.vertexes[i].distance = 0;
        calculateBetweennessPathFromVertex(&graph, graph.vertexes[i]);
        for (j = i + 1; j < graph.vertexCount; j++) {
            printBetweennessCentralityRow(&graph, graph.vertexes[i], graph.vertexes[j]);
        }
    }
    printf("----------\n");

    printBetweennessCentralityScore(graph);
}

int main (void) {
    char lines[MAX_PEOPLE_COUNT][MAX_STRING_LENGTH];
    int lineCount = 0;
    readInputLines(lines, &lineCount);
    if (DEBUG == 1) {
        printf("\n");
        printf("line count %d\n", lineCount);
    }
    Graph graph = getGraph();
    createGraph(&graph, lines, lineCount);
    if (DEBUG == 1) {
        printGraph(graph);
    }
    printAdjacencyMatrix(graph);
    printDegreeCentrality(graph);
    printClosenessCentrality(graph);
    prindBetweennessCentrality(graph);
}
