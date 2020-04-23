.model small
.data
.code
main proc 
    
    mov ah,1h
    int 21h
    
    mov dl,al
    mov dl,48
    
    mov ah,2h
    int 21h
    
endp
end main