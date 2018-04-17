#include <conio.h>
#include <dos.h>
#include <bios.h>

void main()
{


	char buffer[512];

	int i;

	for(i=0;i<512;i++)
	{

		buffer[i] = 'Z';

	}

	printf("\nREADING HEAD 1, TRACK 1, SECTOR 1...\n\n");

	printf("%i\n", biosdisk(2, 2, 1, 1, 1, 1, buffer));

	for(i=0;i<512; i++)
	{

		printf("%c", buffer[i]);

	}

}