.model small
.data
.code
    main proc  
        
        mov ax,2
        mov bx,2
        
        mul bx ;mul will multiply bx with ax and store the result in ax
        
        ;if the value is higher than four bits then ax will have the first four bits and
        ;dx will have the other four bits and cf (carry flag) will be set to 1
        
        mov cx,1000h
        mov dx,200h
        
        mul dx ;mul will multiply bx with ax and result will be stored in ax.
               ;200 x 4 = 800 -> ax = 800h = 00000800. ax = 0800 and dx = 0000
              
        div bx
        
        
        mov ax,85h
        div bx ;div will divide ax by bx and result (quotient) will go to ax and remainder to dx
               ;85/2 = 42 (quotient) = ax, 1 (remainder) = dx
               
        mov ax,83h
        mov bx,2
        div bl ;div will divide ax by bx and result (quotient) will go to al and remainder to ah
               ;85/2 = 42 (quotient) = al, 1 (remainder) = ah            
        
    endp 
    end main