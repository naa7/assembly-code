//
//  flags.h
//  retro86
//

#ifndef FLAGS_H
#define FLAGS_H

#include "retro86.h"
#include "cpu8086.h"

extern void set_flag_add_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2 );
extern void set_flag_add_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2 );
extern void set_flag_sub_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2 );
extern void set_flag_sub_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2 );
extern void set_flag_log_byte( cpu_state_ptr cpu, uint8_t v1 );
extern void set_flag_log_word( cpu_state_ptr cpu, uint16_t v1 );
extern void set_flag_adc_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2, uint8_t v3 );
extern void set_flag_adc_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2, uint16_t v3 );
extern void set_flag_sbb_byte( cpu_state_ptr cpu, uint8_t v1, uint8_t v2, uint8_t v3 );
extern void set_flag_sbb_word( cpu_state_ptr cpu, uint16_t v1, uint16_t v2, uint16_t v3 );

#endif // FLAGS_H
