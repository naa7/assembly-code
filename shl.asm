.model small
.data
.code
    main proc  
        
        ;shl instruction shifts the destination to the left depending on the value of count
        ;and the highest bit goes to cf flag and zero goes in the previous position of the shifted
        ;value and the lowest bit is replaced with zero
        mov ax,5 ;0000 0101 -> 4+1 = 5
        shl ax,2 ;0001 0100 -> 16+4 = 20
        ;here, ax is shifted twice to the left and the highest bit goes to cf flag which is zero in
        ;this case and zeros fill in for the empty spots of 1s in this case two zeros filled in for 1s
        
        mov bx,3 ;0000 0011 -> 2+1 = 3
        shl bx,2 ;0000 1100 -> 8+4 = 12   
        
        ;shr instruction shifts the destination to the right depending on the value of count
        ;and the lowest bit goes to cf flag and zero goes in the previous position of the shifted
        ;value and the highest bit is replaced with zero
        
        mov cx,2 ;0000 0010 -> 2
        shr cx,1 ;0000 0001 -> 1
        shr cx,1 ;0000 0000 -> 0 and cf = 1
        
        ;sal instruction shifts the destination to the left depending on the value of count
        ;and the highest bit goes to cf flag and zero goes in the previous position of the shifted
        ;value and the highest bit is replaced with zero
        
        mov dx,2;0000 0010 -> 2      
        sal dx,1;0000 0100 -> 4    
        
        ;sar instruction shifts the destination to the right depending on the value of count
        ;and the lowest bit goes to cf flag and zero goes in the previous position of the shifted
        ;value and the highest bit is replaced with zero
        
        mov ax,5 ;0000 0101 -> 4+1 = 5
        sar ax,1 ;0000 0010 -> 2 and cf = 1
        
    endp
    end main
        