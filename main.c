#include "circuit.h"
 
int main(int argc, char *argv[])
{

  	FILE	*fp_bench;
  	NODE	graph[MAX_NODES];

	CELL	noc[MAX_NODES];
	NET		con[MAX_NODES];


	DLIST	*bucket_A=NULL, *bucket_B=NULL;	
  	LIST	*in=NULL, *out=NULL;
	int	max_node_id=0, i, cut=0;	
	int maxgainA=0, maxgainB=0;
	int base_cell=NULL;
	int converge, pass;
  clock_t start, end;
  double cpu_time_used;
	if(argc!=2)
  	{
     	printf("Insufficient arguments\n");
     	exit(0);
  	}
  
   fp_bench=fopen(argv[1],"r");
   max_node_id = read_circuit(fp_bench, graph, &in, &out);
  // print_graph(graph);
   printf("MaxNodeID=%d\n", max_node_id);
	int coun=0,Pm;
  start = clock(); 
	coun=  Tgat(graph,max_node_id);
	printf("Totalgate= %d\n",coun);
	adj_func( graph,noc,con,max_node_id,coun);
	gain_fnc(noc,con,graph,max_node_id);
	//print_noc(graph,noc,max_node_id);
	//print_con(graph,con,max_node_id);
	Pm=Pmax(noc,con,graph,max_node_id);
	//printf("Pmax=%d\n",Pm);
	converge=0;
	pass=0;
	while(!converge){
   
		pass=pass+1;
   printf("Pass = %d\n", pass);
	    Bucket_list(noc, con,graph,max_node_id, Pm, &bucket_A,&bucket_B, &maxgainA, &maxgainB);
		  //print_Bucket(bucket_A, bucket_B, Pm);
  		move_cell(noc,con,graph,max_node_id,Pm,bucket_A,bucket_B, &maxgainA, &maxgainB,base_cell, &cut, &converge);
   //printf("Converge=%d\n",converge);
    //printf("case main 1\n");
  		update_netarray(graph, noc, con, max_node_id);
     //printf("case main 2\n");
		  gain_fnc(noc,con,graph,max_node_id);
    //printf("case main 3\n");
    	
    // print_noc(graph,noc,max_node_id);
    // if(pass==100)break;
    
  }
  end = clock();
  cpu_time_used = ((double) (end - start))*1000000 / CLOCKS_PER_SEC;
  printf("TIME(ms)=%f\n", cpu_time_used);
}

























  
