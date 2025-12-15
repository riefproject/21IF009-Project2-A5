#ifndef GRAPH_H
#define GRAPH_H
#include "reestruct_export.h"

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct GraphVertex GraphVertex;
typedef struct GraphEdge GraphEdge;

/** Edge in adjacency list. */
struct GraphEdge {
    GraphVertex* to; /**< Destination vertex. */
    GraphEdge* next; /**< Next edge. */
};

/** Vertex with adjacency list. */
struct GraphVertex {
    void* data;          /**< User payload. */
    GraphEdge* edges;    /**< Outgoing edges. */
    GraphVertex* next;   /**< Next vertex in graph list. */
};

/** Simple adjacency-list graph. */
typedef struct Graph {
    GraphVertex* vertices; /**< Vertex list head. */
    size_t vertexCount;    /**< Number of vertices. */
    bool directed;         /**< Directed if true. */
} Graph;

/** Create empty graph (directed or undirected).
 *  @param[in] directed True for directed graph, false for undirected.
 *  @return Graph pointer or NULL on allocation failure.
 */
RSTAPI Graph* createGraph(bool directed);
/** Add vertex with user data.
 *  @param[in,out] graph Graph pointer.
 *  @param[in] data Payload pointer.
 *  @return Vertex pointer or NULL on allocation failure.
 */
RSTAPI GraphVertex* graphAddVertex(Graph* graph, void* data);
/** Add edge from->to (and reverse if undirected).
 *  @param[in,out] graph Graph pointer.
 *  @param[in,out] from Source vertex.
 *  @param[in,out] to Destination vertex.
 */
RSTAPI void graphAddEdge(Graph* graph, GraphVertex* from, GraphVertex* to);
/** Remove edge; returns true if removed.
 *  @param[in,out] graph Graph pointer.
 *  @param[in,out] from Source vertex.
 *  @param[in,out] to Destination vertex.
 *  @return True if an edge was removed.
 */
RSTAPI bool graphRemoveEdge(Graph* graph, GraphVertex* from, GraphVertex* to);
/** Vertex count.
 *  @param[in] graph Graph pointer.
 */
RSTAPI size_t graphSize(Graph* graph);
/** Return directed flag.
 *  @param[in] graph Graph pointer.
 */
RSTAPI bool graphIsDirected(Graph* graph);
/** Iterate over vertices.
 *  @param[in] graph Graph pointer.
 *  @param[in] visit Callback invoked for each vertex.
 */
RSTAPI void graphForEach(Graph* graph, void (*visit)(GraphVertex* vertex));
/** Remove all vertices and edges.
 *  @param[in,out] graph Graph pointer.
 *  @note Does not free vertex payloads.
 */
RSTAPI void clearGraph(Graph* graph);
/** Free graph (does not free vertex payloads).
 *  @param[in,out] graph Graph pointer.
 */
RSTAPI void freeGraph(Graph* graph);

#endif
