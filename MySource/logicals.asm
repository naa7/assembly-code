.model small
.data
.code
    main proc 
           
        ;logical AND:
        ;x y -> x*y
        ;0 0 -> 0
        ;1 0 -> 0
        ;0 1 -> 0
        ;1 1 -> 1
        
        mov ah,00000001b
        mov bh,00000101b
        and bh,ah ; will compare the values of ah and bh with logical AND and will assign the new value to ah
         
              
        ;logical OR:      
        ;x y -> x+y
        ;0 0 -> 0
        ;1 0 -> 1
        ;0 1 -> 1
        ;1 1 -> 1    
        
        mov ch,00000010b
        mov dh,00000011b
        or ch,dh ;will compare the values of ch and dh with logical OR and will assign the new value to ch
        
                     
        ;logical XOR:             
        ;x y -> x^y
        ;0 0 -> 0
        ;1 0 -> 1
        ;0 1 -> 1
        ;1 1 -> 0 
        
        mov al,00000011b
        mov bl,00000101b
        xor al,bl ;will compare the values of al and bl with logical XOR and will assign the new value to al
                 
                    
        ;logical NOT:           
        mov ah,11111000b
        not ah ;logical NOT will inverts the operand and 1s will be 0s and 0s will be 1s
                     
                     
         ;test instruction:      
         mov ah,00000001b
         test ah,00000001b ;test instruction works the same as logical AND but will NOT affect the value of ah. it just test it
         test ah,00001000b ;The change will appear in the flags section, zero flag (zf). if not matching zf will be 1 and if matching zf will be 0
         
         ;CMP instruction:
         
         ;CMP results               zf(zero flag)   cf(carry flag)
         ;Destination < source            0                1
         ;Destination > source            0                0
         ;Destination = source            1                0
         
         mov ax,500
         mov bx,200
         mov cx,500
         cmp ax,bx 
         cmp bx,ax 
         cmp ax,cx
         
         
         
    
    
    endp
    end main
         
                  
        