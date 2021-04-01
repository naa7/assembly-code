.model small
.data
.code
    main proc
        
        mov ah,1h
        int 21h 
        mov bx,1 
        mov dl,al
        mov al,30h
        cmp dl,al
        ja l1
        je l2 
        
        l2:
             mov dl,30h 
             mov cx,9
             numbers:
                    inc dl
                    mov ah,2h
                    int 21h
                    cmp bx,1
            
             loopz numbers
        
        jcxz l3
      
        l1:  
             mov dl,40h
             mov cx,26
             letters:
                    inc dl
                    mov ah,2h
                    int 21h
            
             loop letters  
            
        l3:
            end      
            
    endp
    end main
    
    
    