#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CRCVER "v1.0.0"

int verbose = 0;

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
	int number = 0;
	
	if( (ascii_hex >= 'A' && ascii_hex <= 'F') || (ascii_hex >= '0' && ascii_hex <= '9') )
	{
		if( ascii_hex >= 'A')
		{
			number = 10 + (ascii_hex - 'A');
		}
		else
		{
			number = ascii_hex - '0';
		}
	}
	else
	{
		printf("warning: ascii code is not HEX: %d  ", ascii_hex);
		number = -1;
	}
	return number;
}

int main(int argc, char *argv[])
{
	
	uint16_t _CRC16 = 0;
	
	FILE *outputfile = NULL;
	int write_file = 0;
	printf("CRC16 version: %s \n", CRCVER);

	if (5 == argc && strcmp(argv[3], "-o") == 0 )
	{
		printf("Write to output file: %s \n", argv[4]);
		outputfile = fopen(argv[4], "w");
		if(outputfile)
		{
			write_file = 1;
		}
		else
		{
			printf("ERROR: Can't create the output file \n");
		}
	}
	
	if(3 == argc || 5 == argc)
	{
		if (strcmp(argv[1], "-x") == 0 || strcmp(argv[1], "-xv") == 0) {
			
			if(strcmp(argv[1], "-xv") == 0)
				verbose = 1;
			//read hex file here and do crc
			FILE *fptr = NULL;
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
				
				int lineidx = 0;
				int linechar_idx = 0;
				int bytes_in_line = 0;
				
				char line_data[32] = {0};
				int byteidx = 0;
				int hex_nibble = 0;
				
				for(int i = 0; i < filelen ; i++)
				{
					//printf(" %d ", buffer[i]);
					
					
					if(linechar_idx == 1)
					{
						hex_nibble = asciiHexToDec(buffer[i]);
						if(hex_nibble != -1)
							bytes_in_line = hex_nibble * 16;
						else
						{
							printf("Skip line %d\n", lineidx);
							bytes_in_line = 0;
							break;
						}
					}
					else if(linechar_idx == 2)
					{
						hex_nibble = asciiHexToDec(buffer[i]);
						if(hex_nibble != -1)
						{
							bytes_in_line = bytes_in_line + asciiHexToDec(buffer[i]);
							if(verbose)
							{
								printf("<%d>", bytes_in_line);
							}
							if(write_file)
							{
								fprintf(outputfile,"<%d>", bytes_in_line);
							}
						}
						else
						{
							printf("Skip line %d\n", lineidx);
							bytes_in_line = 0;
							break;
						}
					}
					if(linechar_idx > 8 && byteidx < (bytes_in_line * 2) )
					{
						line_data[byteidx++] = buffer[i];
					}
					else if( linechar_idx == 8) // take the address
					{
						char addr[5] = {0};
						for(int k = 0; k < 4; k++)
						{
							addr[k] = buffer[ (i - 5) + k ];
						}
						addr[4] = '\0';
						if(verbose)
							printf("<%s>", addr);
						if(write_file)
							fprintf(outputfile, "<%s>", addr);
					}
					
					//end of line (in windows this is reading as 10 LF
					if(buffer[i] == 10)
					{
						lineidx++;
						uint8_t one_byte = 0;
						uint8_t nibble_count = 0;
						if(verbose)
						{
							printf("{");
						}
						if(write_file)
						{
							fprintf(outputfile, "{");
						}
						for(int j = 0; j < (bytes_in_line * 2); j++)
						{
							if(0 == nibble_count)
							{
								if(0 != j)
								{
									if(verbose)
									{
										printf(", ");
									}
									
									if(write_file)
									{
										fprintf(outputfile, ", ");
									}
								}
								one_byte = asciiHexToDec(line_data[j]) * 16;
								nibble_count++;
							}								
							else if(1 == nibble_count)
							{
								one_byte += asciiHexToDec(line_data[j]);
								if(verbose)
								{
									printf("%d", one_byte);
								}
								if(write_file)
								{
									fprintf(outputfile,"%d", one_byte);
								}
								//do the CRC here
								_CRC16 = crc16_update(_CRC16, one_byte);
								nibble_count = 0;
							}
						}
						if(verbose)
						{
							printf("}\n");
						}
						if(write_file)
						{
							fprintf(outputfile,"}\n");
						}
						linechar_idx = 0;
						byteidx = 0;
					}
					else
						linechar_idx++;
						
				}
				
				printf("CRC16 calculated: %d\n", _CRC16);
				
				if(write_file)
				{
					fprintf(outputfile, "<CRC16>{%d}\n", _CRC16);
				}
			}
			else
			{
				printf("ERROR: Can't open the file");
			}
			
			
			fclose(fptr);
			fclose(outputfile);
		}

	}
	else
	{
		printf("ERR: invalid arguments CRC16 usage: crc -x <HEX_FILE> -o <OUTPUT_FILE>");
	}
	return 0;
}