#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <time.h>
 /* NODE TYPE CONSTANTS */
#define INPT	1				/* Primary Input*/
#define AND   	2				/* AND       */
#define NAND 	3				/* NAND      */
#define OR  	4				/* OR        */
#define NOR 	5				/* NOR       */
#define NOT  	6				/* INVERTER  */
#define OUTPUT  7				/* OUTPUT    */ 
#define BUFF    8				/* BUFFER    */

#define MAX_NODES   25000

typedef struct list
{
   int ID;
   struct list *next;
} LIST;


typedef struct node
{
  int type;			//Gate Type
  int fanout_num;	//Number of fan-outs from a gate
  int fanin_num;	// Number of fan-ins to a gate
  int output;
  LIST *fanin;		//pointer to Linked list which represent the Fan-ins to a gate.
  LIST *fanout;	//pointer to Linked list which represent the Fan-outs from a gate.
 
} NODE;

/*-----------------------------------------------------------------
                  New Structures
------------------------------------------------------------------*/   

typedef struct cell
{
LIST *nets;
int BLK;//Partitionb Id
int Gain;
int Lock;

}CELL;

typedef struct net
{
LIST *cells;
int Nb,Na;// Number of cells in partition(b,a) connected to that net

}NET;


typedef struct dlist
{
int ID;
struct dlist *next;
struct dlist *prev;
}DLIST; // Structure for the Bucket List




/*---------------------------------------------------------------------------
 			Function Prototypes                                                       
---------------------------------------------------------------------------*/

int read_circuit(FILE*,NODE[MAX_NODES],LIST**,LIST**);
void insert_list(LIST **, int);  
void initialize_graph(NODE *); 
void charcat(char * ,char );  
int assign_type(char *);
void print_graph(NODE [MAX_NODES]);
void print_list(LIST *);
int count_list(LIST *); 
void insert_cutlist(LIST **, int);  




/*-----------------------------------------------------------------
                  New Function Prototype
------------------------------------------------------------------*/   

void adj_func( NODE[MAX_NODES], CELL[MAX_NODES], NET[MAX_NODES], int, int);
void print_con(NODE [MAX_NODES], NET [MAX_NODES], int);
void print_noc(NODE [MAX_NODES], CELL [MAX_NODES], int);
int Tgat(NODE [MAX_NODES], int);
void gain_fnc(CELL[MAX_NODES], NET[MAX_NODES], NODE[MAX_NODES], int);
int Pmax(CELL [MAX_NODES],NET [MAX_NODES], NODE [MAX_NODES], int );
void insert_dlist(DLIST ** , int); 
void Bucket_list(CELL [MAX_NODES],NET [MAX_NODES], NODE [MAX_NODES], int , int , DLIST **, DLIST **, int *, int *);
void print_dlist(DLIST *);
void print_Bucket(DLIST *, DLIST *, int);
void move_cell(CELL [MAX_NODES],NET [MAX_NODES], NODE [MAX_NODES], int , int , DLIST *, DLIST *,int *,int *, int , int *,int *);
void bucketlist_remove(DLIST **, int , int );
void initial_cutset(NET [MAX_NODES], int , int *);
void update_netarray( NODE [MAX_NODES], CELL [MAX_NODES], NET [MAX_NODES], int);





















 

