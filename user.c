#include "circuit.h" 

int Tgat(NODE graph[MAX_NODES], int max_node_id){
	int count=0, i;
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0)
			count++;
	}
	return count;
}


void adj_func( NODE graph[MAX_NODES], CELL noc[MAX_NODES], NET con[MAX_NODES], int max_node_id, int Tgat){
	int i,a;
	a=0;
	LIST *temp_list;
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			if(a<(Tgat/2+Tgat%2))
			noc[i].BLK=0;
			else
			noc[i].BLK=1;
			a++;
      noc[i].Lock=0;
			//printf("%d %d %d\n",a, i, noc[i].BLK);
		}
	}
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			temp_list=graph[i].fanout;
			while(temp_list!=NULL){
				insert_list(&con[i].cells,temp_list->ID);
				if(noc[temp_list->ID].BLK==0)
					con[i].Na++;
				else con[i].Nb++;
				insert_list(&noc[temp_list->ID].nets,i);
				temp_list=temp_list->next;
			}		
			if(graph[i].fanout!=NULL){
				insert_list(&con[i].cells,i);
				if(noc[i].BLK==0)
					con[i].Na++;
				else con[i].Nb++;
				insert_list(&noc[i].nets,i);
			}
		}
		
	}
}

void print_noc(NODE graph[MAX_NODES], CELL noc[MAX_NODES], int max_node_id){     //Cell Array
	int i;
	printf("\nID\tCELL ARRAY\t\t\tBLK\t\tGain\n");
	for(i=0;i<=max_node_id;i++){
		if(graph[i].type!=0){
			printf("%d\t",i);
			print_list(noc[i].nets);
			printf("\t\t\t\t%d",noc[i].BLK);
			printf("\t\t\t%d\n",noc[i].Gain);		
		}	
	}

}

void print_con(NODE graph[MAX_NODES], NET con[MAX_NODES], int max_node_id){       //Net Array
	int i;
	printf("\nID\tNET ARRAY\t\tNa\tNb\n");
	for(i=0;i<=max_node_id;i++){
		if(graph[i].type!=0){
			printf("%d\t",i);
			print_list(con[i].cells);
			if(graph[i].fanout!=NULL){
				printf("\t\t\t%d\t%d",con[i].Na,con[i].Nb);
			} printf("\n");		
		}	
	}

}

void gain_fnc(CELL noc[MAX_NODES],NET con[MAX_NODES], NODE graph[MAX_NODES], int max_node_id){
	int i, F,T,Fn,Tn,g;
	LIST *temp_list;
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			temp_list=noc[i].nets;
			g=0;  //setteing initial Gain
			F=noc[i].BLK; //Storing Current Block Value for the cell
			while(temp_list!=NULL){
				if(F==0){
				Fn=con[temp_list->ID].Na;
				Tn=con[temp_list->ID].Nb;
				}
				else{
				Fn=con[temp_list->ID].Nb;
				Tn=con[temp_list->ID].Na;
				}
				if(Fn==1) g++;
				if(Tn==0) g--;
				noc[i].Gain=g;
				temp_list=temp_list->next;
			}
		}
	}
}


int Pmax(CELL noc[MAX_NODES],NET con[MAX_NODES], NODE graph[MAX_NODES], int max_node_id){
	int Pmax=0,p,i;
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			p=count_list(noc[i].nets);
			if(p>Pmax)
				Pmax=p;
		}
	}
	return Pmax;
}

void Bucket_list(CELL noc[MAX_NODES],NET con[MAX_NODES], NODE graph[MAX_NODES], int max_node_id, int Pmax, DLIST **bucket_A, DLIST **bucket_B, int *maxgainA,int *maxgainB){
	int gain, ml, i;
	int S;
	*maxgainA=*maxgainB=0;
	S=2*Pmax+1;
	*bucket_A=(DLIST*)malloc(S*sizeof(DLIST));
	*bucket_B=(DLIST*)malloc(S*sizeof(DLIST));
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			gain=noc[i].Gain;
			ml=Pmax+gain;				//ml= memory location
			if(noc[i].BLK==0){ 
				insert_dlist(&((*bucket_A)[ml].next),i);
				if(*maxgainA<ml){
					*maxgainA=ml;
				}
			}
			else{
				insert_dlist(&((*bucket_B)[ml].next),i);
				if(*maxgainB<ml){
					*maxgainB=ml;
				}
			}
		}
	}
}

void print_Bucket(DLIST *bucket_A, DLIST *bucket_B, int Pmax){
	int i;	
	printf("Bucket_List_A\n");
	printf("ID\tGain\tCells\t\n");		
	for(i=0; i<(2*Pmax+1);i++){
		printf("%d\t%d\t", i,i-Pmax);
		print_dlist(bucket_A[i].next);
		printf("\n");
	}
	printf("Bucket_List_B\n");	
	printf("ID\tGain\tCells\t\n");
	for(i=0; i<(2*Pmax+1);i++){
		printf("%d\t%d\t", i,i-Pmax);
		print_dlist(bucket_B[i].next);
		printf("\n");
	}

}


void move_cell(CELL noc[MAX_NODES],NET con[MAX_NODES], NODE graph[MAX_NODES], int max_node_id, int Pmax, DLIST *bucket_A, DLIST *bucket_B,int *maxgainA,int *maxgainB, int base_cell, int *cut_return, int *converge){
	int i, SA=0, SB=0;
	int F,T, Fn, Tn, gain;
	int base_cut, minimum_cut, Reverse, cut;
 	LIST *temp_con, *temp, *temp2;
	LIST *bcell_list=NULL, *cut_list=NULL;
 *cut_return=0;
	for(i=0; i<=max_node_id; i++){
		if(graph[i].type!=0){
			if(noc[i].BLK==0){
				SA++;				
			}
			else
				SB++;
		}
	}
	initial_cutset( con, max_node_id, &cut);
	base_cut=cut;
 //printf
 //print_con(graph,con,max_node_id);
 //print_noc(graph,noc,max_node_id);
	printf("Cut=%d\n",cut);
	int z=(2*Pmax+1);
	while(*maxgainA!=z || *maxgainB!=z){//{printf("Removing Cell %d from %d\t%d\t\%d\t%d\n",base_cell,*maxgainA, *maxgainB,SA,SB);
      //printf("uperr user 1\n");
      if(SA>=SB && *maxgainA!=z ){
      base_cell= bucket_A[*maxgainA].next->ID;
     // printf("removing cell %d from %d\n", basecell, maxgain_bkta );getchar();
      bucketlist_remove(&(bucket_A[*maxgainA].next), *maxgainA,base_cell);
      F=0;
      T=1;
      SA--;
      SB++;
    }
    else{
      base_cell= bucket_B[*maxgainB].next->ID; 
      bucketlist_remove(&(bucket_B[*maxgainB].next), *maxgainB,base_cell);
      F=1;
      T=0;
      SA++;
      SB--;
    }
  
		
	 
		//F=noc[base_cell].BLK;
		//T=1-F;
		LIST *temp_noc;
		DLIST *bucket;
		int c_buc,n_buc, flag;
		temp_noc=noc[base_cell].nets;
		noc[base_cell].Lock =1;
		noc[base_cell].BLK=T;
		while(temp_noc!=NULL){
			if(F==0){
				Fn=con[temp_noc->ID].Na;
				Tn=con[temp_noc->ID].Nb;	
			}
			else{
				Fn=con[temp_noc->ID].Nb;
				Tn=con[temp_noc->ID].Na;	
			}
			//printf("Tn=%d\tFn=%d",Fn,Tn);
			
			if(Tn==0){		// If Tn=0 increment gains of all free cells on net(n)
				temp_con=con[temp_noc->ID].cells;
        while(temp_con!=NULL){
					if(noc[temp_con->ID].Lock==0){
						c_buc=noc[temp_con->ID].Gain+Pmax;
						noc[temp_con->ID].Gain++;	
						n_buc=noc[temp_con->ID].Gain+Pmax;
						if(noc[temp_con->ID].BLK==0)
							bucket= bucket_A;        		
						else
							bucket=	bucket_B;
					        
						//printf("Moving Cell %d From IG %d to FG %d\n",noc[temp_con->ID],c_buc,n_buc);
						bucketlist_remove(&bucket[c_buc].next, c_buc,temp_con->ID);
						insert_dlist(&(bucket[n_buc].next),temp_con->ID);
		       			
					}
					temp_con=temp_con->next;;
				}
			}
			else if(Tn==1){ // If Tn=1 Decrement gain of only T cell of Net(n)
				temp_con=con[temp_noc->ID].cells;
				while(temp_con!=NULL){
					if(noc[temp_con->ID].Lock==0 && noc[temp_con->ID].BLK==T){
						c_buc=noc[temp_con->ID].Gain+Pmax;
						noc[temp_con->ID].Gain--;	
						n_buc=noc[temp_con->ID].Gain+Pmax;
						if(noc[temp_con->ID].BLK==0)
							bucket=	bucket_A;
						else
							bucket=	bucket_B;
						//printf("Moving Cell %d From IG %d to FG %d\n",noc[temp_con->ID],c_buc,n_buc);
						bucketlist_remove(&bucket[c_buc].next, c_buc,temp_con->ID);
						insert_dlist(&(bucket[n_buc].next),temp_con->ID);
					
					}
					temp_con=temp_con->next;
				}				
			}// print_Bucket(bucket_A, bucket_B, Pmax);
			if(F==0){//Chanage of distribution to reflect the move
				Fn=--con[temp_noc->ID].Na;
				Tn=++con[temp_noc->ID].Nb;	
			}	
			else{
				Fn=--con[temp_noc->ID].Nb;
				Tn=++con[temp_noc->ID].Na;	
			}//printf("Fn=%d,Tn=%d",Fn,Tn);
			if(Fn==0){
				temp_con=con[temp_noc->ID].cells;
				while(temp_con!=NULL){
					if(noc[temp_con->ID].Lock==0){
						c_buc=noc[temp_con->ID].Gain+Pmax;
						noc[temp_con->ID].Gain--;	
						n_buc=noc[temp_con->ID].Gain+Pmax;
						if(noc[temp_con->ID].BLK==0)
							bucket=	bucket_A;
						else
							bucket=	bucket_B;
						//printf("Moving Cell %d From IG %d to FG %d\n",noc[temp_con->ID],c_buc,n_buc);
		      			bucketlist_remove(&bucket[c_buc].next, c_buc,temp_con->ID);
						insert_dlist(&(bucket[n_buc].next),temp_con->ID);
					}
					temp_con=temp_con->next;
				}
			}
			else if(Fn==1){
			temp_con=con[temp_noc->ID].cells;
				while(temp_con!=NULL){
					if(noc[temp_con->ID].Lock==0 && noc[temp_con->ID].BLK==T){
						c_buc=noc[temp_con->ID].Gain+Pmax;
						noc[temp_con->ID].Gain++;	
						n_buc=noc[temp_con->ID].Gain+Pmax;
						if(noc[temp_con->ID].BLK==0)
							bucket=	bucket_A;
						else
							bucket=	bucket_B;
						//printf("Moving Cell %d From IG %d to FG %d\n",noc[temp_con->ID],c_buc,n_buc);
						bucketlist_remove(&bucket[c_buc].next, c_buc,temp_con->ID);
						insert_dlist(&(bucket[n_buc].next),temp_con->ID);                                            
					}
					temp_con=temp_con->next;
				}				
			}
			temp_noc=temp_noc->next;
		}
	cut=cut-noc[base_cell].Gain;
	insert_cutlist(&bcell_list,base_cell);
	insert_cutlist(&cut_list,cut);
	*maxgainA=maxgain_pointer(bucket_A, Pmax);
	*maxgainB=maxgain_pointer(bucket_B,Pmax);
	}
	
  temp2 = cut_list;
	minimum_cut=cut;
 //print_list(temp2);
 //printf("\n");
 //print_list(temp);
 //getchar();
	while(temp2!=NULL){
		if(minimum_cut>temp2->ID)
			minimum_cut=temp2->ID;
		temp2=temp2->next;
	}
	printf("Minimum Cut=%d\n",minimum_cut);
 printf("BAse Cut=%d\n",base_cut);
  temp = bcell_list;
  ///print_list(temp);
  temp2 = cut_list;
	if(minimum_cut>=base_cut){
		Reverse=1;
		*converge=1;
	}
	else{
  	Reverse=0;
  	*converge=0;
	 // printf(" user case1\n");
	}
    ///print_list(temp2);
  	*cut_return=minimum_cut;
    printf("Cut_return=%d\n",*cut_return);
    ///print_list(temp2);
    ///printf("user case 4\n");
	while(temp2!=NULL){
     //printf("base_cell=%base_cell\n",base_cell);
		 base_cell=temp->ID;
		 noc[base_cell].Lock=0;
		 if(Reverse==1){
       noc[base_cell].BLK=1-noc[base_cell].BLK;
     }
     if(minimum_cut==temp2->ID)
			Reverse=1;
	temp= temp->next;
	temp2=temp2->next;
	}//printf("finish %d",*converge);

	
}

void bucketlist_remove(DLIST **bucket, int maxg_b, int bcell){	
	DLIST *temp_list;
	temp_list=*bucket;
	while(temp_list!=NULL){
		if(temp_list->ID==bcell){
			if(temp_list->prev!=NULL)
				temp_list->prev->next=temp_list->next;
			else
				*bucket=temp_list->next;
			if(temp_list->next!=NULL)
				temp_list->next->prev=temp_list->prev;
			free(temp_list);
			break;
		}
		temp_list=temp_list->next;	
	}
}

void initial_cutset(NET con[MAX_NODES], int max_node_id, int *cut){
	int i;
  *cut=0;	
	for(i=0;i<max_node_id;i++){
		if(con[i].Na>0 && con[i].Nb>0)
			*cut=*cut+1;
     // printf("Initialize\n");
	}
}


int maxgain_pointer(DLIST *bucket, int Pmax){
	int i;
	int S=2*Pmax+1;
	for(i=S-1;i!=0;i--){
	  if(bucket[i].next!=NULL)
	  	return i;	
 	}
  return S;
}

void update_netarray( NODE graph[MAX_NODES], CELL noc[MAX_NODES], NET con[MAX_NODES], int max_node_id){
	int i;
	LIST *temp;
	for(i=0; i<=max_node_id; i++){
		con[i].Na=con[i].Nb=0;
		if(graph[i].type!=0){
			temp=graph[i].fanout;
			while(temp!=NULL){
				if(noc[temp->ID].BLK==0)
					con[i].Na++;
				else con[i].Nb++;
        temp=temp->next;
			}
			if(graph[i].fanout!=NULL){
				if(noc[i].BLK==0)
					con[i].Na++;
				else con[i].Nb++;
			}
		}
   //printf("netarray\n");
	}
}


