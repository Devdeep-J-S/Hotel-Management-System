
#include "header.h"

void assure_state()
{
	/* Assure that more than zero pizzas are available */
	if (pizza_count < 0)
	{
		fprintf(fptr,"--->\t ERROR in function ASSURE_STATE: Negative pizza count!\n");
		exit(150);
	}

	/* Assure that not more pizzas than tray spaces are available */
	if (pizza_count > TRAY_CAPACITY)
	{
		fprintf(fptr,"--->\t ERROR in function ASSURE_STATE: Tray is full already!\n");
		exit(153);
	}

	fprintf(fptr,"--->\t Message from [assure_state()]:\t State consistent.\n");
}
