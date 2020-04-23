//
//  opcodes.c
//  retro86
//

#include <stdio.h>
#include "opcodes.h"
#include "cpu8086.h"
#include "flags.h"

typedef uint8_t (*op86_fn) ( cpu_state_ptr cpu);

const char* wordRegName[] = {"AX","CX","DX","BX","SP","BP","SI","DI"};
const char* byteRegName[] = {"AL","CL","DL","BL","AH","CH","DH","BH"};

// forward global variable declarations
op86_fn op86_opcodes[256];

// forward function declarations
uint8_t bit_count( uint8_t byte );

static void set_reg_val_16( cpu_state_ptr cpu, uint8_t reg, uint16_t val )
{
	// @TODO: convert this to a direct lookup table with enum register indexes
	switch( reg )
	{
	case REG_AX: cpu->AX = val; break;
	case REG_CX: cpu->CX = val; break;
	case REG_DX: cpu->DX = val; break;
	case REG_BX: cpu->BX = val; break;
	case REG_SP: cpu->SP = val; break;
	case REG_BP: cpu->BP = val; break;
	case REG_SI: cpu->SI = val; break;
	case REG_DI: cpu->DI = val; break;
	}
}

static uint16_t get_reg_val_16( cpu_state_ptr cpu, uint8_t reg )
{
	uint16_t val = 0;
	// @TODO: convert this to a direct lookup table with enum register indexes
	switch( reg )
	{
	case REG_AX: val = cpu->AX; break;
	case REG_CX: val = cpu->CX; break;
	case REG_DX: val = cpu->DX; break;
	case REG_BX: val = cpu->BX; break;
	case REG_SP: val = cpu->SP; break;
	case REG_BP: val = cpu->BP; break;
	case REG_SI: val = cpu->SI; break;
	case REG_DI: val = cpu->DI; break;
	}

	return val;
}

static void set_reg_val_8( cpu_state_ptr cpu, uint8_t reg, uint8_t val )
{
	// @TODO: convert this to a direct lookup table with enum register indexes
	switch( reg )
	{
	case REG_AL: cpu->AX = (uint16_t) val + ( cpu->AX & 0xff00 ); break;
	case REG_CL: cpu->CX = (uint16_t) val + ( cpu->CX & 0xff00 ); break;
	case REG_DL: cpu->DX = (uint16_t) val + ( cpu->DX & 0xff00 ); break;
	case REG_BL: cpu->BX = (uint16_t) val + ( cpu->BX & 0xff00 ); break;
	case REG_AH: cpu->AX = (uint16_t)( ( (uint16_t)val << 8 ) + ( cpu->AX & 0xff ) ); break;
	case REG_CH: cpu->CX = (uint16_t)( ( (uint16_t)val << 8 ) + ( cpu->CX & 0xff ) ); break;
	case REG_DH: cpu->DX = (uint16_t)( ( (uint16_t)val << 8 ) + ( cpu->DX & 0xff ) ); break;
	case REG_BH: cpu->BX = (uint16_t)( ( (uint16_t)val << 8 ) + ( cpu->BX & 0xff ) ); break;
	}
}

static uint8_t get_reg_val_8( cpu_state_ptr cpu, uint8_t reg )
{
	uint8_t val = 0;
	// @TODO: convert this to a direct lookup table with enum register indexes
	switch( reg )
	{
	case REG_AL: val = cpu->AX & 0xff; break;
	case REG_CL: val = cpu->CX & 0xff; break;
	case REG_DL: val = cpu->DX & 0xff; break;
	case REG_BL: val = cpu->BX & 0xff; break;
	case REG_AH: val = cpu->AX >> 8; break;
	case REG_CH: val = cpu->CX >> 8; break;
	case REG_DH: val = cpu->DX >> 8; break;
	case REG_BH: val = cpu->BX >> 8; break;
	}

	return val;
}

static uint8_t opxx( cpu_state_ptr cpu )
{
	printf("Unhandled opcode: 0x%02x\n", cpu->mp[0]);
	//exit(-1);
	return UNHANDLED_OPCODE;
}

static uint8_t get_ea_val_8( cpu_state_ptr cpu )
{
	uint16_t disp16 = MAKE16( cpu->mp[3], cpu->mp[2] );
	uint8_t disp8 = cpu->mp[2];
	
	switch( cpu->modrm.bit.mod )
	{
		case 0x0:
		case 0x1:
		case 0x2:
		{
			uint16_t addr = 0;
			switch( cpu->modrm.bit.rm )
			{
				case 0:
					addr += cpu->BX;
					addr += cpu->SI;
					break;
				case 1:
					addr += cpu->BX;
					addr += cpu->DI;
					break;
				case 2:
					addr += cpu->BP;
					addr += cpu->SI;
					break;
				case 3:
					addr += cpu->BP;
					addr += cpu->DI;
					break;
				case 4:
					addr += cpu->SI;
					break;
				case 5:
					addr += cpu->DI;
					break;
				case 6:
					if( 0x0 == cpu->modrm.bit.mod )
					{
						addr += disp16;
						cpu->cnt |= BCNT_2 | BCNT_3;
					}
					else
					{
						addr += cpu->BP;
					}
					break;
				case 7:
					addr += cpu->BX;
					break;
			}
			
			if( cpu->modrm.bit.mod == 0x1 )
			{
				addr += disp8;
				cpu->cnt |= BCNT_2;
			}
			else if( cpu->modrm.bit.mod == 0x2 )
			{
				addr += disp16;
				cpu->cnt |= BCNT_2 | BCNT_3;
			}
			
			return cpu->memory[ addr ];
		}
			break;
			
		case 0x3:
		{
			return get_reg_val_8( cpu, cpu->modrm.bit.rm );
		}
			break;
	}
		
	return 0;
}

static void set_ea_val_8( cpu_state_ptr cpu, uint8_t val )
{
	uint16_t disp16 = MAKE16( cpu->mp[3], cpu->mp[2] );
	uint8_t disp8 = cpu->mp[2];
	
	switch( cpu->modrm.bit.mod )
	{
		case 0x0:
		case 0x1:
		case 0x2:
		{
			uint16_t addr = 0;
			switch( cpu->modrm.bit.rm )
			{
				case 0:
					addr += cpu->BX;
					addr += cpu->SI;
					break;
				case 1:
					addr += cpu->BX;
					addr += cpu->DI;
					break;
				case 2:
					addr += cpu->BP;
					addr += cpu->SI;
					break;
				case 3:
					addr += cpu->BP;
					addr += cpu->DI;
					break;
				case 4:
					addr += cpu->SI;
					break;
				case 5:
					addr += cpu->DI;
					break;
				case 6:
					if( 0x0 == cpu->modrm.bit.mod )
					{
						addr += disp16;
						cpu->cnt |= BCNT_2 | BCNT_3;
					}
					else
					{
						addr += cpu->BP;
					}
					break;
				case 7:
					addr += cpu->BX;
					break;
			}
			
			if( cpu->modrm.bit.mod == 0x1 )
			{
				addr += disp8;
				cpu->cnt |= BCNT_2;
			}
			else if( cpu->modrm.bit.mod == 0x2 )
			{
				addr += disp16;
				cpu->cnt |= BCNT_2 | BCNT_3;
			}
			
			cpu->memory[ addr ] = val;
		}
			break;
			
		case 0x3:
		{
			set_reg_val_8( cpu, cpu->modrm.bit.rm, val );
		}
			break;
	}
	
	return;
}

static uint16_t get_ea_val_16( cpu_state_ptr cpu )
{
	uint16_t disp16 = MAKE16( cpu->mp[3], cpu->mp[2] );
	uint8_t disp8 = cpu->mp[2];

	switch( cpu->modrm.bit.mod )
	{
		case 0x0:
		case 0x1:
		case 0x2:
		{
			uint16_t addr = 0;
			switch( cpu->modrm.bit.rm )
			{
				case 0:
					addr += cpu->BX;
					addr += cpu->SI;
					break;
				case 1:
					addr += cpu->BX;
					addr += cpu->DI;
					break;
				case 2:
					addr += cpu->BP;
					addr += cpu->SI;
					break;
				case 3:
					addr += cpu->BP;
					addr += cpu->DI;
					break;
				case 4:
					addr += cpu->SI;
					break;
				case 5:
					addr += cpu->DI;
					break;
				case 6:
					if( 0x0 == cpu->modrm.bit.mod )
					{
						addr += disp16;
						cpu->cnt |= BCNT_2 | BCNT_3;
					}
					else
					{
						addr += cpu->BP;
					}
					break;
				case 7:
					addr += cpu->BX;
					break;
			}

			if( cpu->modrm.bit.mod == 0x1 )
			{
				addr += disp8;
				cpu->cnt |= BCNT_2;
			}
			else if( cpu->modrm.bit.mod == 0x2 )
			{
				addr += disp16;
				cpu->cnt |= BCNT_2 | BCNT_3;
			}

			return MAKE16( cpu->memory[ addr ], cpu->memory[ addr + 1 ] );
		}
		break;

		case 0x3:
		{
			return get_reg_val_16( cpu, cpu->modrm.bit.rm );
		}
		break;
	}

	return 0;
}

static void set_ea_val_16( cpu_state_ptr cpu, uint16_t val )
{
	uint16_t disp16 = MAKE16( cpu->mp[3], cpu->mp[2] );
	uint8_t disp8 = cpu->mp[2];

	switch( cpu->modrm.bit.mod )
	{
		case 0x0:
		case 0x1:
		case 0x2:
		{
			uint16_t addr = 0;
			switch( cpu->modrm.bit.rm )
			{
				case 0:
					addr += cpu->BX;
					addr += cpu->SI;
					break;
				case 1:
					addr += cpu->BX;
					addr += cpu->DI;
					break;
				case 2:
					addr += cpu->BP;
					addr += cpu->SI;
					break;
				case 3:
					addr += cpu->BP;
					addr += cpu->DI;
					break;
				case 4:
					addr += cpu->SI;
					break;
				case 5:
					addr += cpu->DI;
					break;
				case 6:
					if( 0x0 == cpu->modrm.bit.mod )
					{
						addr += disp16;
						cpu->cnt |= BCNT_2 | BCNT_3;
					}
					else
					{
						addr += cpu->BP;
					}
					break;
				case 7:
					addr += cpu->BX;
					break;
			}
			
			if( cpu->modrm.bit.mod == 0x1 )
			{
				addr += disp8;
				cpu->cnt |= BCNT_2;
			}
			else if( cpu->modrm.bit.mod == 0x2 )
			{
				addr += disp16;
				cpu->cnt |= BCNT_2 | BCNT_3;
			}
			
			cpu->memory[ addr ] = HI8( val );
			cpu->memory[ addr+1 ] = LO8( val );
			
		}
			break;
			
		case 0x3:
		{
			set_reg_val_16( cpu, cpu->modrm.bit.rm, val );
		}
			break;
	}
	
	return;
}

// ADD rm16, reg16
static uint8_t op01( cpu_state_ptr cpu )
{
	uint16_t src1, src2;
	cpu->cnt |= BCNT_1; // modrm
	printf("ADD rm16, reg16\n");
	src1 = get_ea_val_16( cpu );
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	set_ea_val_16( cpu, src1 + src2 );
	set_flag_add_word( cpu, src1, src2 );
	return cpu->cnt;
}

// ADD 	AL, data8
static uint8_t op04( cpu_state_ptr cpu )
{
	uint8_t val = cpu->mp[1];
	uint8_t AL = get_reg_val_8( cpu, REG_AL );
	cpu->cnt |= BCNT_1;
	printf( "ADD AL, 0x%02x\n", val );
	set_reg_val_8( cpu, REG_AL, AL + val );
	set_flag_add_byte( cpu, AL, val);
	return cpu->cnt;
}

// OR rm16, reg16
static uint8_t op09( cpu_state_ptr cpu )
{
	uint16_t src1, src2, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16( cpu );
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	dst = src1 | src2;
	set_ea_val_16(cpu, dst);
	set_flag_log_word(cpu, dst);
	printf( "OR %s, %s\n", wordRegName[ cpu->modrm.bit.reg ], wordRegName[ cpu->modrm.bit.rm ] );
	return cpu->cnt;
}

// OR reg8, rm8
static uint8_t op0a( cpu_state_ptr cpu )
{
	uint8_t src1, src2, dst;
	cpu->cnt |= BCNT_1;
	src1 = get_reg_val_8(cpu, cpu->modrm.bit.reg);
	src2 = get_ea_val_8(cpu);
	dst = src1 | src2;
	set_ea_val_8(cpu, dst);
	set_flag_log_byte(cpu, dst);
	printf( "OR %s, %s\n", byteRegName[ cpu->modrm.bit.rm ], byteRegName[ cpu->modrm.bit.reg ] );
	return cpu->cnt;
}

// SBB rm16, reg16
static uint8_t op19( cpu_state_ptr cpu )
{
	uint16_t src1, src2, src3, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16( cpu );
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	src3 = cpu->flags.bit.CF;
	dst = src1 - src2 - src3;
	set_ea_val_16( cpu, dst );
	set_flag_sbb_word( cpu, src1, src2, src3 );
	printf( "SBB rm16, reg16\n");
	return cpu->cnt;
}

// AND 	rm8, reg8
static uint8_t op20( cpu_state_ptr cpu )
{
	uint8_t src1, src2, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_8(cpu);
	src2 = get_reg_val_8(cpu, cpu->modrm.bit.reg);
	dst = src1 & src2;
	set_ea_val_8(cpu, dst);
	set_flag_log_byte( cpu, dst );
	printf("AND %s, %s\n", byteRegName[ cpu->modrm.bit.reg ], "???" );
	return cpu->cnt;
}

// AND rm16, reg16
static uint8_t op21( cpu_state_ptr cpu )
{
	uint16_t src1, src2, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16(cpu);
	src2 = get_reg_val_16(cpu, cpu->modrm.bit.reg);
	dst = src1 & src2;
	set_ea_val_16(cpu, dst);
	set_flag_log_word(cpu, dst );
	printf("AND %s, %s\n", wordRegName[ cpu->modrm.bit.reg ], "???" );
	return cpu->cnt;
}

// SUB 	rm16, reg16
static uint8_t op29( cpu_state_ptr cpu )
{
	uint16_t src1, src2, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16(cpu);
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	dst = src1 - src2;
	set_ea_val_16( cpu, dst );
	set_flag_sub_word( cpu, src1, src2 );
	printf( "SUB %s %s\n", wordRegName[ cpu->modrm.bit.reg ], "???" );
	return cpu->cnt;
}

// SUB 	AL, data8
static uint8_t op2c( cpu_state_ptr cpu )
{
	uint8_t val = cpu->mp[1];
	uint8_t AL = get_reg_val_8( cpu, REG_AL );
	cpu->cnt |= BCNT_1; // src2
	set_reg_val_8(cpu, REG_AL, AL - val);
	set_flag_sub_byte(cpu, AL, val);
	printf( "ADD AL, 0x%02x\n", val );
	return cpu->cnt;
}

// XOR rm16, reg16
static uint8_t op31( cpu_state_ptr cpu )
{
	uint16_t src1, src2, dst;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16( cpu );
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	dst = src1 ^ src2;
	set_ea_val_16(cpu, dst);
	set_flag_log_word(cpu, dst);
	printf( "XOR %s, %s\n", wordRegName[ cpu->modrm.bit.rm ], wordRegName[ cpu->modrm.bit.reg ] );
	return cpu->cnt;
}

// CMP 	rm16, reg16
static uint8_t op39( cpu_state_ptr cpu )
{	
	uint16_t src1, src2;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16( cpu );
	src2 = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	set_flag_sub_word( cpu, src1, src2);
	printf( "CMP %s, %s\n", wordRegName[ cpu->modrm.bit.rm ], wordRegName[ cpu->modrm.bit.reg ] );
	return cpu->cnt;
}

// CMP 	AL 	Ib
static uint8_t op3c( cpu_state_ptr cpu )
{
	uint8_t src1, src2;
	src1 = get_reg_val_8(cpu, REG_AL);
	src2 = cpu->mp[1];
	cpu->cnt |= BCNT_1; // src2
	set_flag_sub_byte(cpu, src1, src2);
	return cpu->cnt;
}

// INC 	reg16
static uint8_t op40( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM(cpu->mp[0]);
	uint16_t src = get_reg_val_16(cpu, rm);
	set_reg_val_16( cpu, rm, src + 1 );
	// we can't modify the CF flag
	int cf = cpu->flags.bit.CF;
	set_flag_add_word( cpu, src, 1 );
	cpu->flags.bit.CF = ( cf != 0 );
	printf( "INC %s\n", wordRegName[rm]);
	return cpu->cnt;
}

// DEC 	reg16
static uint8_t op48( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM(cpu->mp[0]);
	uint16_t src = get_reg_val_16(cpu, rm);
	set_reg_val_16( cpu, rm, src - 1 );
	// we can't modify the CF flag
	int cf = cpu->flags.bit.CF;
	set_flag_sub_word( cpu, src, 1 );
	cpu->flags.bit.CF = ( cf != 0 );
	printf( "DEC %s\n", wordRegName[rm]);
	return cpu->cnt;
}

// PUSH
static uint8_t op50( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM(cpu->mp[0]);
	printf( "PUSH %s\n", wordRegName[rm] );	
	if( rm == REG_SP /* && CPU_PUSH_FIRST?? */ )
	{
		cpu_push( cpu, cpu->SP - 2 );
	}
	else
	{
		uint16_t value = get_reg_val_16( cpu, rm );
		cpu_push( cpu, value );
	}
	return cpu->cnt;
}

// POP 
static uint8_t op58( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM(cpu->mp[0]);
	uint16_t val = cpu_pop( cpu );
	set_reg_val_16( cpu, rm, val );
	printf( "POP %s\n", wordRegName[rm] );	
	return cpu->cnt;
}

// JB/JNAE/JC imm8
static uint8_t op72( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1;
	printf("JC %xh\n", mem);
	if( cpu->flags.bit.CF )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump
}

// JZ/JE imm8
static uint8_t op74( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1; // modrm
	printf("JZ %xh\n", mem);
	if( cpu->flags.bit.ZF )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump
}

// JNZ/JNE imm8
static uint8_t op75( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1;
	printf("JNZ %xh\n", mem);
	if( !cpu->flags.bit.ZF )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump	
}

// JBE/JNA imm8
static uint8_t op76( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1;
	printf("JBE 0x%02x\n", mem);
	if( cpu->flags.bit.CF || cpu->flags.bit.ZF )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump
}

// JA imm8
static uint8_t op77( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1;
	printf("JA 0x%02x\n", mem);
	if( !( cpu->flags.bit.CF || cpu->flags.bit.ZF ) )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump
}

// JNS imm8
static uint8_t op79( cpu_state_ptr cpu )
{
	uint16_t mem = ADD_S8( cpu->IP + 2, cpu->mp[1] );
	cpu->cnt |= BCNT_1;
	printf("JNS 0x%02x\n", mem);
	if( !cpu->flags.bit.SF )
	{
		cpu->IP = mem;
		cpu->mp = &cpu->memory[ mem ];
		return 0; // jump
	}
	return cpu->cnt; // no jump
}

// Various rm8, imm8
static uint8_t op80( cpu_state_ptr cpu )
{
	uint8_t src1, src2, dst, offset;
	cpu->cnt |= BCNT_1;
	switch( cpu->modrm.bit.reg )
	{
		case 0x1: // OR rm8, imm8
			src1 = get_ea_val_8(cpu);
			offset = bit_count( cpu->cnt );
			src2 = cpu->mp[offset];
			cpu->cnt |= (1<<(offset));
			dst = src1 | src2;
			set_ea_val_8( cpu, dst);
			set_flag_log_byte( cpu, dst );
			printf("ORB(80) %xh %xh\n", src1, src2);
		break;
		
		default:
			printf("reg = %x\n", cpu->modrm.bit.reg );
			return UNHANDLED_OPCODE;
		break;
	}
	return cpu->cnt;
}

// Various rm16, imm16
static uint8_t op81( cpu_state_ptr cpu )
{
	uint16_t src1, src2, offset;
	cpu->cnt |= BCNT_1;
	src1 = get_ea_val_16( cpu );
	offset = bit_count( cpu->cnt );
	src2 = MAKE16( cpu->mp[offset+1], cpu->mp[offset] );;
	cpu->cnt |= (1<<(offset)) | (1<<(offset+1));
	switch( cpu->modrm.bit.reg )
	{
		case 0x7: // CMP rm16, imm16
			printf("CMP(81) (%xh) %xh\n", src1, src2);
			set_flag_sub_word( cpu, src1, src2 );
		break;
		
		default:
			printf("reg = %x\n", cpu->modrm.bit.reg );
			return UNHANDLED_OPCODE;
		break;
	}
	return cpu->cnt;
}

// Various rm16, imm8
static uint8_t op83( cpu_state_ptr cpu )
{
	uint16_t src1, src2, dst, offset;
	cpu->cnt |= BCNT_1; // modrm
	src1 = get_ea_val_16(cpu);
	offset = bit_count( cpu->cnt );
	src2 = MAKE_S16( cpu->mp[offset] );
	cpu->cnt |= (1<<(offset));
	
	switch( cpu->modrm.bit.reg ) 
	{
		case 0x0: // ADD rm16, imm8
			printf("ADD %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			dst = src1 + src2;
			set_ea_val_16(cpu, dst);
			set_flag_add_word(cpu, src1, src2);
			break;
			
		case 0x1: // OR rm16, imm8
			return UNHANDLED_OPCODE;
			break;
			
		case 0x2: // ADC rm16, imm8
			printf("ADC %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			dst = src1 +  src2 + cpu->flags.bit.CF;
			set_ea_val_16( cpu, dst );
			set_flag_adc_word( cpu, src1, src2, cpu->flags.bit.CF );
			break;
			
		case 0x3: // SBB rm16, imm8
			return UNHANDLED_OPCODE;
			break;
			
		case 0x4: // AND rm16, imm8 
			printf("AND %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			dst = src1 & src2;
			set_ea_val_16( cpu, dst );
			set_flag_log_word(cpu, dst);
			break;
			
		case 0x5: // SUB rm16, imm8
			printf("SUB %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			dst = src1 - src2;
			set_ea_val_16( cpu, dst );
			set_flag_sub_word(cpu, src1, src2);
			break;
			
		case 0x6: // XOR /m16, imm8
			printf("XOR %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			dst = src1 ^ src2;
			set_ea_val_16( cpu, dst );
			set_flag_log_word(cpu, dst);
			break;
			
		case 0x7: // CMP rm16, imm8
			printf("CMP %s(0x%04x) %xh\n", wordRegName[cpu->modrm.bit.rm], src1, src2);
			set_flag_sub_word( cpu, src1, src2 );
			break;
	}
	return cpu->cnt;
}

// XCHG  rm8, reg8
static uint8_t op86( cpu_state_ptr cpu )
{
	uint8_t src1 = get_ea_val_8(cpu);
	uint8_t src2 = get_reg_val_8(cpu, cpu->modrm.bit.reg);
	cpu->cnt |= BCNT_1; // modrm
	set_ea_val_8( cpu, src2);
	set_reg_val_8(cpu, cpu->modrm.bit.reg, src1);
	printf( "XCHG %s, %s\n", byteRegName[ cpu->modrm.bit.reg ], "???" );
	return cpu->cnt;
}

// XCHG  rm16, reg16
static uint8_t op87( cpu_state_ptr cpu )
{
	uint16_t src1 = get_ea_val_16(cpu);
	uint16_t src2 = get_reg_val_16(cpu, cpu->modrm.bit.reg);
	cpu->cnt |= BCNT_1; // modrm
	set_ea_val_16(cpu, src2);
	set_reg_val_16(cpu, cpu->modrm.bit.reg, src1);
	printf( "XCHG %s, %s\n", wordRegName[ cpu->modrm.bit.reg ], "???" );
	return cpu->cnt;
}

// MOV rm8, reg8
static uint8_t op88( cpu_state_ptr cpu )
{
	uint8_t val = get_reg_val_8( cpu, cpu->modrm.bit.reg );
	cpu->cnt |= BCNT_1; // modrm
	printf("MOV [BX+DI](%xh) %s(0x%x)\n", cpu->BX + cpu->DI, byteRegName[ cpu->modrm.bit.reg ], val);
	set_ea_val_8(cpu, val);
	return cpu->cnt;
}

// MOV rm16, reg16
static uint8_t op89( cpu_state_ptr cpu )
{
	uint16_t val = get_reg_val_16( cpu, cpu->modrm.bit.reg );
	uint16_t addr = MAKE16( cpu->mp[3], cpu->mp[2] );
	cpu->cnt |= BCNT_1;
	printf("MOV [%xh] %s(0x%x)\n", addr, wordRegName[ cpu->modrm.bit.reg ], val);
	set_ea_val_16( cpu, val );
	return cpu->cnt;
}

// MOV reg8, rm8
static uint8_t op8a( cpu_state_ptr cpu )
{
	uint8_t val = get_ea_val_8(cpu);
	cpu->cnt |= BCNT_1; // modrm
	set_reg_val_8(cpu, cpu->modrm.bit.reg, val);
	printf("MOV %s 0x%02x \n", byteRegName[ cpu->modrm.bit.reg ], val );
	return cpu->cnt;
}

// MOV reg16, rm16
static uint8_t op8b( cpu_state_ptr cpu )
{
	uint16_t val = get_ea_val_16(cpu);
	cpu->cnt |= BCNT_1;
	set_reg_val_16(cpu, cpu->modrm.bit.reg, val );
	printf("MOV %s 0x%04x \n", wordRegName[ cpu->modrm.bit.reg ], val );
	return cpu->cnt;
}

// XCHG AX, reg16
static uint8_t op90( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM(cpu->mp[0]);
	uint16_t val = cpu->AX;
	val = cpu->AX;
	cpu->AX = get_reg_val_16( cpu, rm );
	set_reg_val_16( cpu, rm, val );
	printf( "NOP/XCHG AX, %s\n", wordRegName[ rm ] );
	return cpu->cnt;
}


// MOV reg8, imm8
static uint8_t opb0( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM( cpu->mp[0] );
	uint8_t val = cpu->mp[1];
	cpu->cnt |= BCNT_1;
	set_reg_val_8(cpu, rm, val);
	printf("MOV %s, $%x\n", byteRegName[rm], val);
	return cpu->cnt;
}


// MOV reg16, imm16
static uint8_t opb8( cpu_state_ptr cpu )
{
	uint8_t rm = MODRM_RM( cpu->mp[0] );
	uint16_t val =  MAKE16( cpu->mp[2], cpu->mp[1] );
	cpu->cnt |= BCNT_1 | BCNT_2;
	set_reg_val_16(cpu, rm, val);
	printf("MOV %s, $%x\n", wordRegName[rm], val);
	return cpu->cnt;
}

// RETN
static uint8_t opc3( cpu_state_ptr cpu )
{
	uint16_t addr = cpu_pop( cpu );
	printf( "RETN\n");
	cpu->IP = addr;
	cpu->mp = &cpu->memory[ addr ];
	return 0;
}

// LES reg16, rm16
static uint8_t opc4( cpu_state_ptr cpu )
{
	uint16_t val1, val2;
	cpu->cnt |= BCNT_1 | BCNT_2 | BCNT_3 | BCNT_4 | BCNT_5;
	val1 = MAKE16( cpu->mp[3], cpu->mp[2] );
	val2 = MAKE16( cpu->mp[5], cpu->mp[4] );
	set_reg_val_16(cpu, cpu->modrm.bit.reg, val1);
	cpu->ES = val2;
	return cpu->cnt;
}

// MOV(W) rm16, data16
static uint8_t opc7( cpu_state_ptr cpu )
{
	// @TODO: set the offsets based on modrm displacements instead of hard coding the 5,4 indexes here
	uint16_t val = MAKE16( cpu->mp[5], cpu->mp[4] );
	cpu->cnt |= BCNT_1 | BCNT_4 | BCNT_5 ;
	set_ea_val_16(cpu, val);
	return cpu->cnt;
}

// various Ev CL
static uint8_t opd3( cpu_state_ptr cpu )
{
	int32_t src, dst;
	uint16_t cl = get_reg_val_8(cpu, REG_CL );
	cpu->cnt |= BCNT_1; // modrm
	src = get_ea_val_16(cpu);
	//cl &= 0x1f;
	if( cl == 0x0 )
		return cpu->cnt;
	switch( cpu->modrm.bit.reg )
	{
		case 0x0: // ROL
			dst = ( src << ( cl & 15 ) ) | ( src >> ( 16 - ( cl & 15 ) ) );
			cpu->flags.bit.CF  = ( ( dst & 1 ) != 0 );
			break;
		case 0x1: // ROR
			return UNHANDLED_OPCODE;
			break;
		case 0x2: // RCL
			return UNHANDLED_OPCODE;
			break;
		case 0x3: // RCR
			return UNHANDLED_OPCODE;
			break;
		case 0x4: // SHL
		case 0x6: // alias
			return UNHANDLED_OPCODE;
			break;
		case 0x5: // SHR
			return UNHANDLED_OPCODE;
			break;
		case 0x7: // SAR
			return UNHANDLED_OPCODE;
			break;
	}
	cpu->flags.bit.OF = ( ( ( dst ^ src) & 0x8000 ) != 0 );
	set_ea_val_16(cpu, (uint16_t)dst);
	return cpu->cnt;
}

// LOOPNZ//LOOPNE imm8
static uint8_t ope0( cpu_state_ptr cpu )
{
	cpu->cnt |= BCNT_1;
	cpu->CX -= 1;
	if( cpu->CX != 0 && !cpu->flags.bit.ZF )
	{
		cpu->IP = ADD_S8(cpu->IP, cpu->mp[1] + 2);
		cpu->mp = &cpu->memory[ cpu->IP ];
		return 0;
	}
	return cpu->cnt;
}

// CALL imm16
static uint8_t ope8( cpu_state_ptr cpu )
{
	uint16_t mem =  MAKE16( cpu->mp[2], cpu->mp[1] );
	cpu->cnt |= BCNT_1 | BCNT_2;
	cpu_push( cpu, cpu->IP + 3 );
	cpu->IP += 3 + mem;
	cpu->mp = &cpu->memory[ cpu->IP ];
	printf( "CALL %xh\n", cpu->IP  );
	// push the current address(well the next location to execute when the stack is popped) to the stack
	// then set the IP to the new location in memory at imm16
	return 0;
}

// JMP imm8
static uint8_t opeb( cpu_state_ptr cpu )
{
	cpu->IP = ADD_S8( cpu->IP, cpu->mp[1] + 2 );
	cpu->mp = &cpu->memory[ cpu->IP ];
	cpu->cnt |= BCNT_1;
	printf( "JMP %xh\n", cpu->IP );
	return 0;
}

// HALT
static uint8_t opf4( cpu_state_ptr cpu )
{
	printf( "HALT!\n");
	return UNHANDLED_OPCODE; 
}
#if 0
static uint8_t opf7( cpu_state_ptr cpu )
{
	uint16_t src1, src2;
	cpu->cnt |= BCNT_1; // modrm
	//@TODO: once support for segmented memory is in, this will need to be reintegrated
	//src1 = ( (uint32_t)cpu->DX << 16 ) | cpu->AX;
	src1 = cpu->AX;
	src2 = get_ea_val_16( cpu );
	switch( cpu->modrm.bit.reg )
	{
		case 0x0:
			break;
		case 0x1:
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			break;
		case 0x7: // IDIV rm16
		{
			int16_t sign1, sign2, dst1, dst2;
			printf("IDIV rm16\n");
			// @TODO: same thing as above (segmentation)
			//src2 = ( src2 & 0x8000 ) ? ( src2 | 0xffff0000 ) :  src2;
			if( src2 == 0 )
			{
				printf("DIVIDE ERROR\n");
			}
			sign1 = ( ( src1 & 0x80000000 ) != 0 );
			sign2 = ( ( src2 & 0x80000000 ) != 0 );
			dst1 = (int16_t)src1 / (int16_t)src2;
			dst2 = (int16_t)src1 % (int16_t)src2;
			if( sign1 != sign2 )
			{
				if( dst1 > 0x8000 )
				{
					printf("DIVIDE ERROR\n");
				}
				dst1 = (int16_t)(( ~dst1 + 1 ) & 0xffff);
			}
			else
			{
				if( dst1 > 0x7fff )
				{
					printf("DIVIDE ERROR\n");					
				}
				if( sign1 )
				{
					dst2 = (int16_t)(( ~dst2 + 1 ) & 0xffff);
				}
			}
			cpu->AX = (uint16_t)dst1;
			cpu->DX = (uint16_t)dst2;
			return cpu->cnt;
		}	
			break;
	}
	return UNHANDLED_OPCODE;
}
#endif

// CLC
static uint8_t opf8( cpu_state_ptr cpu )
{
	cpu->flags.bit.CF = 0;
	return cpu->cnt;
}

// STC
static uint8_t opf9( cpu_state_ptr cpu )
{
	cpu->flags.bit.CF = 1;
	return cpu->cnt;
}

// INC/DEC 	rm8
static uint8_t opfe( cpu_state_ptr cpu )
{
	uint8_t src, cf;
	cpu->cnt |= BCNT_1; // modrm
	src = get_ea_val_8(cpu);
	switch( cpu->modrm.bit.reg )
	{
		case 0x0: // INC rm8
			set_ea_val_8(cpu, src + 1);
			// don't modify CF 
			cf = cpu->flags.bit.CF;
			set_flag_add_byte(cpu, src, 1);
			cpu->flags.bit.CF = ( cf != 0 );
			break;
			
		case 0x1: // DEC rm8
			set_ea_val_8(cpu, src - 1);
			// don't modify CF 
			cf = cpu->flags.bit.CF;
			set_flag_sub_byte(cpu, src, 1);
			cpu->flags.bit.CF = ( cf != 0 );		
			break;
			
		default:
			return UNHANDLED_OPCODE;
			break;
	}
	return cpu->cnt;
}

uint8_t bit_count( uint8_t byte )
{
	uint8_t bit_index, count = 0;
	for( bit_index = 0; bit_index < 8; ++bit_index )
		count += ( byte >> ( 7 - bit_index ) ) & 1;
	return count;
}

uint8_t op_execute( cpu_state_ptr cpu )
{
	uint8_t bit_index, bit_counter;
	uint8_t opcode = cpu->mp[0];
	uint8_t skip = 0;
	uint16_t preIP = cpu->IP;
	cpu->cnt = BCNT_OPCODE; // start off the count with the one byte opcode
	cpu->modrm.val = cpu->mp[1];
	bit_counter = op86_opcodes[ opcode ]( cpu );
	if( bit_counter == UNHANDLED_OPCODE )
		return UNHANDLED_OPCODE;
	// @TODO: convert to a lookup table with 256 entries
	for( bit_index = 0; bit_index < 8; ++bit_index )
		skip += ( bit_counter >> ( 7 - bit_index ) ) & 1;
	printf("skip for opcode %x is %d [ addr %04x -> %04x ]\n", opcode, skip, preIP, cpu->IP + skip);
	return skip;
}

// opxx = undefined

#define op00 opxx
//#define op01 opxx
#define op02 opxx
#define op03 opxx
//#define op04 opxx
#define op05 opxx
#define op06 opxx
#define op07 opxx
#define op08 opxx
//#define op09 opxx
//#define op0a opxx
#define op0b opxx
#define op0c opxx
#define op0d opxx
#define op0e opxx
#define op0f opxx

#define op10 opxx
#define op11 opxx
#define op12 opxx
#define op13 opxx
#define op14 opxx
#define op15 opxx
#define op16 opxx
#define op17 opxx
#define op18 opxx
//#define op19 opxx
#define op1a opxx
#define op1b opxx
#define op1c opxx
#define op1d opxx
#define op1e opxx
#define op1f opxx

//#define op20 opxx
//#define op21 opxx
#define op22 opxx
#define op23 opxx
#define op24 opxx
#define op25 opxx
#define op26 opxx
#define op27 opxx
#define op28 opxx
//#define op29 opxx
#define op2a opxx
#define op2b opxx
//#define op2c opxx
#define op2d opxx
#define op2e opxx
#define op2f opxx

#define op30 opxx
//#define op31 opxx
#define op32 opxx
#define op33 opxx
#define op34 opxx
#define op35 opxx
#define op36 opxx
#define op37 opxx
#define op38 opxx
//#define op39 opxx
#define op3a opxx
#define op3b opxx
//#define op3c opxx
#define op3d opxx
#define op3e opxx
#define op3f opxx

//#define op40
#define op41 op40
#define op42 op40
#define op43 op40
#define op44 op40
#define op45 op40
#define op46 op40
#define op47 op40
//#define op48
#define op49 op48
#define op4a op48
#define op4b op48
#define op4c op48
#define op4d op48
#define op4e op48
#define op4f op48

//#define op50 
#define op51 op50
#define op52 op50
#define op53 op50
#define op54 op50
#define op55 op50
#define op56 op50
#define op57 op50
//#define op58
#define op59 op58
#define op5a op58
#define op5b op58
#define op5c op58
#define op5d op58
#define op5e op58
#define op5f op58

#define op60 opxx
#define op61 opxx
#define op62 opxx
#define op63 opxx
#define op64 opxx // todo
#define op65 opxx // todo
#define op66 opxx
#define op67 opxx
#define op68 opxx
#define op69 opxx
#define op6a opxx
#define op6b opxx
#define op6c opxx // todo
#define op6d opxx
#define op6e opxx
#define op6f opxx // todo

#define op70 opxx
#define op71 opxx
//#define op72 opxx
#define op73 opxx
//#define op74 opxx
//#define op75 opxx
//#define op76 opxx
//#define op77 opxx // todo
#define op78 opxx
//#define op79 opxx
#define op7a opxx
#define op7b opxx
#define op7c opxx
#define op7d opxx
#define op7e opxx
#define op7f opxx

//#define op80 opxx // todo
//#define op81 opxx
#define op82 op80
//#define op83 opxx
#define op84 opxx
#define op85 opxx
//#define op86 opxx
//#define op87 opxx
//#define op88 opxx
//#define op89 opxx
//#define op8a opxx
//#define op8b opxx
#define op8c opxx
#define op8d opxx
#define op8e opxx
#define op8f opxx

//#define op90 opxx
#define op91 op90
#define op92 op90
#define op93 op90
#define op94 op90
#define op95 op90
#define op96 op90
#define op97 op90
#define op98 opxx
#define op99 opxx
#define op9a opxx
#define op9b opxx
#define op9c opxx
#define op9d opxx
#define op9e opxx
#define op9f opxx

#define opa0 opxx
#define opa1 opxx
#define opa2 opxx
#define opa3 opxx
#define opa4 opxx
#define opa5 opxx
#define opa6 opxx
#define opa7 opxx
#define opa8 opxx
#define opa9 opxx // todo
#define opaa opxx
#define opab opxx
#define opac opxx
#define opad opxx
#define opae opxx
#define opaf opxx

//#define opb0
#define opb1 opb0
#define opb2 opb0
#define opb3 opb0
#define opb4 opb0
#define opb5 opb0
#define opb6 opb0
#define opb7 opb0
//#define opb8
#define opb9 opb8
#define opba opb8
#define opbb opb8
#define opbc opb8
#define opbd opb8
#define opbe opb8
#define opbf opb8

#define opc0 opxx
#define opc1 opxx
#define opc2 opxx
//#define opc3 opxx
//#define opc4 opxx
#define opc5 opxx
#define opc6 opxx
//#define opc7 opxx
#define opc8 opxx
#define opc9 opxx
#define opca opxx
#define opcb opxx
#define opcc opxx
#define opcd opxx
#define opce opxx
#define opcf opxx

#define opd0 opxx
#define opd1 opxx
#define opd2 opxx
//#define opd3 opxx
#define opd4 opxx
#define opd5 opxx
#define opd6 opxx
#define opd7 opxx
#define opd8 opxx
#define opd9 opxx
#define opda opxx
#define opdb opxx
#define opdc opxx
#define opdd opxx
#define opde opxx
#define opdf opxx

//#define ope0 opxx
#define ope1 opxx
#define ope2 opxx
#define ope3 opxx
#define ope4 opxx
#define ope5 opxx
#define ope6 opxx
#define ope7 opxx
//#define ope8 opxx
#define ope9 opxx
#define opea opxx
//#define opeb opxx
#define opec opxx
#define oped opxx
#define opee opxx
#define opef opxx

#define opf0 opxx
#define opf1 opxx
#define opf2 opxx
#define opf3 opxx
//#define opf4 opxx
#define opf5 opxx
#define opf6 opxx
#define opf7 opxx
//#define opf8 opxx
//#define opf9 opxx
#define opfa opxx
#define opfb opxx
#define opfc opxx
#define opfd opxx
//#define opfe opxx
#define opff opxx

#define op81_0 opxx
#define op81_1 opxx
#define op81_2 opxx
#define op81_3 opxx
#define op81_4 opxx
#define op81_5 opxx
#define op81_6 opxx
//#define op81_7 opxx

op86_fn op86_opcodes[256] = 
{
	// x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xa    xb    xc    xd    xe    xf 
	&op00,&op01,&op02,&op03,&op04,&op05,&op06,&op07,&op08,&op09,&op0a,&op0b,&op0c,&op0d,&op0e,&op0f, // 0x
	&op10,&op11,&op12,&op13,&op14,&op15,&op16,&op17,&op18,&op19,&op1a,&op1b,&op1c,&op1d,&op1e,&op1f, // 1x
	&op20,&op21,&op22,&op23,&op24,&op25,&op26,&op27,&op28,&op29,&op2a,&op2b,&op2c,&op2d,&op2e,&op2f, // 2x
	&op30,&op31,&op32,&op33,&op34,&op35,&op36,&op37,&op38,&op39,&op3a,&op3b,&op3c,&op3d,&op3e,&op3f, // 3x
	&op40,&op41,&op42,&op43,&op44,&op45,&op46,&op47,&op48,&op49,&op4a,&op4b,&op4c,&op4d,&op4e,&op4f, // 4x
	&op50,&op51,&op52,&op53,&op54,&op55,&op56,&op57,&op58,&op59,&op5a,&op5b,&op5c,&op5d,&op5e,&op5f, // 5x
	&op60,&op61,&op62,&op63,&op64,&op65,&op66,&op67,&op68,&op09,&op6a,&op6b,&op6c,&op6d,&op6e,&op6f, // 6x
	&op70,&op71,&op72,&op73,&op74,&op75,&op76,&op77,&op78,&op79,&op7a,&op7b,&op7c,&op7d,&op7e,&op7f, // 7x
	&op80,&op81,&op82,&op83,&op84,&op85,&op86,&op87,&op88,&op89,&op8a,&op8b,&op8c,&op8d,&op8e,&op8f, // 8x
	&op90,&op91,&op92,&op93,&op94,&op95,&op96,&op97,&op98,&op99,&op9a,&op9b,&op9c,&op9d,&op9e,&op9f, // 9x
	&opa0,&opa1,&opa2,&opa3,&opa4,&opa5,&opa6,&opa7,&opa8,&opa9,&opaa,&opab,&opac,&opad,&opae,&opaf, // ax
	&opb0,&opb1,&opb2,&opb3,&opb4,&opb5,&opb6,&opb7,&opb8,&opb9,&opba,&opbb,&opbc,&opbd,&opbe,&opbf, // bx
	&opc0,&opc1,&opc2,&opc3,&opc4,&opc5,&opc6,&opc7,&opc8,&opc9,&opca,&opcb,&opcc,&opcd,&opce,&opcf, // cx
	&opd0,&opd1,&opd2,&opd3,&opd4,&opd5,&opd6,&opd7,&opd8,&opd9,&opda,&opdb,&opdc,&opdd,&opde,&opdf, // dx
	&ope0,&ope1,&ope2,&ope3,&ope4,&ope5,&ope6,&ope7,&ope8,&ope9,&opea,&opeb,&opec,&oped,&opee,&opef, // ex
	&opf0,&opf1,&opf2,&opf3,&opf4,&opf5,&opf6,&opf7,&opf8,&opf9,&opfa,&opfb,&opfc,&opfd,&opfe,&opff  // fx
};

