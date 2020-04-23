.model small
.data
    var1 db 1
.code
    main proc
        
        mov ah,1h
        int 21h 
        
        mov dl,al 
        mov ax,0
        add dl,3
        mov al,dl
        add ax,-1
        mov dl,al 
        neg dl  ;negative dl (2's compilment of dl = 4 -> -4)
 
        mov ah,2h   ;code for print a char
        int 21h
        
        
        
    endp
    end main