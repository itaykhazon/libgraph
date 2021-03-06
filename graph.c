#include <malloc.h>
#include <stdio.h>
#include "graph.h"

/**
 * @brief   Checks if s is connected to d, if so, return the connecting edge in e.
 * @param   s   The source vertex.
 * @param   d   The destination vertex.
 * @param   e   The connecting edge (optional).
 * @return  true if there is an edge (s,d), false otherwise.
 */
bool graph_is_connected(struct graph_vertex *s, struct graph_vertex *d, struct graph_edge **e) {
    bool res = false;
    struct graph_edge *curr_edge = NULL;

    /* Parameter check. */
    if ((NULL == s) || (NULL == d)) {
        goto cleanup;
    }

    /* Check if d is in the neighbor list of s. */
    LIST_FOREACH(curr_edge, &s->neighbors, next) {
        if (d->id == curr_edge->d_id) {
            res = true;
            if (NULL != e) {
                *e = curr_edge;
            }
            goto cleanup;
        }
    }

    cleanup:
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_init(bool is_directional, struct graph **g) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph *local_graph = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Allocate memory for the g. */
    local_graph = malloc(sizeof(*local_graph));
    if (NULL == local_graph) {
        res = GRAPH_ERR_MEM;
        goto cleanup;
    }

    /* Initialize fields. */
    local_graph->is_directional = is_directional;
    local_graph->vertex_count = 0;
    LIST_INIT(&local_graph->vertices);

    /* Transfer ownership and indicate success. */
    *g = local_graph;
    local_graph = NULL;

    res = GRAPH_ERR_SUCCESS;

    cleanup:
    if (NULL != local_graph) {
        free(local_graph);
    }

    return res;
}

/** @see graph.h */
graph_res_t GRAPH_destroy(struct graph *g) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_vertex *v = NULL;

    /* Parameter check. */
    if (NULL == g) {
        return GRAPH_ERR_PARAMS;
    }

    /* Go over all the vertices and remove them. */
    while (!LIST_EMPTY(&g->vertices)) {
        v = LIST_FIRST(&g->vertices);
        GRAPH_remove_vertex(g, v->id);
        /* v is a dangling pointer here. */
    }

    /* Free the graph. */
    free(g);

    return GRAPH_ERR_SUCCESS;
}

/** @see graph.h */
graph_res_t GRAPH_add_vertex(struct graph *g, uint64_t id) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_vertex *v = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Check if the vertex already exists. */
    LIST_FOREACH(v, &g->vertices, next) {
        if (id == v->id) {
            v = NULL;
            res = GRAPH_ERR_FOUND;
            goto cleanup;
        }
    }

    /* Allocate memory for the vertex. */
    v = malloc(sizeof(*v));
    if (NULL == v) {
        res = GRAPH_ERR_MEM;
        goto cleanup;
    }

    /* Initialize fields. */
    v->id = id;
    v->neighbor_count = 0;
    LIST_INIT(&v->neighbors);

    /* Attach to graph. */
    LIST_INSERT_HEAD(&g->vertices, v, next);
    g->vertex_count++;

    /* Indicate success. */
    v = NULL;
    res = GRAPH_ERR_SUCCESS;

    cleanup:
    if (NULL != v) {
        free(v);
    }
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_remove_vertex(struct graph *g, uint64_t id) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_vertex *v = NULL;
    struct graph_edge *e = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Check if the vertex doesn't exist. */
    LIST_FOREACH(v, &g->vertices, next) {
        if (id == v->id) {
            break;
        }
    }
    if (NULL == v) {
        res = GRAPH_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* Go over all the edges connected to it and remove them. */
    while (!LIST_EMPTY(&v->neighbors)) {
        e = LIST_FIRST(&v->neighbors);
        GRAPH_remove_edge(g, e->s_id, e->d_id);
        /* e is a dangling pointer here. */
    }

    /* Detach the vertex from the graph and free it. */
    LIST_REMOVE(v, next);
    g->vertex_count--;
    free(v);

    /* Indicate success. */
    res = GRAPH_ERR_SUCCESS;

    cleanup:
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_add_edge(struct graph *g, uint64_t s_id, uint64_t d_id, double weight) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_edge *e = NULL;
    struct graph_edge *e2 = NULL;
    struct graph_vertex *v = NULL;
    struct graph_vertex *s = NULL;
    struct graph_vertex *d = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Find the vertices. */
    LIST_FOREACH(v, &g->vertices, next) {
        if (s_id == v->id) {
            s = v;
        }
        if (d_id == v->id) {
            d = v;
        }
    }
    if ((NULL == s) || (NULL == d)) {
        res = GRAPH_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* Check that the edge doesn't exist. */
    if (graph_is_connected(s, d, NULL)) {
        res = GRAPH_ERR_FOUND;
        goto cleanup;
    }

    /* Allocate memory for the edge. */
    e = malloc(sizeof(*e));
    if (NULL == e) {
        res = GRAPH_ERR_MEM;
        goto cleanup;
    }
    if ((!g->is_directional) && (s_id != d_id)) {
        /* Allocate memory for the edge on the other side. */
        e2 = malloc(sizeof(*e2));
        if (NULL == e2) {
            res = GRAPH_ERR_MEM;
            goto cleanup;
        }
    }

    /* Initialize fields. */
    e->s_id = s_id;
    e->d_id = d_id;
    e->weight = weight;

    /* Attach to vertices. if its undirectional, attach only to source. */
    LIST_INSERT_HEAD(&s->neighbors, e, next);
    s->neighbor_count++;
    if ((!g->is_directional) && (s_id != d_id)) {
        e2->s_id = d_id;
        e2->d_id = s_id;
        e2->weight = weight;

        LIST_INSERT_HEAD(&d->neighbors, e2, next);
        d->neighbor_count++;
    }

    /* Indicate success. */
    e = NULL;
    e2 = NULL;
    res = GRAPH_ERR_SUCCESS;

    cleanup:
    if (NULL != e) {
        free(e);
    }
    if (NULL != e2) {
        free(e2);
    }
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_remove_edge(struct graph *g, uint64_t s_id, uint64_t d_id) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_edge *e = NULL;
    struct graph_vertex *v = NULL;
    struct graph_vertex *s = NULL;
    struct graph_vertex *d = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Find the vertices. */
    LIST_FOREACH(v, &g->vertices, next) {
        if (s_id == v->id) {
            s = v;
        }
        if (d_id == v->id) {
            d = v;
        }
    }
    if ((NULL == s) || (NULL == d)) {
        res = GRAPH_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* Check that the edge exists and get it. */
    if (!graph_is_connected(s, d, &e)) {
        res = GRAPH_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* remove from s. if its undirectional, remove also from d. */
    LIST_REMOVE(e, next);
    s->neighbor_count--;
    free(e);

    if ((!g->is_directional) && (s_id != d_id)) {
        /* We can ignore return value, it will always succeed. */
        (void)graph_is_connected(d, s, &e);
        LIST_REMOVE(e, next);
        d->neighbor_count--;
        free(e);
    }

    /* Indicate success. */
    res = GRAPH_ERR_SUCCESS;

    cleanup:
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_get_adjecency_matrix(struct graph *g, double ***adj_matrix, size_t *size) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_vertex  *v = NULL;
    struct graph_vertex  *v2 = NULL;
    struct graph_edge *e = NULL;
    double **local_matrix = NULL;
    double *matrix_data = NULL;
    size_t i = 0;
    size_t j = 0;

    /* Parameter check. */
    if ((NULL == g) || (NULL == adj_matrix) || (NULL == size)) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Allocate the rows array. */
    local_matrix = malloc(sizeof(*local_matrix) * g->vertex_count);
    if (NULL == local_matrix) {
        res = GRAPH_ERR_MEM;
        goto cleanup;
    }

    /* Allocate a buffer for the matrix itself. */
    matrix_data = malloc(sizeof(*matrix_data) * g->vertex_count * g->vertex_count);
    if (NULL == matrix_data) {
        res = GRAPH_ERR_MEM;
        goto cleanup;
    }

    /* Fix the rows in the rows array. */
    for (i = 0; i < g->vertex_count; ++i) {
        local_matrix[i] = (matrix_data + (i * g->vertex_count));
    }

    for (i = 0; i < g->vertex_count; ++i) {
        for (j = 0; j < g->vertex_count; ++j) {
            local_matrix[i][j] = -1;
        }
    }

    /* Go over all the vertices and fill the matrix. */
    i = 0;
    j = 0;
    LIST_FOREACH(v, &g->vertices, next) {
        LIST_FOREACH(e, &v->neighbors, next) {
            /* Find the index of d_id */
            LIST_FOREACH(v2, &g->vertices, next) {
                if (e->d_id == v2->id) {
                    matrix_data[i * g->vertex_count + j] = e->weight;
                }
                j++;
            }
            j = 0;
            i++;
        }
    }

    /* Transfer ownership and indicate success. */
    *adj_matrix = local_matrix;
    *size = g->vertex_count;
    local_matrix = NULL;
    matrix_data = NULL;

    res = GRAPH_ERR_SUCCESS;

    cleanup:
    if (NULL != local_matrix) {
        free(local_matrix);
    }
    if (NULL != matrix_data) {
        free(matrix_data);
    }

    return res;
}

/** @see graph.h */
graph_res_t GRAPH_free_adjecency_matrix(struct graph *g, double **adj_matrix) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    /* Parameter check. */
    if ((NULL == g) || (NULL == adj_matrix)) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    if (NULL != adj_matrix[0]) {
        free(adj_matrix[0]);
    }
    free(adj_matrix);

    /* Indicate success. */
    res = GRAPH_ERR_SUCCESS;

    cleanup:
    return res;
}

/** @see graph.h */
graph_res_t GRAPH_print(struct graph *g) {
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    struct graph_vertex  *v = NULL;
    struct graph_edge *e = NULL;

    /* Parameter check. */
    if (NULL == g) {
        res = GRAPH_ERR_PARAMS;
        goto cleanup;
    }

    /* Go over all the vertices and print their edges. */
    LIST_FOREACH(v, &g->vertices, next) {
        (void)printf("%llu: ", v->id);
        LIST_FOREACH(e, &v->neighbors, next) {
            (void)printf("(%llu->%llu (%f)) ", e->s_id, e->d_id, e->weight);
        }
        (void)printf("\n");
    }

    cleanup:
    return res;
}
