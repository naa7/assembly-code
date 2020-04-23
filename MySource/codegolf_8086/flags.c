//
//  flags.c
//  retro86
//

#include "flags.h"
#include "cpu8086.h"

static void set_psz_flags_byte( cpu_state_ptr cpu, uint8_t val )
{
	int bit_index, bit_count;

	cpu->flags.bit.ZF = ( 0 == val );
	cpu->flags.bit.SF = ( 0 != ( val & 0x80 ) );
	// @TODO: convert to a lookup table with 256 entries
	for( bit_index = 0; bit_index < 8; ++bit_index )
		bit_count = ( val >> ( 7 - bit_index ) ) & 1;
	cpu->flags.bit.PF = ( ( bit_count & 1 ) > 0 );
}

static void set_psz_flags_word( cpu_state_ptr cpu, uint16_t val )
{
	int bit_index, bit_count;
	
	cpu->flags.bit.ZF = ( 0 == ( val & 0xffff ) );
	cpu->flags.bit.SF = ( 0 != ( val & 0x8000 ) );
	// @TODO: convert to a lookup table with 256 entries
	for( bit_index = 0; bit_index < 16; ++bit_index )
		bit_count = ( val >> ( 15 - bit_index ) ) & 1;
	cpu->flags.bit.PF = ( ( bit_count & 1 ) > 0 );
}

void set_flag_add_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2 )
{
	uint16_t dst = (uint16_t)v1 + (uint16_t)v2;

	set_psz_flags_byte( cpu, v1 + v2 );
	
	cpu->flags.bit.CF = ( ( dst & 0xff00) != 0 );
	cpu->flags.bit.OF = ( ( dst ^ v1 ) & ( dst ^ v2 ) & 0x80 ) != 0;
	cpu->flags.bit.AF = ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0;
}

void set_flag_add_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2 )
{
	uint32_t dst = (uint32_t)v1 + (uint32_t)v2;

	set_psz_flags_word( cpu, v1 + v2 );

	cpu->flags.bit.CF = ( ( dst & 0xffff0000 ) != 0 );
	cpu->flags.bit.OF = ( ( ( dst ^ v1 ) & ( dst ^ v2 ) & 0x8000 ) != 0 );
	cpu->flags.bit.AF = ( ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0 );
}

void set_flag_sub_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2 )
{
	uint16_t dst = (uint16_t)v1 - (uint16_t)v2;
	
	set_psz_flags_byte(cpu, v1 - v2 );

	cpu->flags.bit.CF = ( ( dst & 0xff00) != 0 );
	cpu->flags.bit.OF = ( ( ( v1 ^ dst ) & ( v1 ^ v2 ) & 0x80 ) != 0 );
	cpu->flags.bit.AF = ( ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0 );
}

void set_flag_sub_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2 )
{
	// oszapc
	uint32_t dst = (uint32_t)v1 - (uint32_t)v2;

	set_psz_flags_word( cpu, v1 - v2 );

	cpu->flags.bit.CF = ( ( dst & 0xffff0000 ) != 0 );
	cpu->flags.bit.OF = ( ( ( v1 ^ dst ) & ( v1 ^ v2 ) & 0x8000 ) != 0 );
	cpu->flags.bit.AF = ( ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0 );
}

void set_flag_log_byte( cpu_state_ptr cpu, uint8_t v1 )
{
	set_psz_flags_byte( cpu, v1 );

	cpu->flags.bit.CF = 0;
	cpu->flags.bit.OF = 0;
}

// modify		define		un-define	values
// o..szapc 	o..sz.pc 	.....a.. 	o......c
void set_flag_log_word( cpu_state_ptr cpu, uint16_t v1 )
{
	set_psz_flags_word( cpu, v1 );
	
	cpu->flags.bit.CF = 0;
	cpu->flags.bit.OF = 0;
}

void set_flag_adc_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2, uint8_t v3 )
{
	uint16_t dst = (uint16_t)v1 + (uint16_t)v2 + (uint16_t)v3;

	set_psz_flags_byte( cpu, v1 + v2 + v3 );

	cpu->flags.bit.CF = ( ( dst & 0xff00 ) != 0 );
	cpu->flags.bit.OF = ( ( dst ^ v1 ) & ( dst ^ v2 ) & 0x80) != 0;
	cpu->flags.bit.AF = ( ( v1 ^ v2 ^ dst) & 0x10 ) != 0;
}

void set_flag_adc_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2, uint16_t v3 )
{
	uint32_t dst = (uint32_t)v1 + (uint32_t)v2 + (uint32_t)v3;
	
	set_psz_flags_word( cpu, v1 + v2 + v3 );
	
	cpu->flags.bit.CF = ( ( dst & 0xffff0000 ) != 0 );
	cpu->flags.bit.OF = ( ( dst ^ v1 ) & ( dst ^ v2 ) & 0x8000 ) != 0;
	cpu->flags.bit.AF = ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0;
}

void set_flag_sbb_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2, uint8_t v3 )
{
	uint16_t dst = (uint16_t)v1 - (uint16_t)v2 - (uint16_t)v3;;

	set_psz_flags_byte( cpu, v1 - v2 - v3 );

	cpu->flags.bit.CF = ( ( dst & 0xff00) != 0 );
	cpu->flags.bit.OF = ( ( v1 ^ dst ) & ( v1 ^ v2 ) & 0x80 ) != 0;
	cpu->flags.bit.AF = ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0;
}

void set_flag_sbb_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2, uint16_t v3 )
{
	// o..szapc 	
	uint32_t dst = (uint32_t)v1 - (uint32_t)v2 - (uint32_t)v3;

	set_psz_flags_word( cpu, v1 - v2 - v3 );

	cpu->flags.bit.CF = ( ( dst & 0xffff0000 ) != 0 );
	cpu->flags.bit.OF = ( ( v1 ^ dst ) & ( v1 ^ v2 ) & 0x8000 ) != 0;
	cpu->flags.bit.AF = ( ( v1 ^ v2 ^ dst ) & 0x10 ) != 0;
}