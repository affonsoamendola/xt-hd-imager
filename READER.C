#include <conio.h>
#include <dos.h>
#include <bios.h>

void main()
{
	char buffer[512];

	int i;
	int disk;
	int head;
	int sector;
	int track;
	int nsec;
	
	printf("Enter number of the disk to read:\n");
	scanf("%i", &disk);
	printf("Enter number of the head to read:\n");
	scanf("%i", &head);
	printf("Enter number of the track to read:\n");
	scanf("%i", &track);
	printf("Enter number of the sector to read:\n");
	scanf("%i", &sector);

	for(i=0;i<512;i++)
	{

		buffer[i] = 'Z';

	}

	printf("\nREADING DISK 2, HEAD 1, TRACK 1, SECTOR 1...\n\n");

	printf("%i\n", biosdisk(2, disk, head, track, sector, 1, buffer));

	for(i=0;i<512; i++)
	{
		
		char currentData[2];
		

		//Converting Raw data into readable hex codes, first time doing this, and I have literally no idea if it will work,

		currentData[0]= buffer[i] & 240
		//Masking with 11110000, returning XXXX0000
		currentData[0]= currentData[0] >> 4
		//Shifting bits to the right, returning 0000XXXX
		currentData[0]= currentData + 48
		//Adding 48 to place the characters on the right point of the ascii table
		if(currentData[0] > 57)
		{
			currentData[0]= currentData + 8
			//Add 8 to skip the punctuations on the ascii table and get to A B C, etc
		}
		
		currentData[1]= buffer[i] & 15
		//Masking with 00001111 returning only 0000XXXX
		currentData[1]= currentData + 48
		//Adding 48 to place the characters on the right point of the ascii table
		if(currentData[1] > 57)
		{
			currentData[1]= currentData + 8
			//Add 8 to skip the punctuations on the ascii table and get to A B C, etc
		}

		printf("%c", currentData[0]);
		printf("%c", currentData[1]);
	}
}
