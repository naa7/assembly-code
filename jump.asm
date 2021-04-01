



.model small
.data
.code
    main proc
        
        mov dl,21h
        
        mov ah,2h
        int 21h
        
        top:
        add dl,1      
        
        mov ah,2h
        int 21h
        
        jmp top
        
    endp
    end main