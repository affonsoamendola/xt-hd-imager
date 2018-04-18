#include <conio.h>
#include <dos.h>
#include <bios.h>
//Comment these three so that the code will compile on modern OS's, these should compile with Borland C++, Turbo C and the like, 
//try with some 90s C compiler, it should work

//#include <stdio.h>

char * convertByteToHexCode(char c_input);

int main()
{
	char c_buffer[512];

	int i_disk;
	int i_head;
	int i_sector;
	int i_track;
	int i_nsec;

	int i_errorCode;
	
	printf("Enter number of the disk to read:\n");
	scanf("%i", &i_disk);
	printf("Enter number of the head to read:\n");
	scanf("%i", &i_head);
	printf("Enter number of the track to read:\n");
	scanf("%i", &i_track);
	printf("Enter number of the sector to read:\n");
	scanf("%i", &i_sector);

	int i;

	for(i=0;i<512;i++)
	{

		c_buffer[i] = 10;

	}

	printf("\nREADING DISK %i",i_disk);
	printf(", HEAD %i",i_head);
	printf(", TRACK %i",i_track);
	printf(", SECTOR %i...\n\n",i_sector);

	i_errorCode = biosdisk(2, i_disk, i_head, i_track, i_sector, 1, c_buffer));
	//i_errorCode = 0;

	if(i_errorCode == 0)
	{
		int j = 0;

		for(i=0;i<512; i++,j++)
		{

			if(j > 15)
			{
				//Making pretty columns
				printf("\n");
				j=0;

			}	

			char c_hexcode[2];
			c_hexcode[0] = *convertByteToHexCode(c_buffer[i]);
			c_hexcode[1] = *convertByteToHexCode(c_buffer[i]);

			printf("%c", c_hexcode[0]);
			printf("%c ", c_hexcode[1]+1);
		}
	}
	else
	{
		printf("BIOSDISK() returned an error:\nERROR: %i", i_errorCode);
	}
}

char * convertByteToHexCode(char c_input)
{
	static char c_currentData[2];
	//Converting Raw data into readable hex codes
	
	c_currentData[0]= c_input & 240;
	//Masking with 11110000, returning XXXX0000
	c_currentData[0]= c_currentData[0] >> 4;
	//Shifting bits to the right, returning 0000XXXX
	c_currentData[0]= c_currentData[0] + 48;
	//Adding 48 to place the characters on the right point of the ascii table
	if(c_currentData[0] > 57)
	{
		c_currentData[0]= c_currentData[0] + 8;
		//Add 8 to skip the punctuations on the ascii table and get to A B C, etc
	}
	
	c_currentData[1]= c_input & 15;
	//Masking with 00001111 returning only 0000XXXX
	c_currentData[1]= c_currentData[1] + 48;
	//Adding 48 to place the characters on the right point of the ascii table
	if(c_currentData[1] > 57)
	{
		c_currentData[1]= c_currentData[1] + 7;
		//Add 7 to skip the punctuations on the ascii table and get to A B C, etc
	}

	return c_currentData;
}
