#include"circuit.h"

/*-----------------------------------------------------------
         Routine to read the Bench Mark
-------------------------------------------------------------*/

int read_circuit(FILE*fp_bench,NODE graph[MAX_NODES],LIST** in,LIST** out)
{

    char  c,node_type[10],node_num[10],line[80];
    int   i,node_no,node_count=0,max_node_id=0;

 
   for(i=0;i<MAX_NODES;i++)
   initialize_graph(&graph[i]);
   
   do
  	fgets(line,100,fp_bench);
   while(line[0]=='#');
   
   while(!feof(fp_bench))
   {
     	bzero(node_num, strlen(node_num));
	 	bzero(node_type, strlen(node_type));
	 
	 
     	if(line[0]=='#' || line[0]=='\n')  
     		fgets(line,100,fp_bench);
       
        /*------------------------Feeding the Inputs------------------------------*/
        
     	if(line[0]=='I')
     	{
            	
     		strcpy(node_type,"INPUT");
       	i=0;
        	while(line[i]!='(')
         	i++;
            
        	while(line[i]!=')')
     		{
      		i++;
      		charcat(node_num,line[i]);
      	}
             	  
      	graph[atoi(node_num)].type=assign_type(node_type);
         insert_list(in,atoi(node_num));
     	}   
         
        /*-------------------------Feeding the Outputs------------------------------*/  
        
      else if(line[0]=='O')
      {
      	strcpy(node_type,"OUTPUT");
      	i=0;
      	while(line[i]!='(')
      	i++;
          
      	while(line[i]!=')')
      	{
      		i++;
      		charcat(node_num,line[i]);
      	}
            	   
      	graph[atoi(node_num)].type=assign_type(node_type);
      	graph[atoi(node_num)].fanout_num=0;
      	graph[atoi(node_num)].output=1;
        	insert_list(out,atoi(node_num));
       	max_node_id = atoi(node_num);
           	
     	}
         
          /*-----------------------Feeding the Internal Gates------------------------------*/ 
     else 
     {
     		i=0;
           
     		while(line[i]!= '=')
     		{
     		    	charcat(node_num,line[i]);
     				i++;
        	} 
      	i++;
              
      	while(line[i]!='(')
        	{
        		i++;
          	if(line[i]!='(') charcat(node_type,line[i]);
       	}
                  
       	graph[atoi(node_num)].type=assign_type(node_type);  
        	node_no=atoi(node_num);
        	bzero(node_num, strlen(node_num));
	  		bzero(node_type, strlen(node_type));
	  		i++;
	  
	     	while(line[i]!='\0')
	     	{
	      	if(line[i]!= ',')
	      	charcat(node_num,line[i]);
	     
	     
	      	if(line[i]== ',' || line[i]== ')') 
	     		{
	      		insert_list(&graph[node_no].fanin,atoi(node_num));
	      		insert_list(&graph[atoi(node_num)].fanout,node_no);
	      	   node_count=atoi(node_num);
	        	   bzero(node_num, strlen(node_num));
	        	   i++;
	     		} 
	     	   i++;
	     	   graph[node_no].fanin_num=count_list(graph[node_no].fanin);
	  	    	graph[node_count].fanout_num=count_list(graph[node_count].fanout);
	   
	  	 	}  
	  	
   	}/*------------------End of feeding gates----------------------------*/ 
        
 	 	bzero(line, strlen(line));  
      fgets(line,100,fp_bench);
         
  	}/*end while*/
   fclose(fp_bench);      
	return max_node_id;

}     

/*-----------------------------------------------------------
         concatante a character to a string 
-------------------------------------------------------------*/

void charcat(char *s, char b)
{
  while(*s!='\0')
    {
      *s++;
    }
   *s=b;
}


/*--------------------------------------------------------------
         convert (char *) type read to (int)     
----------------------------------------------------------------*/
int assign_type(char *type)
{
 if(strcmp(type,"INPUT")==0)  		return 1;
  else if(strcmp(type,"AND")==0)		return 2;
  else if(strcmp(type,"NAND")==0)   return 3;
  else if(strcmp(type,"OR")==0)     return 4;
  else if(strcmp(type,"NOR")==0)    return 5;
  else if(strcmp(type,"NOT")==0)    return 6;
  else if(strcmp(type,"OUTPUT")==0) return 7;
  else if(strcmp(type,"BUFF")==0)	return 8;
  else                              return 0;
}


/*--------------------------------------------------------------
               Initialize the Graph
----------------------------------------------------------------*/           
void initialize_graph(NODE *nd)
{
   nd->type=0;
   nd->fanout_num=0;
   nd->fanin_num=0;
   nd->output=0;
   nd->fanin=NULL;
   nd->fanout=NULL;
}




/*------------------------------------------------------------------
                   Insert List
---------------------------------------------------------------------*/
void insert_list(LIST ** head, int num)  
{
   LIST *temp,*tail;
   
   
   if((temp = (LIST*)malloc(sizeof(LIST)))==NULL)
   {
     printf("INSER_LIST out of memory\n");
     exit(1);
   }
   
   temp->ID=num;
   temp->next=NULL;
   
   if(*head==NULL)
   *head=temp;
   else
   {
     tail=*head;
     while(tail->next!=NULL)
	{
		if(num==tail->ID){   //Avoid Duplication
			free(temp);
			return;		
		}
     tail=tail->next;
	}
	tail->next=temp;
   }   
       
}

/*------------------------------------------------------------------
                   Insert dList
---------------------------------------------------------------------*/
void insert_dlist(DLIST ** head, int num)  
{
   DLIST *temp,*next;
   
   
   if((temp = (DLIST*)malloc(sizeof(DLIST)))==NULL)
   {
     printf("INSER_LIST out of memory\n");
     exit(1);
   }
   
	temp->ID=num;
	temp->next=NULL;
	temp->prev = NULL;
   
   if(*head==NULL)
   *head=temp;
   else
   {
     next=*head;
     while(next->next!=NULL)
	{
		if(num==next->ID){   //Avoid Duplication
			free(temp);
			return;		
		}
     next=next->next;
	}
	next->next=temp;
	temp->prev=next;
   }   
       
}

/*----------------------------------------------------------------
                    PRINT LIST
 -----------------------------------------------------------------*/   
void print_list(LIST *head)   
{
 
  while(head!=NULL)
   {
     printf("%d ",head->ID);
     head=head->next;
   }
}

/*-----------------------------------------------------------------
                  COUNT LIST
------------------------------------------------------------------*/                  
int count_list(LIST *head)   
{
  int count=0;
  while(head!=NULL)
   {
     count++;
     head=head->next;
   }
   return count;
}

/*-----------------------------------------------------------------
                  PRINT GRAPH
------------------------------------------------------------------*/   

void print_graph(NODE graph[MAX_NODES])
{
	int i;
	printf("NODE\tGATE\tIn #\tOut #\tFanin\tFanout\n");
	for(i=1;i<MAX_NODES;i++)
	{
	  if(graph[i].type!=0)
	  {
	    printf("%d\t%d\t%d\t%d\t",i,graph[i].type,graph[i].fanin_num,graph[i].fanout_num);
	  
	    print_list(graph[i].fanin);
	    printf("\t");
	    print_list(graph[i].fanout);
	    printf("\n");
	  }
	}
}

/*----------------------------------------------------------------
                    PRINT DLIST
 -----------------------------------------------------------------*/   
void print_dlist(DLIST *head)   
{
 
  while(head!=NULL)
   {
     printf("%d ",head->ID);
     head=head->next;
   }
}

/*------------------------------------------------------------------
                   Insert CutList
---------------------------------------------------------------------*/
void insert_cutlist(LIST ** head, int num)  
{
   LIST *temp,*tail;
   
   
   if((temp = (LIST*)malloc(sizeof(LIST)))==NULL)
   {
     printf("INSER_LIST out of memory\n");
     exit(1);
   }
   
   temp->ID=num;
   temp->next=NULL;
   
   if(*head==NULL)
   *head=temp;
   else
   {
     tail=*head;
     while(tail->next!=NULL)
	{
     tail=tail->next;
	}
	tail->next=temp;
   }   
       
}













