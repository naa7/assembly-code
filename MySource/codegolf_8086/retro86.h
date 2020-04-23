//
//  retro86.h
//  retro86
//

#ifndef RETRO86_H
#define RETRO86_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

#ifndef LO8
#define LO8(w)			((uint8_t)(w))
#define HI8(w)			((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#define MAKE16(h,l)		((uint16_t) (((uint8_t) (l)) | ((uint16_t) ((uint8_t) (h)) << 8))) 
#define ADD_S8(w, b)	( ( ( ( b ) & 0x80 ) ? ( ( w ) + ( ( b ) | 0xff00 ) ) : ( ( w ) + ( ( b ) & 0xff ) ) ) & 0xffff )
#define MAKE_16(lo, hi) ( ( ( hi ) << 8 ) | ( lo ) )
#define MAKE_S16(lo)	( ( ( lo ) & 0x80 ) ? ( ( lo ) | 0xff00) : ( ( lo ) & 0xff ) )
#define MODRM_MOD(b)	( ( b ) >> 6 )
#define MODRM_REG(b)	( ( ( b ) >> 3 ) & 0x7 )
#define MODRM_RM(b)		( ( b ) & 0x7 )
#endif

#define MEMSIZE 0x10000
#define MEMOFFSET_VIDEO 0x8000
#define UNHANDLED_OPCODE 0xff

// 16bit reg
#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7

// 8bit reg
#define REG_AL 0
#define REG_CL 1
#define REG_DL 2
#define REG_BL 3
#define REG_AH 4
#define REG_CH 5
#define REG_DH 6
#define REG_BH 7

// seg
#define REG_ES 0
#define REG_CS 1
#define REG_SS 2
#define REG_DS 3

// byte counter
#define BCNT_CLEAR	(0)
#define BCNT_OPCODE ( 1 << 0 )
#define BCNT_1		( 1 << 1 )
#define BCNT_2		( 1 << 2 )
#define BCNT_3		( 1 << 3 )
#define BCNT_4		( 1 << 4 )
#define BCNT_5		( 1 << 5 )
#define BCNT_6		( 1 << 6 )

#endif //RETRO86_H
