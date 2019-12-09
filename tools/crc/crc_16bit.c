#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint16_t
crc16_update(uint16_t crc, uint8_t a)
{
	int i;

	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}

	return crc;
}

int asciiHexToDec(char ascii_hex)
{
	uint8_t number = 0;
	
	if( ascii_hex >= 'A')
	{
		number = 10 + (ascii_hex - 'A');
	}
	else
	{
		number = ascii_hex - '0';
	}
	
	return number;
}

int main(int argc, char *argv[])
{

	if(3 == argc)
	{
		if (strcmp(argv[1], "-x") == 0) {
			
			//read hex file here and do crc
			FILE *fptr;
			fptr = fopen(argv[2], "r");
			if(fptr)
			{
				char *buffer;
				long filelen;
				
				fseek(fptr, 0, SEEK_END);          // Jump to the end of the file
				filelen = ftell(fptr);             // Get the current byte offset in the file
				rewind(fptr);                      // Jump back to the beginning of the file

				buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
				fread(buffer, filelen, 1, fptr); // Read in the entire file
				
				int line_idx = 0;
				int numbytes = 0;
				char line_bytes[32] = {0};
				int byteidx = 0;
				for(int i = 0; i < filelen; i++)
				{
					//printf("%c", buffer[i]);
					
					
					if(line_idx == 1)
					{
						numbytes = asciiHexToDec(buffer[i]) * 16;
					}
					else if(line_idx == 2)
					{
						numbytes = numbytes + asciiHexToDec(buffer[i]);
						printf("- %d -", numbytes);
					}
					if(line_idx > 8 && byteidx < (numbytes * 2) )
					{
						line_bytes[byteidx++] = buffer[i];
					}
					
					if(buffer[i] == 10)
					{
						uint8_t crc_byte;
						for(int j = 0; j < (numbytes * 2); j++)
						{
							printf("%c", line_bytes[j]);
						}
						printf("\n");
						line_idx = 0;
						byteidx = 0;
					}
					else
						line_idx++;
						
				}
			}
			
			
			fclose(fptr);
		}
	}
	else
	{
		printf("CRC16 usage: ");
	}
	return 0;
}