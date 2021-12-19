#include "stdio.h"
#include "string.h"
#include "graph.h"
#include "stdlib.h"

extern void print(graph_t graph);

/*synartisi sygkishs poy thn pername ws parametro sthn graph*/
int compare(void* v1, void* v2){
	if(strcmp((char*)(v1), (char*)(v2)) == 0){
		return(1);
	}
	else {
		return(0);
	}
	return(0);
}

/*synartisi free poy thn pername ws parametro sthn graph*/
void vertex_destroy(void* v1){
	free((char*)v1);
}

/*dimiourgei ena grafo*/
graph_t create(graph_t graph, int *cond){
	if((*cond) == 0){
		graph = graph_new(compare,vertex_destroy);
		(*cond) = 1;
		printf("\nC-OK\n");
	}
	else {
		printf("\nC-NOK\n");
	}
	return(graph);
}

/*prosthesi enos komvou*/
void add(graph_t graph, char *name){
	int result;
	
	result = graph_add_vertex(graph,name);
	
	if(result == 0){
		printf("\nAP-OK\n");
	}
	else {
		printf("\nAP-NOK\n");
		free(name);
	}
}

/*afairesh enos komvou*/
void remove_v(graph_t graph){
	int result;
	char name[64];
	
	scanf(" %63s", name);
	result = graph_rmv_vertex(graph, name);
	
	if(result == 0){
		printf("\nRP-OK\n");
	}
	else {
		printf("\nRP-NOK\n");
	}
}

/*prosthsh filias metaxh dyo atomwn*/
void add_fr(graph_t graph){
	int result,weight;
	char name1[64],name2[64];
	
	scanf(" %63s %63s %d", name1, name2, &weight);
	result = graph_add_edge(graph, name1, name2, weight);

	if(result == 0){
		printf("\nAF-OK\n");
	}
	else if(result == -1){
		printf("\nAF-NOK P\n");
	}
	else if(result == -2){
		printf("\nAF-NOK F\n");
	}
}

/*afairesh filias metaxh dyo atomwn efoson yparxei*/
void rmv_fr(graph_t graph){
	int result;
	char name1[64], name2[64];
	
	scanf("%63s %63s", name1, name2);
	result = graph_rmv_edge(graph, name1, name2);
	
	if(result == 0){
		printf("\nRF-OK\n");
	}
	else if(result == -1){
		printf("\nRF-NOK P\n");
	}
	else if(result == -2){
		printf("\nRF-NOK F\n");
	}
}

/*evresh monopatioy anamesa se dyo atoma me to mikrotero weight*/
void likely_connection(graph_t graph, path_t path){
	void* name; 
	int length;
	char name1[64],name2[64];
	
	scanf(" %s %s", name1, name2);
	path = find_shortest_path(graph, name1, name2);
	if(path == NULL){
		printf("\nFL-NOK\n");
	}
	else {
		length = path_len(path);
		path_traverse_init(path);
		printf("\n##\n");
		printf("FL-OK %d", length);
		while(1){
			name = path_traverse_nxt(path);
			if(name == NULL){
				break;
			}
			printf(" %s", (char*)name);
		}
		printf("\n");
	}
}

/*evresh monopatioy anamesa se dyo atoma me tis ligoteres akmes*/
void min_hops(graph_t graph, path_t path){
	void* name; 
	int length;
	char name1[64], name2[64];
	
	scanf(" %s %s", name1, name2);

	path = find_minhops_path(graph, name1, name2);
	if(path == NULL){
		printf("\nFC-NOK\n");
	}
	else {
		length = path_len(path);
		path_traverse_init(path);
		printf("\n##\n");
		printf("FC-OK %d", length);
		while(1){
			name = path_traverse_nxt(path);
			if(name == NULL){
				break;
			}
			printf(" %s", (char*)name);
		}
		printf("\n");
	}
}

void print_g(graph_t graph){
	print(graph);
}

int main (int argc, char *argv[]){
	char option[3];
	graph_t graph=NULL;
	char *name;
	path_t path=NULL;
	int graph_created=0;
	
	while(1){
		scanf(" %2s", option);
		if(strcmp(option, "c") == 0){
			graph = create(graph,&graph_created);
		}
		else if(strcmp(option, "d") == 0){
			graph_destroy(graph);
			graph_created = 0;
			printf("\nD-OK\n");
		}
		else if(strcmp(option, "ap") == 0){
			name = malloc(64*sizeof(char));
			scanf(" %s", name);
			add(graph, name);
		}
		else if(strcmp(option, "rp") == 0){
			remove_v(graph);
		}
		else if(strcmp(option, "af") == 0){
			add_fr(graph);
		}
		else if(strcmp(option, "rf") == 0){
			rmv_fr(graph);
		}
		else if(strcmp(option, "fc") == 0){
			min_hops(graph,path);
			if(path!=NULL){
				path_destroy(path);
			}
		}
		else if(strcmp(option, "fl") == 0){
			likely_connection(graph,path);
			if(path != NULL){
				path_destroy(path);
			}
		}
		else if(strcmp(option, "p") == 0){
			print_g(graph);
		}
		else if(strcmp(option, "q") == 0){
			return(0);
		}
		else{
			return(0);
		}
	}
	return(0);
	
}
