#include "header.h"
 
void *waiter_run(void *args)
{
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t *)args;

	/* Get id */
	uint8_t waiter_id = args_ptr->id;

	/* Create private order and pizza semaphore */
	sem_t order;
	sem_t pizza;
	sem_init(&order, 0, 0);
	sem_init(&pizza, 0, 0);

	/* Print status and signal the init_done semaphore */
	fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t created successfully.\n", waiter_id);
	sem_post(args_ptr->init_done);

	/* Infinite loop */
	while (1)
	{
		/* Wait for customer */
		sem_wait(&customer);

		/* Check if we should terminate */
		if (interrupt)
		{
			break;
		}

		/* print status */
		fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t is currently serving customer .\n", waiter_id);

		/* Save my id to the exchange */
		waiter_exchange.order = &order;
		waiter_exchange.pizza = &pizza;
		waiter_exchange.id = waiter_id;

		/* Tell customer that I am awake and information is placed */
		sem_post(&waiter_awake);

		/* Wait for the order and print sttaus */
		sem_wait(&order);
		fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t has recieved the order.\n", waiter_id);

		/* Print status that now the pizza will be get from the tray */
		fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t is going to tray to get the pizza...\n", waiter_id);

		/* Go to tray */
		sleep(rand() % WAITING_TIME);

		/* Acquire waiter semaphore */
		sem_wait(&waiter);

		/* Lock tray and get pizza */
		sem_wait(&tray);
		assure_state();
		pizza_count--;
		assure_state();
		sem_post(&tray);

		/* Signal a waiting chef a new pizza can be produced */
		sem_post(&chef);

		/* Got successfull a pizza. Print status */
		fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t has picked up pizza from the tray, on way to serve...\n", waiter_id);

		/* Go back to customer */
		sleep(rand() % WAITING_TIME);

		/* Give pizza to customer and print status */
		sem_post(&pizza);
		fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t served pizza to customer.\n", waiter_id);
	}

	/* free semaphores and print message */
	sem_destroy(&order);
	sem_destroy(&pizza);
	fprintf(fptr,"--->\t Message from [cashier()]: waiter with id \t[%d]\t completed his job.\n", waiter_id);

	return NULL;
}
