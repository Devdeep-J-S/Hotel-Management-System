
#include "header.h"

void *chef_run(void *args)
{
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t *)args;

	/* Get id */
	uint8_t chef_id = args_ptr->id;

	/* Print status and signal the init_done semaphore */
	fprintf(fptr,"--->\t Message from [chef()]: chef with id \t[%d]\t created successfully.\n", chef_id);
	sem_post(args_ptr->init_done);

	while (1)
	{
		/* Acquire chef semaphore */
		sem_wait(&chef);

		/* Check if we should terminate */
		if (interrupt)
		{
			break;
		}

		/* chef */
		sleep(rand() % WAITING_TIME);

		/* Lock tray and produce pizza */
		sem_wait(&tray);
		assure_state();
		pizza_count++;
		assure_state();
		sem_post(&tray);

		fprintf(fptr,"--->\t Message from [chef()]: chef with id \t[%d]\t placed a new pizza in the tray.\n", chef_id);

		/* Signal a waiting waiter */
		sem_post(&waiter);
	}

	fprintf(fptr,"--->\t Message from [chef()]: chef with id \t[%d]\t has completed his work...\n", chef_id);
	return NULL;
}
