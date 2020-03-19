//
// Created by User on 25/06/2019.
//
#include "tests.h"
#include "graph.h"

bool test_graph_init_happy_flow() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, &g);

    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_TRUE(g != NULL);
    ASSERT_EQUAL(g->vertex_count, 0);
    ASSERT_EQUAL(g->is_directional, false);
    ASSERT_TRUE(LIST_EMPTY(&g->vertices));

    res = GRAPH_destroy(g);

    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_init_bad_params() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, NULL);

    ASSERT_EQUAL(res, GRAPH_ERR_PARAMS);
    ASSERT_EQUAL(g, NULL);

    return true;
}

bool test_graph_add_vertex_happy_flow() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(g->vertex_count, 1);
    ASSERT_TRUE(LIST_FIRST(&g->vertices) != NULL);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->id, 2);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 0);

    res = GRAPH_remove_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(g->vertex_count, 0);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices), NULL);

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_add_vertex_already_exists() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(g->vertex_count, 1);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_FOUND);
    ASSERT_EQUAL(g->vertex_count, 1);

    res = GRAPH_remove_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(g->vertex_count, 0);

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_add_vertex_multiple() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    uint64_t i = 0;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    for (i = 1; i < 100; i++) {
        res = GRAPH_add_vertex(g, i);
        ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
        ASSERT_EQUAL(g->vertex_count, i);
    }

    for (i = 99; i > 0; i--) {
        res = GRAPH_remove_vertex(g, i);
        ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
        ASSERT_EQUAL(g->vertex_count, i - 1);
    }

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_get_adj_matrix() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;
    double **adj = NULL;
    size_t size = 0;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 1);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_edge(g, 1, 2, 0.5);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_get_adjecency_matrix(g, &adj, &size);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(size, 2);
    ASSERT_EQUAL(adj[0][0], -1);
    ASSERT_EQUAL(adj[0][1], 0.5);
    ASSERT_EQUAL(adj[1][0], 0.5);
    ASSERT_EQUAL(adj[1][1], -1);

    res = GRAPH_free_adjecency_matrix(g, adj);
    ASSERT_EQUAL(res , GRAPH_ERR_SUCCESS);

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_add_edge_multiple() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 1);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_edge(g, 1, 2, 0.5);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 1);

    res = GRAPH_add_edge(g, 1, 2, 0.5);
    ASSERT_EQUAL(res, GRAPH_ERR_FOUND);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 1);

    res = GRAPH_add_edge(g, 2, 1, 0.5);
    ASSERT_EQUAL(res, GRAPH_ERR_FOUND);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 1);

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

bool test_graph_add_edge_happy_flow() {
    struct graph *g = NULL;
    graph_res_t res = GRAPH_ERR_UNDEFINED;

    res = GRAPH_init(false, &g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 1);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_vertex(g, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    res = GRAPH_add_edge(g, 1, 2, 0.5);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 1);
    ASSERT_TRUE(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors) != NULL);
    if (LIST_FIRST(&g->vertices)->id == 1) {
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->s_id, 1);
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->d_id, 2);
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->weight, 0.5);
    } else {
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->s_id, 2);
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->d_id, 1);
        ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors)->weight, 0.5);
    }

    res = GRAPH_remove_edge(g, 1, 2);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);
    ASSERT_EQUAL(LIST_FIRST(&g->vertices)->neighbor_count, 0);
    ASSERT_EQUAL(LIST_FIRST(&LIST_FIRST(&g->vertices)->neighbors), NULL);

    res = GRAPH_destroy(g);
    ASSERT_EQUAL(res, GRAPH_ERR_SUCCESS);

    return true;
}

int main() {
    SUITE_INIT(Sanity)
        ASSERT_TEST(test_graph_init_happy_flow);
        ASSERT_TEST(test_graph_init_bad_params);

        ASSERT_TEST(test_graph_add_vertex_happy_flow);
        ASSERT_TEST(test_graph_add_vertex_already_exists);
        ASSERT_TEST(test_graph_add_vertex_multiple);

        ASSERT_TEST(test_graph_add_edge_happy_flow);
        ASSERT_TEST(test_graph_add_edge_multiple);

        ASSERT_TEST(test_graph_get_adj_matrix);
    SUITE_END(Sanity)
}

