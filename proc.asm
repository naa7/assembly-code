.model small
.data
.code
    main proc
        
        mov ax,1
        mov bx,2
        call blue 
        mov ax,4
        mov bx,3
        
        int 21h
    endp
    
    blue proc
        
        mov ax,2
        mov bx,1
        
       
       ret
    blue endp
    
    end main