//
// Created by User on 25/06/2019.
//

#ifndef LIBGRAPH_ERRORS_H
#define LIBGRAPH_ERRORS_H

typedef enum graph_res_e {
    GRAPH_ERR_SUCCESS = 0,
    GRAPH_ERR_UNDEFINED = 1,
    GRAPH_ERR_PARAMS,
    GRAPH_ERR_MEM,
    GRAPH_ERR_FOUND,
    GRAPH_ERR_NOT_FOUND,

} graph_res_t;

#endif //LIBGRAPH_ERRORS_H
