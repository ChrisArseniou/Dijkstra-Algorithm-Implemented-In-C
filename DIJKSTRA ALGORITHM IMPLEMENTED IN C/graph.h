#ifndef _GRAPH_H
#define _GRAPH_H

typedef struct private_graph* graph_t;
typedef struct private_path* path_t;

graph_t graph_new(int (*cmp)(void*, void*), void (*destroy_vertex)(void*));
void graph_destroy(graph_t graph);

int graph_add_vertex(graph_t graph, void *vertex);
int graph_rmv_vertex(graph_t graph, void *vertex);

int graph_add_edge(graph_t graph, void *vertex1, void *vertex2, int weight);
int graph_rmv_edge(graph_t graph, void *vertex1, void *vertex2); 

int path_len(path_t path);
void path_traverse_init(path_t path);
void *path_traverse_nxt(path_t path);
void path_destroy(path_t path);

path_t find_minhops_path(graph_t graph, void *vertex1, void *vertex2);
path_t find_shortest_path(graph_t graph, void *vertex1, void *vertex2);

#endif
