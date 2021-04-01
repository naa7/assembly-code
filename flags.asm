.model small
.data
.code
    main proc   
        
        ;carry flag (cf) - unsigned overflow (0-255)
        mov cl,255
        add cl,1  
        
        ;overflow flag (of) - signed overflow (-128 - 127)
        mov cl,-128
        add cl,1
        
        ;parity flag (af) - even number of one bits
        mov al,2 ;00000010 odd number of one bits
        add al,1 ;00000001 odd number of one bits
                 ;00000011 even number of one bits
        
        ;auxillary flag (af) - unsigned overflow for low nibble (4-bits)
        mov bl,8 ;00001000 1 in the fourth bit 
        add bl,8 ;00001000 1 in the fourth bit
                 ;00010000 1 carry to the fifth bit
                 
        ;zero flag (zf) - result is zero
        mov cl,0
        add cl,0                        
        
        ;sign flag (sf) - result is negative
        mov cl,-1
        add cl,-2
        
    endp
    end main