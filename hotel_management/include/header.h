#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void *chef_run();
void *waiter_run();
void *customer_run();
void assure_state(); 

#define CHEF_COUNT 			3
#define WAITER_COUNT		2
int CUSTOMER_COUNT ; 
#define TRAY_CAPACITY	4
#define WAITING_TIME		5


/* Used this flag variable for terminating all the threads, after completion of the program */
bool interrupt ;

/* define a structure representing a waiter */
typedef struct
{
	uint8_t id;
	sem_t *order;
	sem_t *pizza;
} waiter_t;

/* define a structure to pass arguments to the respective _run functions */
typedef struct
{
	uint8_t id;
	sem_t *init_done;
} simple_arg_t;



/* Defined some globally needed semaphores */
sem_t tray;
sem_t chef;
sem_t waiter;
sem_t waiter_awake;
sem_t customer;
sem_t customer_private;

/* Defined this semaphore to exchange information between some local semaphores*/
waiter_t waiter_exchange;

/* Defined the counter for available pizzas in the tray */
uint8_t pizza_count ;

FILE *fptr;
