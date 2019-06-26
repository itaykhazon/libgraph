#ifndef LIBGRAPH_LIBRARY_H
#define LIBGRAPH_LIBRARY_H

/******************************
 * Includes
 ******************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "queue.h"

#include "errors.h"

/**
 * @brief   A graph edge.
 */
struct graph_edge {
    /* The id of the first vertex. */
    uint64_t s_id;

    /* The id of the second vertex. */
    uint64_t d_id;

    /* The weight of the edge. */
    double weight;

    /* The next edge in the list, used only if this edge is a part of a neighbor list of a vertex. */
    LIST_ENTRY(graph_edge) next;
};

/**
 * @brief   A list of neighbors.
 */
BSD_LIST_HEAD(neighbor_list, graph_edge);

/**
 * @brief   A graph vertex.
 */
struct graph_vertex {
    /* The id of the vertex. */
    uint64_t id;

    /* The neighbors of the vertex. */
    size_t neighbor_count;
    struct neighbor_list neighbors;

    /* The next vertex in the list, used only if this is a part of a vertices list of a graph. */
    LIST_ENTRY(graph_vertex) next;
};

/**
 * @brief   A list of vertices.
 */
BSD_LIST_HEAD(vertex_list, graph_vertex);

/**
 * @brief   A struct of a graph G=(V,E)
 */
struct graph {
    /* Is the graph directional. */
    bool is_directional;

    /* The vertices of the graph. */
    size_t vertex_count;
    struct vertex_list vertices;
};

/**
 * @brief   Initialize an empty graph.
 * @param   is_directional  Is the graph directional.
 * @param   g           The graph generated (out parameter).
 * @return  GRAPH_ERR_SUCCESS on success.
 *
 * @note    The caller should call GRAPH_destroy to release the graph's memory.
 */
graph_res_t GRAPH_init(bool is_directional, struct graph **g);

/**
 * @brief   Destroy a graph and free its memory.
 * @param   g   The graph.
 * @return  GRAPH_ERR_SUCCESS on success.
 *
 * @note    g is a dangling pointer after the call and should be assigned to NULL.
 */
graph_res_t GRAPH_destroy(struct graph *g);

/**
 * @brief   Add a new vertex to the graph.
 * @param   g   The graph.
 * @param   id  The id of the new vertex.
 * @return  GRAPH_ERR_SUCCESS on success.
 */
graph_res_t GRAPH_add_vertex(struct graph *g, uint64_t id);

/**
 * @brief   Remove a given vertex from the graph and all its connected edges.
 * @param   g   The graph.
 * @param   id  The vertex to remove.
 * @return  GRAPH_ERR_SUCCECSS on success.
 */
graph_res_t GRAPH_remove_vertex(struct graph *g, uint64_t id);

/**
 * @brief   Adds an edge to the graph.
 * @param   g       The graph.
 * @param   s_id    The id of the source vertex (must exist!)
 * @param   d_id    The id of the destination vertex (must exist!)
 * @param   weight  The weight of the edge.
 * @return  GRAPH_ERR_SUCCESS on success.
 *
 * @note    Only one edge is allowed between any two vertices.
 */
graph_res_t GRAPH_add_edge(struct graph *g, uint64_t s_id, uint64_t d_id, double weight);

/**
 * @brief   Remove an edge from the graph.
 * @param   g       The graph
 * @param   s_id    id of the source vertex.
 * @param   d_id    id of the destination vertex.
 * @return  GRAPH_ERR_SUCCESS on success.
 */
graph_res_t GRAPH_remove_edge(struct graph *g, uint64_t s_id, uint64_t d_id);

/**
 * @brief   Returns an adjecency matrix of the graph.
 *          The order of the vertices is the order of the internal graph list.
 *          if e=(u,v) is an edge in the graph, adj[u][v] = w(e), otherwise, adj[u][v]=-1.
 * @param g             The graph.
 * @param adj_matrix    The returned adjencency matrix.
 * @param size          The size of the col/row of the matrix.
 * @return  GRAPH_ERR_SUCCESS on success.
 *
 * @note    GRAPH_free_adjecency_matrix should be called on the matrix to free it.
 */
graph_res_t GRAPH_get_adjecency_matrix(struct graph *g, double ***adj_matrix, size_t *size);

/**
 * @brief   Frees the memory of the adjecency matrix.
 * @param g             The graph.
 * @param adj_matrix    The adjecency matrix.
 * @return  GRAPH_ERR_SUCCESS on success.
 *
 * @note    adj_matrix is a dangling pointer after the call to this function.
 */
graph_res_t GRAPH_free_adjecency_matrix(struct graph *g, double **adj_matrix);

/**
 * @brief   Prints a representation of the graph.
 * @param   g       The graph
 * @return  GRAPH_ERR_SUCCESS on success.
 */
graph_res_t GRAPH_print(struct graph *g);

#endif