#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int frobcmp(char const *a, char const *b)
{
	//Compare length of each array assuming one or both are empty
	if (a[0] == ' ' && b[0] != ' ')
		return -1;
	else if (a[0] == ' ' && b[0] == ' ')
		return 0;
	else if (a[0] != ' ' && b[0] == ' ')
		return 1;

	int i = 0;
	while ((a[i] != ' ') && (b[i] != ' '))
	{
		/*Looking at memfrob documentation online, it shows the function transforms each byte of
		the string by bitwise XOR with binary 00101010 which translate to 32 + 8 + 2 = 42 */
		if ((a[i] ^ 42) < (b[i] ^ 42))
			return -1;
		else if ((a[i] ^ 42) == (b[i] ^ 42))
			i++;
		else if ((a[i] ^ 42) > (b[i] ^ 42))
			return 1;
	}

	return 0;
}

/*According to a Piazza post, we need to use a function to be able to utilize frobcmp with
void data type variables for qsort.*/
int qsortHelper(const void *a, const void *b)
{
	return frobcmp(*(char const**)a, *(char const**)b);
}

int main()
{
	//Size and counters for input list
	int size = 512;
	int counter_1 = 0;
	int list_1_index = 0;

	/*Looking at documentation online, apparently, this is how to declare dynamic memory allocation.
	Also, there were some notes on end-of-file(EOF), but doesn't really make sense.*/
	char *list_1 = (char*)malloc(size * sizeof(char));
	int current;

	while ((current = getchar()) != EOF)
	{
		//Add character to list and move counter
		list_1[list_1_index++] = (char)current;
		counter_1 = counter_1 + sizeof(char);

		//Check if we reached max amount of memory
		if (counter_1 == size)
		{
			//Reallocation of memory
			list_1 = (char*)realloc(list_1, size * 4);

			//Error trapping
			if (list_1 == NULL)
			{
				fprintf(stderr, "Error in reallocationg memory.");
				exit(1);
			}

			//Update size of list
			size = size * 4;
		}
	}

	//Check for the last character to be space byte
	if (list_1[list_1_index - 1] != ' ')
	{
		list_1[list_1_index++] = ' ';
	}

	//Variable for # of characters in input
	int elements = 0;
	int counter_2 = 0;
	int i = 0;

	//Count number of elements in list_1
	while (i < list_1_index)
	{
		if (list_1[i] == ' ')
			elements++;
		i++;
	}

	//Dynamic allocation of output
	char **list_2 = (char**)malloc(sizeof(char*) * elements);

	//Error trapping
	if (list_2 == NULL)
	{
		fprintf(stderr, "Error in reallocationg memory.");
		exit(1);
	}

	int list_2_index = 0;
	int j = 0;

	//Fill list_2 with location of each element
	while(j < list_1_index)
	{
		list_2[list_2_index++] = &list_1[j];
		
		while (list_1[j] != ' ')
		{
			j++;
		}

		j++;
	}

	//Use qsort as specified by spec for sorting
	qsort(list_2, elements, sizeof(char*), qsortHelper);

	char *c;
	int k = 0;

	//Print out characters
	while(k < elements)
	{
		c = list_2[k];
		while (*c != ' ')
		{
			putchar((int)(*c));
			c++;
		}
		putchar((int)(*c));
		k++;
	}

	//Free memory to avoid error
	free(list_1);
	free(list_2);

	exit(0);
}