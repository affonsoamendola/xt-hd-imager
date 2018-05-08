#include <conio.h>
#include <dos.h>
#include <bios.h>
/*Comment these three so that the code will compile on modern OS's, these should compile with Borland C++, Turbo C and the like,*/
/*try with some 90s C compiler, it should work*/

/*#include <stdio.h>                         */
/*#include <stdlib.h>                        */

enum e_BiosDisk
{
	SUCCESS 				= 	0x00,
	BADCOMMAND 				= 	0x01,
	ADDRESSMARKNOTFOUND 	= 	0x02,
	WRITEPROTECTED 			= 	0x03,
	SECTORNOTFOUND 			= 	0x04,
	RESETFAIL 				= 	0x05,
	DISKCHANGED 			= 	0x06,
	PARAMETERACTIVITYFAIL	=	0x07,
	DMAOVERRUN				=	0x08,
	ATTEMPTDMA64K			=	0x09,
	BADSECTOR				=	0x0A,
	BADTRACK				=	0x0B,
	UNSUPPORTEDTRACK		=	0x0C,
	CRCCORRECTEDDATAERROR	=	0x11,
	CONTROLLERFAIL			=	0x20,
	SEEKOPFAIL				=	0x40,
	ATTACHMENTFAIL			=	0x80,
	DRIVENOTREADY			=	0xAA,
	UNDEFINEDERROR			=	0xBB,
	WRITEFAULT				=	0xCC,
	STATUSERROR				=	0xE0,
	SENSEOPFAIL				=	0xFF
};

char * convertByteToHexCode(char c_input);
char * biosDiskError(int i_errNo);

int main()
{
	char c_buffer[512];

	int i_cmd;
	int i_disk;
	int i_head;
	int i_sector;
	int i_track;
	int i_nsec;

	int i_errorCode;
	char c_serialConfig;

	int i_useSerial;

	int i;
	int j;

	int t;
	int s;
	int h;

	char * c_hexcode;
	char c_answer;

	printf("Use serial Comms? (Y or N)");
	scanf("%c", &c_answer);

	if(c_answer == 'Y' || c_answer == 'y')
	{
		i_useSerial = 1;
	}
	else
	{
		i_useSerial = 0;
	}

	if(i_useSerial == 1)
	{
		c_serialConfig = 224|3|0|0;

		printf("Initializing serial comms....\n");
		printf("Status:%u\n",bioscom(0,c_serialConfig,0));
	}
	printf("Enter command number:\n");
	scanf("%i", &i_cmd);
	printf("Enter number of the disk to read:\n");
	scanf("%i", &i_disk);
	printf("Enter number of the heads to read:\n");
	scanf("%i", &i_head);
	printf("Enter number of the tracks to read:\n");
	scanf("%i", &i_track);
	printf("Enter number of the sectors to read:\n");
	scanf("%i", &i_sector);

	for(i=0;i<512;i++)
	{

		c_buffer[i] = 46;

	}

	if(i_cmd == 3 || i_cmd == 11 || i_cmd == 15)
	{
		printf("You're trying to write some sector(s), this might overwrite stuff and might mess up your data, only do this if you're absolutely sure of what you're doing.\n");
		printf("Do you wish to continue? (Y or N)\n");
		scanf("%c", &c_answer);
		if(c_answer != 'Y' || c_answer != 'y')
		{
			return 0;
		}
	}

	if(i_cmd == 5 || i_cmd == 6 || i_cmd == 7)
	{
		printf("You're trying to format something, this might overwrite stuff and might mess up your data, only do this if you're absolutely sure of what you're doing.\n");
		printf("Do you wish to continue? (Y or N)\n");
		scanf("%c", &c_answer);
		if(c_answer != 'Y' || c_answer != 'y')
		{
			return 0;
		}
	}


	for(h=0; h<i_head; h++)
	{
		for(t=0; t<i_track;t++)
		{
			for(s=1; s<i_sector+1;s++)
			{
				printf("\nREADING DISK %i",i_disk);
				printf(", HEAD %i",h);
				printf(", TRACK %i",t);
				printf(", SECTOR %i...\n\n",s);

				i_errorCode = biosdisk(i_cmd, i_disk, h, t, s, 1, c_buffer);
				printf("%i", i_errorCode);
				/*Comment  this line to compile in modern compilers

				//i_errorCode = 8;
				*/

				if(i_errorCode == 0)
				{
					j = 0;

					/*Send data to serial port
					*/
					for(i=0; i<512; i++)
					{
						if(i_useSerial == 1)
						{
							bioscom(1,c_buffer[i],0);
						}
					}

					/*Print contents on screen
					*/
					for(i=0;i<512; i++,j++)
					{
						if(j > 23)
						{
							/*Making pretty columns
							*/
							printf("\n");
							j=0;
						}

						c_hexcode = convertByteToHexCode(c_buffer[i]);

						printf("%c", c_hexcode[0]);
						printf("%c ", c_hexcode[1]);

						free(c_hexcode);
					}
				}
				else
				{
					printf("BIOSDISK() returned an error:\nERROR: %i\n", i_errorCode);
					printf(biosDiskError(i_errorCode));
					printf("\n");
				}
			}
		}
	}
}

char * convertByteToHexCode(char c_input)
{
	static char * c_currentData;
	/*Converting Raw data into readable hex codes

	*/
	c_currentData = malloc(2);

	c_currentData[0]= c_input & 0xf0;
	/*Masking with 11110000, returning XXXX0000
	*/
	c_currentData[0]= c_currentData[0] >> 4;
	c_currentData[0]= c_currentData[0] & 0x0f;
	/*shifting bits to the right, returning 0000XXXX
	*/
	c_currentData[0]= c_currentData[0]+48;
	/*Adding 48 to place the characters on the right point of the ascii table
	*/
	if(c_currentData[0] > 57)
	{
		c_currentData[0]= c_currentData[0]+7;
		/*Add 7 to skip the punctuations on the ascii table and get to A B C, etc
		*/
	}

	c_currentData[1]= c_input & 0x0f;
	/*Masking with 00001111 returning only 0000XXXX
	*/
	c_currentData[1]= c_currentData[1] + 48;
	/*Adding 48 to place the characters on the right point of the ascii table
	*/
	if(c_currentData[1] > 57)
	{
		c_currentData[1]= c_currentData[1] + 7;
		/*Add 7 to skip the punctuations on the ascii table and get to A B C, etc
		*/
	}

	return c_currentData;
}

char * biosDiskError(int i_errNo)
{
	switch(i_errNo)
	{
		case(0x00):
			return "Successful completion";
		case(0x01):
			return "Bad command";
		case(0x02):
			return "Address mark not found";
		case(0x03):
			return "Attempt to write to write-protected disk";
		case(0x04):
			return "Sector not found";
		case(0x05):
			return "Reset failed (hard disk)";
		case(0x06):
			return "Disk changed since last operation";
		case(0x07):
			return "Drive parameter activity failed";
		case(0x08):
			return "Direct memory access (DMA) overrun";
		case(0x09):
			return "Attempt to perform DMA across 64K";
		case(0x0A):
			return "Bad sector detected";
		case(0x0B):
			return "Bad track detected";
		case(0x0C):
			return "Usupported track";
		case(0x11):
			return "CRC/ECC corrected data error";
		case(0x20):
			return "Controller failure";
		case(0x40):
			return "Seek operation failed";
		case(0x80):
			return "Attachment failed to respond";
		case(0xAA):
			return "Drive not ready (hard disk)";
		case(0xBB):
			return "Undefined errror occurred (hard disk)";
		case(0xCC):
			return "Write fault occurred";
		case(0xE0):
			return "Status error";
		case(0xFF):
			return "Sense operation failed";
		default:
			return "Unknown Error";
	}
}

