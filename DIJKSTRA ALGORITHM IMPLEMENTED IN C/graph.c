#include "stdio.h"
#include "graph.h"
#include "stdlib.h"
#include "limits.h"

/*struct poy xrhsimopoieitai gia ta kelia toy pinaka*/
typedef struct vertex_chell vertex_c;

struct vertex_chell{
	void* name;
	vertex_c *root;
	int pos;
	
	vertex_c *start;
	vertex_c *this_v;
	vertex_c *next;
	int weight;
};

/*struct poy perigrafei ton grapho*/
struct private_graph{
	int size,number_of_people,cond;
	vertex_c *graph_g;
	int (*cmp)(void*, void*);
	void (*destroy)(void*);
};

/*struct poy perigrafei to path*/
struct private_path{
	int number_of_edges,curr_pos,size,max_weight,len;
	vertex_c *root;
	void* curr_v;
};

/*synartisi dimiourgias toy graphoy h opoia arxikopoiei kai tis 2 vasikes synartiseis poy xrisimopeioyme sto programma*/
graph_t graph_new(int (*compare)(void*, void*), void (*destroy_vertex)(void*)){
	graph_t graph;
	
	graph = (struct private_graph*) malloc(sizeof(struct private_graph));
	graph->size = 5;
	graph->graph_g = (vertex_c*)malloc(graph->size*sizeof(vertex_c));
	graph->number_of_people = 0;
	graph->cmp = compare;
	graph->destroy = destroy_vertex;
	return(graph);
}

/*diagrafh grafoy kanontas free olous tous komvous*/
void graph_destroy(graph_t graph){
	int pos;
	vertex_c *curr;
	
	for(pos=0;pos<graph->number_of_people;pos++){
		graph->destroy(graph->graph_g[pos].name);
		curr = graph->graph_g[pos].root;
		if(graph->graph_g[pos].root != NULL){
			 while(curr->next != NULL){
				 curr = curr->next;
			}
			free(curr);
		}
	}
	free(graph->graph_g);
	free(graph);
}

/*prosthesei komvou, se ena neo keli toy pinaka, efoson den yparxrei idi*/
int graph_add_vertex(graph_t graph, void *vertex){
	int pos;
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex) == 1){
			return(-1);
		}
	}
	
	if(graph->number_of_people < graph->size){
		graph->graph_g[graph->number_of_people].name = vertex;
		graph->graph_g[graph->number_of_people].root = NULL;
		graph->graph_g[graph->number_of_people].pos = graph->number_of_people;
		graph->number_of_people++;
		return(0);
	}
	else {
		graph->size = graph->size*2;
		graph->graph_g = (vertex_c *) realloc(graph->graph_g, (graph->size)*sizeof(vertex_c));
		graph->graph_g[graph->number_of_people].name = vertex;
		graph->graph_g[graph->number_of_people].root = NULL;
		graph->graph_g[graph->number_of_people].pos = graph->number_of_people;
		graph->number_of_people++;
		return(0);
	}
	return(-1);
}

/*afairesh komvou , me diagrafei olwn twn connections me tous allous komvous*/
int graph_rmv_vertex(graph_t graph, void *vertex){
	int pos,name_to_be_remove_pos;
	
	for(name_to_be_remove_pos=0;name_to_be_remove_pos<graph->number_of_people;name_to_be_remove_pos++){
		if(graph->cmp(graph->graph_g[name_to_be_remove_pos].name , vertex) == 1){
			break;
		}
	}
	if(name_to_be_remove_pos == graph->number_of_people){
		return(-1);
	}

	if(name_to_be_remove_pos >= 0 && graph->number_of_people > 0){
		for(pos=0;pos<graph->number_of_people;pos++){
			if((graph->cmp(graph->graph_g[pos].name, vertex) != 1) && (graph->graph_g[pos].root != NULL)){
				graph_rmv_edge(graph, vertex, graph->graph_g[pos].name);
			}
		}
		graph->number_of_people--;
		graph->destroy(graph->graph_g[name_to_be_remove_pos].name);
		graph->graph_g[name_to_be_remove_pos].name = graph->graph_g[graph->number_of_people].name;
		graph->graph_g[name_to_be_remove_pos].root = graph->graph_g[graph->number_of_people].root;
		if(graph->number_of_people >= 1){
			graph->size = graph->number_of_people;
			graph->graph_g = (vertex_c *)realloc(graph->graph_g, (graph->size)*sizeof(vertex_c));
		}
	}
	else {
		return(-1);
	}	
	return(0);
}

/*prosthesh enos connection anamesa se dyo komvous*/
int graph_add_edge(graph_t graph, void *vertex1, void *vertex2, int weight){
	int pos_vertex1=-1, pos_vertex2=-1,pos;
	vertex_c *edge1, *edge2,*curr;
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex1) == 1){
			pos_vertex1 = pos;
			break;
		}
	}
	
	if(pos_vertex1 == -1){
		return(-1);
	}
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex2) == 1){
			pos_vertex2 = pos;
			break;
		}
	}
	
	if(pos_vertex2 == -1){
		return(-1);
	}
	
	//prosthetoyme thn akmh ston prwto komvo
	curr = graph->graph_g[pos_vertex1].root;
	while(curr != NULL){
		if(graph->cmp(curr->this_v->name, vertex2) == 1){
			return(-2);
		}
		curr = curr->next;
	}
	
	edge1 = (vertex_c *)malloc(sizeof(vertex_c));
	edge1->start = &graph->graph_g[pos_vertex1];
	edge1->pos = graph->graph_g[pos_vertex1].pos;
	edge1->this_v = &graph->graph_g[pos_vertex2];
	edge1->weight = weight;
	edge1->next = graph->graph_g[pos_vertex1].root;
	graph->graph_g[pos_vertex1].root = edge1;

	//prosthetoyme thn akmh ston deutero komvo
	curr = graph->graph_g[pos_vertex2].root;
	while(curr!=NULL){
		if(graph->cmp(curr->this_v->name, vertex1) == 1){
			return(-2);
		}
		curr=curr->next;
	}
	edge2 = (vertex_c *)malloc(sizeof(vertex_c));
	edge2->start = &graph->graph_g[pos_vertex2];
	edge2->pos = graph->graph_g[pos_vertex2].pos;
	edge2->this_v = &graph->graph_g[pos_vertex1];
	edge2->weight = weight;
	edge2->next = graph->graph_g[pos_vertex2].root;
	graph->graph_g[pos_vertex2].root = edge2;
	return(0);
}

/*afairesh akmhs metaxy dyo komvwn , efoson yparxrei*/
int graph_rmv_edge(graph_t graph, void *vertex1, void *vertex2){
	int pos_vertex1=-1, pos_vertex2=-1,pos;
	vertex_c *curr,*prev;

	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex1) == 1){
			pos_vertex1 = pos;
			break;
		}
	}
	
	if(pos_vertex1 == -1){
		return(-1);
	}
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex2) == 1){
			pos_vertex2 = pos;
			break;
		}
	}
	
	if(pos_vertex2 == -1){
		return(-1);
	}
	
	curr = graph->graph_g[pos_vertex1].root;
	while(curr!=NULL){
		if(graph->cmp(curr->this_v->name, vertex2) == 1){
			break;
		}
		curr=curr->next;
	}
	
	if(curr == NULL){
		return(-2);
	}
	
	curr = graph->graph_g[pos_vertex2].root;
	while(curr!=NULL){
		if(graph->cmp(curr->this_v->name, vertex1) == 1){
			break;
		}
		curr=curr->next;
	}
	
	if(curr == NULL){
		return(-2);
	}
	/*afairesh filias apo ton proto komvo*/
	for(prev = NULL, curr= graph->graph_g[pos_vertex1].root;((graph->cmp(curr->this_v->name, vertex2) != 1) && (curr!=NULL));prev=curr,curr=curr->next);
	if(curr != NULL){
		if(prev==NULL){
			
			graph->graph_g[pos_vertex1].root = curr->next;
		}
		else {
			prev->next = curr->next;
		}
		free(curr);
	}
	
	/*afairesh filias apo ton deutero komvo*/
	for(prev = NULL, curr= graph->graph_g[pos_vertex2].root;((graph->cmp(curr->this_v->name, vertex1) != 1) && (curr!=NULL));prev=curr,curr=curr->next);
	if(curr != NULL){
		if(prev==NULL){
			graph->graph_g[pos_vertex2].root = curr->next;
		}
		else {
			prev->next = curr->next;
		}
		free(curr);
	}
	return(0);
}

void print(graph_t graph){
	int pos;
	vertex_c *curr;
	
	fprintf(stderr, "graph G {\n");
	for(pos=0;pos<graph->number_of_people;pos++){
		if((graph->graph_g[pos].name) != NULL){
			fprintf(stderr, "%s\n",(char*)graph->graph_g[pos].name);
		}
	}
	
	for(pos=0;pos<graph->number_of_people;pos++){
		curr = graph->graph_g[pos].root;
		while(curr != NULL){
			if(curr->this_v->pos > pos){
				fprintf(stderr, "%s -- %s [label=%d]\n", (char*)graph->graph_g[pos].name, (char*)curr->this_v->name, curr->weight);
			}
			curr=curr->next;
		}
	}
	fprintf(stderr, "}\n");
}

/*epistrefei to length , to opoio thetoume analogos se kathe periptosh*/
int path_len(path_t path_a){
	return(path_a->len);
}

/*thetei ton pointer sto teleutaio stoixeio tou pinaka poy periexei to path*/
void path_traverse_init(path_t path){
	path->curr_v = path->root[path->number_of_edges-1].this_v->name;
	path->curr_pos = path->number_of_edges-1;
}

/*epistrofh toy epomenoy stoixeioy apo ton pinaka poy periexei to path*/
void *path_traverse_nxt(path_t path){
	
	if(path->curr_pos == -1){
		return(NULL); 
	}
	else {
		path->curr_v = path->root[path->curr_pos].this_v->name;
		path->curr_pos--;
	}
	return(path->curr_v);
	
}

/*apeleytherwsei mnhmhs gia ton pinaka poy exei to path*/
void path_destroy(path_t path){
	free(path->root);
	free(path);
}

//pred == predecessor
path_t find_shortest_path(graph_t graph, void *vertex1, void *vertex2){
	path_t path=NULL;
	int pos_vertex1=-1, pos_vertex2=-1,pos,weight[graph->number_of_people],pred[graph->number_of_people],visited[graph->number_of_people],count,min_w,next_node,i,j, start_node;
	vertex_c *vertex,*curr;
	
	/*arxika ginetai elegxos gia ton an yparxoyn oi komvoi*/
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex1) == 1){
			pos_vertex1 = pos;
			break;
		}
	}
	
	if(pos_vertex1 == -1){
		return(NULL);
	}
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex2) == 1){
			pos_vertex2 = pos;
			break;
		}
	}
	
	if(pos_vertex2 == -1){
		return(NULL);
	}
	/*arxikopoihsh metavlitwn poy tha xrisimopoieisoume ston dijkstra*/
	start_node = pos_vertex1;
	for(i=0;i<graph->number_of_people;i++){
		weight[i]=INT_MAX;
		pred[i]=start_node;
		visited[i]=0;
	}
	
	weight[start_node] = 0;
	count = 0;
	while(count<graph->number_of_people-1){
		min_w = INT_MAX;
		for(i=0;i<graph->number_of_people;i++){
			if(weight[i] < min_w && visited[i] != 1){
				min_w = weight[i];
				next_node = i;
			}
		}
		visited[next_node] = 1;
		for(vertex = graph->graph_g[next_node].root;vertex!=NULL;vertex=vertex->next){
			if(!visited[vertex->this_v->pos]){
				if(min_w + (vertex->weight) < weight[vertex->this_v->pos]){
					weight[vertex->this_v->pos] = min_w + (vertex->weight);
					pred[vertex->this_v->pos] = next_node;
				}
			}
		}
			
		count++;
	}
	path =(struct private_path*) malloc(sizeof(struct private_path));
	path->size =  graph->number_of_people;
	path->root = (vertex_c*) malloc(graph->number_of_people*sizeof(vertex_c));
	for(i=0;i<graph->number_of_people;i++){
		path->root[i].this_v = NULL;
	}
	pos = 1;
	j = pos_vertex2;
	path->number_of_edges = 0;
	path->max_weight = 0;
	path->max_weight = weight[j];
	do{
		j = pred[j];
		path->root[pos].this_v = &graph->graph_g[j];
		pos++;
	}while(j!=start_node);
	
	path->root[0].this_v = &graph->graph_g[pos_vertex2];
	//to number_of_edges ousiastika einai o arithos twn stoixeiwn tou
	path->number_of_edges = 0;
	for(i=0;i<graph->number_of_people;i++){
		if(path->root[i].this_v != NULL){
			path->number_of_edges++;
		}
	}
	
	//elegxos gia to an oi komvoi exoun kapia syndesi
	curr = graph->graph_g[pos_vertex1].root;
	while(curr!=NULL){
		if(graph->cmp(curr->this_v->name, vertex2) == 1){
			break;
		}
		curr=curr->next;
	}
	
	path->len = path->max_weight;
	if((graph->cmp(path->root[0].this_v->name, vertex2) == 1) && (graph->cmp(path->root[1].this_v->name, vertex1) == 1) && curr==NULL){
	//	exoyme ws proypothesi to curr na eina NULL prokeimenou na gnwizoyme oti oi komvoi den exoyn kamia syndesi kai epomenos den yparxei path
		return(NULL);
	}
	
	return(path);
}

path_t find_minhops_path(graph_t graph, void *vertex1, void *vertex2){
	path_t path=NULL;
	int pos_vertex1=-1, pos_vertex2=-1,pos,weight[graph->number_of_people],pred[graph->number_of_people], visited[graph->number_of_people],count,min_w,next_node,i,j, start_node;
	vertex_c *vertex,*curr;

	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex1) == 1){
			pos_vertex1 = pos;
			break;
		}
	}
	
	if(pos_vertex1 == -1){
		return(NULL);
	}
	
	for(pos=0;pos<graph->number_of_people;pos++){
		if(graph->cmp(graph->graph_g[pos].name , vertex2) == 1){
			pos_vertex2 = pos;
			break;
		}
	}
	
	if(pos_vertex2 == -1){
		return(NULL);
	}
	
	//elegxos gia to an oi komvoi exoyn syndesi tote h elaxisth akmh tha einai 1
	curr = graph->graph_g[pos_vertex1].root;
	while(curr!=NULL){
		if(graph->cmp(curr->this_v->name, vertex2) == 1){
			break;
		}
		curr=curr->next;
	}
	
	if(curr != NULL){
		path =(struct private_path*) malloc(sizeof(struct private_path));
		path->size =  2;
		path->number_of_edges = 2;
		path->len = 1;
		path->root = (vertex_c*) malloc(graph->number_of_people*sizeof(vertex_c));
		for(i=0;i<2;i++){
			path->root[i].this_v = NULL;
		}
		path->root[1].this_v = &graph->graph_g[pos_vertex1];
		path->root[0].this_v = &graph->graph_g[pos_vertex2];
		return(path);
	}
	
	start_node = pos_vertex1;
	for(i=0;i<graph->number_of_people;i++){
		weight[i]=INT_MAX;
		pred[i]=start_node;
		visited[i]=0;
	}
	
	weight[start_node] = 0;
	count = 0;
	while(count<graph->number_of_people-1){
		min_w = INT_MAX;
		for(i=0;i<graph->number_of_people;i++){
			if(weight[i] < min_w && visited[i] != 1){
				min_w = weight[i];
				next_node = i;
			}
		}
		visited[next_node] = 1;
		for(vertex = graph->graph_g[next_node].root;vertex!=NULL;vertex=vertex->next){
			if(!visited[vertex->this_v->pos]){
				if(min_w + 1 < weight[vertex->this_v->pos]){
					weight[vertex->this_v->pos] = min_w + 1;
					pred[vertex->this_v->pos] = next_node;
				}
			}
		}
		count++;
	}
	
	path =(struct private_path*) malloc(sizeof(struct private_path));
	path->size =  graph->number_of_people;
	path->root = (vertex_c*) malloc(graph->number_of_people*sizeof(vertex_c));
	for(i=0;i<graph->number_of_people;i++){
		path->root[i].this_v = NULL;
	}
	
	pos = 1;
	j = pos_vertex2;
	do{
		j = pred[j];
		path->root[pos].this_v = &graph->graph_g[j];
		pos++;
	}while(j!=start_node);
	
	path->root[0].this_v = &graph->graph_g[pos_vertex2];
	path->number_of_edges = 0;
	for(i=0;i<graph->number_of_people;i++){
		if(path->root[i].this_v != NULL){
			path->number_of_edges++;
		}
	}
	//gia na vgoun oi akmes  akrivos kai oxi iso me ta stoixeia toy pinaka
	path->len = path->number_of_edges - 1;
	if((graph->cmp(path->root[0].this_v->name, vertex2) == 1) && (graph->cmp(path->root[1].this_v->name, vertex1) == 1)){
		return(NULL);
	}
	
	return(path);
}

