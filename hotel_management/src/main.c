#include "header.h"
uint8_t pizza_count =0 ;


int main(int argc, char **argv)
{
	/*true when the threads should terminate themselves */
	interrupt = false;
	
	fptr = fopen("log.txt", "a");
	printf("Enter the number of customers : ");  
	scanf("%d", &CUSTOMER_COUNT);
	/* random number generator */
	srand(time(NULL));

	int pid = fork();
	
	if(pid == 0)
	{
		execlp("./alarm.out", "./alarm.out", NULL);
	}
	/* initialized all local semaphores */
	
	sem_init(&tray, 0, 1);
	sem_init(&waiter, 0, 1);
	sem_init(&waiter_awake, 0, 0);
	sem_init(&chef, 0, TRAY_CAPACITY);
	sem_init(&customer, 0, 0);
	sem_init(&customer_private, 0, 1);

	/* Created semaphore to synchronize thread init and args */
	simple_arg_t args;
	sem_t init_done;
	sem_init(&init_done, 0, 0);
	args.init_done = &init_done;


	/* Start all chef threads */
	pthread_t chefs[CHEF_COUNT];
	for (uint8_t i = 0; i < CHEF_COUNT; i++)
	{
		/* Set id for chef */
		args.id = i;

		/* Start chef thread and pass args, handle possible errors */
		if (pthread_create(chefs + i, NULL, chef_run, (void *)&args))
		{
			perror("--->\t Message from [main()]: \t ERROR: Unable to create chef thread.\n");
			exit(1);
		}

		/* Wait until the chef is initialized and ready to run */
		sem_wait(&init_done);
	}

	/* Start all waiter threads */
	pthread_t waiters[WAITER_COUNT];
	for (uint8_t i = 0; i < WAITER_COUNT; i++)
	{
		/* Set id for waiter */
		args.id = i;

		/* Start waiter thread and pass args, handle possible errors */
		if (pthread_create(waiters + i, NULL, waiter_run, (void *)&args))
		{
			perror("--->\t Message from [main()]: \t ERROR: Unable to create waiter thread.\n");
			exit(2);
		}

		/* Wait until the waiter is initialized and ready to run */
		sem_wait(&init_done);
	}

	/* Start all customer threads */
	pthread_t customers[CUSTOMER_COUNT];
	for (uint8_t i = 0; i < CUSTOMER_COUNT; i++)
	{
		/* Set id for customer */
		args.id = i;

		/* Start customer thread and pass args, handle possible errors */
		if (pthread_create(customers + i, NULL, customer_run, (void *)&args))
		{
			perror("--->\t Message from [main()]:\t ERROR: Unable to create customer thread.\n");
			exit(3);
		}

		/* Wait until the customer is initialized and ready to run */
		sem_wait(&init_done);
	}

	/* destroyed init semaphore */
	sem_destroy(&init_done);

	/* wait for all customer threads to finish */
	for (uint8_t i = 0; i < CUSTOMER_COUNT; i++)
	{
		/* combine all customer threads and handle all possible errors*/
		if (pthread_join(customers[i], NULL))
		{
			perror("--->\t Message from [main()]:\t ERROR: Unable to join cutomers[%d]\n");
			exit(4);
		}
	}

	/*
	 * The process is done now and cleaning up!!
	 */
	 
    //open file sample.txt in write mode
    
	fprintf(fptr, "--->\t Message from [main()]:\t All customers terminated SUCCESSFULLY\n");
	fprintf(fptr, "\n----------------\n\n--->\t Message from [main()]:\t\t SUCCESS: Starting Cleanup\n");

	/* Set interrupt flag true for terminating threads */
	interrupt = true;
	

	/* Wake all chefs up, they will see the interrupt flag and will exit */
	for (uint8_t i = 0; i < CHEF_COUNT; i++)
	{
		sem_post(&chef);
	}

	/* Wake all waiters up, they will see the interrupt flag and will exit */
	for (uint8_t i = 0; i < WAITER_COUNT; i++)
	{
		sem_post(&customer);
	}

	/* All threads were woken up */
	fprintf(fptr, "--->\t Message from [main()]:\t SUCCESS: Told all threads to terminate themselves\n");

	/* wait for all chef threads to finish */
	for (uint8_t i = 0; i < CHEF_COUNT; i++)
	{
		/* Join customer and handle possible errors */
		if (pthread_join(chefs[i], NULL))
		{
			perror("--->\t Message from [main()]:\t ERROR: Unable to join chefs[%d]\n");
			exit(5);
		}
	}

	/* wait for all waiter threads to finish */
	for (uint8_t i = 0; i < WAITER_COUNT; i++)
	{
		/* Join customer and handle possible errors */
		if (pthread_join(waiters[i], NULL))
		{
			perror("--->\t Message from [main()]:\t ERROR: Unable to join waiters[%d]\n");
			exit(6);
		}
	}

	/* Print status and exit */
	assure_state();
	fprintf(fptr, "--->\t Message from [main()]:\t SUCCESS: All threads terminated, state consistent.\n");
	printf("Log file created.\n");
	kill(pid, SIGINT);
	fclose(fptr);
}


