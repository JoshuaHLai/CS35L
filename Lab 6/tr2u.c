#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	//Check if number of arguments is correct
	if (argc != 3)
	{
		fprintf(stderr, "Wrong number of arguments.");
		exit(1);
	}

	//Set variables to the two different arguments
	char* from = argv[1];
	char* to = argv[2];

	//Set length of each argument
	int lenFrom = strlen(from);
	int lenTo = strlen(to);

	//Check if arguments are not equal in length
	if (lenFrom != lenTo)
	{
		fprintf(stderr, "Arguments are not the same size.");
		exit(1);
	}

	//Check for duplicate bytes
	int i = 0;
	while (i < lenFrom)
	{
		int j = i + 1;
		while(j < lenFrom)
		{
			if(from[i] == from[j])
			{
				fprintf(stderr, "Duplicate bytes detected.");
				exit(1);
			}
			j++;
		}
		i++;
	}

	//Declare array for characters
	char current[1];
	int check = 0;;
	while(read(0, current, 1) > 0)
	{
        int k = 0;

        //Loop through file to translate bytes
        while(k < lenFrom)
        {
        	if(current[0] == from[k])
        	{
        		current[0] = to[k];
        		write(1, current, 1);
        		check = 1;
        		break;
        	}
        	k++;
        }
        if(check == 1)
        	write(1, current, 1);
	}

	exit(0);
}