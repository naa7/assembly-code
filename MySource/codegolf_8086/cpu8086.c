//
//  cpu8086.c
//  retro86
//

#include <stdio.h>
#include "cpu8086.h"
#include "opcodes.h"

#define EAADDR(s,o) ((((s) & 0xffffUL) << 4) + ((o) & 0xffff))

uint8_t cpu_get_mem_u8( cpu_state_ptr cpu, uint16_t segment, uint16_t offset )
{
	// @TODO: change these to support segmented memory when that comes
	//uint32_t addr = EAADDR(segment, offset);
	return cpu->memory[ offset ];
}

void cpu_set_mem_u8( cpu_state_ptr cpu, uint16_t segment, uint16_t offset, uint8_t value )
{
	//uint32_t addr = EAADDR(segment, offset);
	cpu->memory[ offset ] = value;
}

uint16_t cpu_get_mem_u16( cpu_state_ptr cpu, uint16_t segment, uint16_t offset )
{
	//uint32_t addr = EAADDR(segment, offset);
	return *((uint16_t*)(&cpu->memory[offset]));
}

void cpu_set_mem_u16( cpu_state_ptr cpu, uint16_t segment, uint16_t offset, uint16_t value )
{
	//uint32_t addr = EAADDR(segment, offset);
	*((uint16_t*)&cpu->memory[ offset ]) = value;
}

void cpu_push( cpu_state_ptr cpu, uint16_t value )
{
	cpu->SP -= 2;
	cpu_set_mem_u16( cpu, cpu->SS, cpu->SP, value );
}

uint16_t cpu_pop( cpu_state_ptr cpu )
{
	uint16_t sp = cpu_get_mem_u16( cpu, cpu->SS, cpu->SP );
	cpu->SP += 2;
	return sp;
}

void cpu_reset( cpu_state_s* cpu )
{
	memset( cpu, 0, sizeof( cpu_state_s ) );

	cpu->memory = malloc( MEMSIZE );
	cpu->mp = cpu->memory;
	memset( cpu->memory, 0, MEMSIZE );

	memset( &cpu->memory[MEMOFFSET_VIDEO], ' ', 80 * 25 );

	cpu->SP = 0x100;
	cpu->flags.bit.IOPL = 0x3;
	cpu->flags.bit.NT = 1;
	cpu->flags.bit.flag15_reserved = 1;


}

uint8_t cpu_execute( cpu_state_ptr cpu )
{
	cpu->cnt = 0;
	return op_execute( cpu );
}

void cpu_create( cpu_state_hdl cpu )
{
	*cpu = malloc( sizeof( cpu_state_s ) );
	cpu_reset( *cpu );
}

void cpu_destroy( cpu_state_hdl cpu )
{
	free( (*cpu)->memory );
	free( *cpu );
	*cpu = NULL;
}


