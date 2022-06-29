
#include "header.h"

void *customer_run(void *args)
{
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t *)args;

	/* Get id */
	uint8_t customer_id = args_ptr->id;

	/* Print status and signal the init_done semaphore */
	fprintf(fptr,"--->\t Message from [customer_run()]: customer with id \t[%d]\t arrived.\n", customer_id);
	sem_post(args_ptr->init_done);

	/* Wait for some random time to mix customers (wait at least 1 second to assure that all
	   customers are already created) */
	sleep(rand() % WAITING_TIME + 1);

	/* Synchronize all customers to get a waiter. This will queue up the
	   customers and guarantees every customer to get a waiter without race
	   conditions */
	sem_wait(&customer_private);

	/* Signal waiter that a customer is in the room and wait for a waiter to
	   wake up */
	sem_post(&customer);
	sem_wait(&waiter_awake);

	/* The waiter has placed his information in the waiter_exchange variable.
	   -> Get it! */
	sem_t *order = waiter_exchange.order;
	sem_t *pizza = waiter_exchange.pizza;
	uint8_t waiter_id = waiter_exchange.id;

	/* Leave synchronized area, now the next customer may aquire a waiter */
	sem_post(&customer_private);

	/* Print a status about the approached waiter */
	fprintf(fptr,"--->\t Message from [customer_run()]: customer with id \t[%d]\t approached waiter no. \t[%d]\t.\n",
		   customer_id, waiter_id);

	/* Print status that now the order will be placed */
	fprintf(fptr,"--->\t Message from [customer_run()]: customer with id\t[%d]\t is placing order to waiter having id \t[%d]\t.\n",
		   customer_id, waiter_id);

	/* Place order */
	sleep(rand() % WAITING_TIME);

	/* Tell waiter about the order */
	sem_post(order);

	/* Wait for waiter to hand over the pizza */
	sem_wait(pizza);

	/* Process done, pizza received. Print status and exit */
	fprintf(fptr,"--->\t Message from [customer_run()]: customer with id \t[%d]\t got pizza from waiter no. \t[%d]\t. Thank you!\n",
		   customer_id, waiter_id);
	fprintf(fptr,"--->\t Message from [customer_run()]: customer with id \t[%d]\t has completed his work...\n", customer_id);

	return NULL;
}

