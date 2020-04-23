//
//  retro86.c
//  retro86
//

#include "retro86.h"
#include "cpu8086.h"

typedef enum
{
	eARGV_exePath,
	eARGV_InputFile,
	eARGV_Count

} ARGVIndexes_e;


static void print_screen( cpu_state_ptr cpu )
{
	int x, y;
	// print out the screen
	for( y = 0; y < 25; ++y )
	{
		for( x = 0; x < 80; ++x )
			fprintf( stdout, "%c", cpu->memory[ MEMOFFSET_VIDEO + ( y * 80 ) + x ] );
		fprintf( stdout, "\n" );
	}
}

static void print_cpu_state( cpu_state_ptr cpu )
{
	printf( "...............................................................\n");
	printf( "AX: %04x BX: %04x CX: %04x DX: %04x\n", cpu->AX, cpu->BX, cpu->CX, cpu->DX );
	printf( "AH: %02x   BH: %02x   CH: %02x   DH: %02x\n", HI8(cpu->AX), HI8(cpu->BX), HI8(cpu->CX), HI8(cpu->DX) );
	printf( "AL:   %02x BL:   %02x CL:   %02x DL:   %02x\n", LO8(cpu->AX), LO8(cpu->BX), LO8(cpu->CX), LO8(cpu->DX) );
	printf( "SP: %04x BP: %04x SI: %04x DI: %04x\n",  cpu->SP, cpu->BP, cpu->SI, cpu->DI );
	printf( "CS: %04x DS: %04x SS: %04x ES: %04x\n",  cpu->CS, cpu->DS, cpu->SS, cpu->ES );
	printf( "CF:%d PF:%d AF:%d ZF:%d SF:%d TF:%d IF:%d DF:%d OF:%d\n",
		cpu->flags.bit.CF, cpu->flags.bit.PF, cpu->flags.bit.AF,
		cpu->flags.bit.ZF, cpu->flags.bit.SF, cpu->flags.bit.TF,
		cpu->flags.bit.IF, cpu->flags.bit.DF, cpu->flags.bit.OF);
	printf( "IP: %04x\n", cpu->IP );
}

static uint8_t process_operation( cpu_state_ptr cpu )
{
	int i, b;
	uint8_t ret;

	printf( "===============================================================\n");
	for( i = 0; i < 6; ++i )
	{
		uint8_t c = cpu->mp[i];
		printf( "0x%02x - ", c );
		for( b = 0; b < 8; ++b )
			printf("%d", (c >> (7-b)) & 1 );
		printf("\n");
	}
	ret = cpu_execute( cpu );
	return ret;
}

static void disasm( cpu_state_ptr cpu )
{
	while( 1 )
	{
		uint8_t count = process_operation( cpu );
		uint16_t memoryOffset;

		if( UNHANDLED_OPCODE == count )
		{
			//printf("operation returned as unhandled\n");
			break;
		}

		cpu->IP += count;
		cpu->mp = &cpu->memory[ cpu->IP ];

		memoryOffset = (uint16_t)(cpu->mp - cpu->memory);
		if( memoryOffset != cpu->IP )
		{
			printf("*** Memory references do not match! ***");
		}


		print_cpu_state( cpu );
		//print_screen( cpu );
	}

	print_screen(cpu);
}

int main( int argc, char **argv )
{
	if( eARGV_Count == argc )
	{
		FILE* binFile = NULL;
		const char* inputFilename = argv[1];

		fprintf( stdout, "input file = %s\n", "/home/anonymous/programming/assembly/codegolf_8086/" );

		binFile = fopen( "sample.asm", "rb" );
		if( binFile )
		{
			size_t rd;

			size_t fileSize = 0;
			fseek( binFile, 0, SEEK_END );
			fileSize = (size_t)ftell( binFile );
			fseek( binFile, 0, SEEK_SET );

			if( fileSize <= MEMSIZE )
			{
				cpu_state_ptr cpu = NULL;

				cpu_create( &cpu );
				cpu_reset( cpu );
				//print_cpu_state( cpu );

				fprintf( stdout, "input file size = %lu\n", fileSize );

				// load the file into emulator memory
				rd = fread( cpu->memory, 1, fileSize, binFile );

				fclose( binFile );
				if( rd == fileSize )
				{
					fprintf(stdout, "Executing binary file...\n");
					disasm( cpu );
				}

				cpu_destroy( &cpu );
			}
			else
			{
				fprintf(stdout, "ERROR: input file '%s' size is > cpu RAM size (%d)", inputFilename, MEMSIZE );
			}
		}
		else
		{
			fprintf(stdout, "ERROR: Unable to open input file '%s'", inputFilename );
		}
	}
	else
	{
		fprintf(stdout, "USAGE: %s <input file path>", argv[eARGV_exePath]);
	}


	return 0;
}
