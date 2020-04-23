//
//  cpu8086.h
//  retro86
//

#ifndef _CPU8086_H
#define _CPU8086_H

#include "retro86.h"

typedef struct CPU_STATE_S
{
	// general registers
	uint16_t AX; // Accumulator register for operands and results data.
	uint16_t BX; // Base resigster. pointer to data in the DS segment.
	uint16_t CX; // Count register for string and loop operations.
	uint16_t DX; // Data register. I/O pointer.
	
	// index registers
	uint16_t SI; // Pointer to data in the segment pointed to by the DS register; source pointer for string operations. (Source index register. Used for string and memory array copying)
	uint16_t DI; // Pointer to data (or destination) in the segment pointed to by the ES register; destination pointer for string operations. (Destination index register. Used for string, memory array copying and setting and for far pointer addressing with ES)
	
	// pointer registers
	uint16_t BP; // Pointer to data on the stack (in the SS segment). (Stack Base pointer register. Holds the base address of the stack)
	uint16_t SP; // Stack pointer (in the SS segment). (Stack pointer register.Holds the top address of the stack)
	
	// flags register
	uint16_t IP; // Index Pointer. Holds the offset of the next instruction. It can only be read
	
	union
	{
		struct
		{
			uint16_t CF:1; // carry flag - set if there was a carry from or borrow to the most significant bit during the last result calculation
			uint16_t flag1_reserved:1;
			uint16_t PF:1; // parity flag - set if parity ( the number of "1" bits ) in the lwo-order byte of the result is even
			uint16_t flag3_reserved:1;
			uint16_t AF:1; // adjust flag - set if there was a carry from or borrow to bits 0-3 in the AL register
			uint16_t flag5_reserved:1;
			uint16_t ZF:1; // zero flag - set if the result is zero
			uint16_t SF:1; // sign flag - set if the most significant bit of the result is set
			uint16_t TF:1; // trap flag - if set then single-step interrupt will occur after the next instruction
			uint16_t IF:1; // interrupt enable flag - setting this bit enables maskable interupts
			uint16_t DF:1; // direction flag - if set then string manipulation instructions will auto-descrement index registers. if cleared then the index registers will be auto-incremented
			uint16_t OF:1; // overflow flag - set if the result is too large positive number, or is to small negative number to fit into destination operand
			uint16_t IOPL:2; // I/O privilege level (always 1,1 on 8086 and 80186)
			uint16_t NT:1; // nested task flag ( always 1 on 8086 and 80186)
			uint16_t flag15_reserved:1; // always 1 on 8086 and 8086. should be 0 above
		} bit;
		uint16_t val;
	} flags;
	
	// 16-bit segment registers
	uint16_t CS; // code segment (Holds the Code segment in which your program runs. Changing its value might make the computer hang.)
	uint16_t DS; // data segment (Holds the Data segment that your program accesses. Changing its value might give erroneous data.)
	uint16_t SS; // stack segment (Holds the Stack segment your program uses. Sometimes has the same value as DS.  Changing its value can give unpredictable results, mostly data related.)
	
	// These are extra segment registers available for far pointer addressing like video memory and such.
	uint16_t ES; // data segment
	uint16_t FS; // data segment
	uint16_t GS; // data segment
	
	uint8_t* memory;
	uint8_t* mp;
	uint8_t cnt;
	union
	{
		struct
		{
			uint8_t rm:3;
			uint8_t reg:3;
			uint8_t mod:2;
		} bit;
		uint8_t val;
	} modrm;
	
} cpu_state_s, *cpu_state_ptr, **cpu_state_hdl;


extern uint8_t cpu_get_mem_u8( cpu_state_ptr cpu, uint16_t segment, uint16_t offset );
extern void cpu_set_mem_u8( cpu_state_ptr cpu, uint16_t segment, uint16_t offset, uint8_t value );

extern uint16_t cpu_get_mem_u16( cpu_state_ptr cpu, uint16_t segment, uint16_t offset );
extern void cpu_set_mem_u16( cpu_state_ptr cpu, uint16_t segment, uint16_t offset, uint16_t value );

extern void cpu_push( cpu_state_ptr cpu, uint16_t value );
extern uint16_t cpu_pop( cpu_state_ptr cpu );

extern void cpu_create( cpu_state_hdl cpu );
extern void cpu_destroy( cpu_state_hdl cpu );
extern void cpu_reset( cpu_state_ptr cpu );
extern uint8_t cpu_execute( cpu_state_ptr cpu );

#endif // _CPU8086_H
