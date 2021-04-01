.model small
.data

.code
    main proc

        mov ah,1h
        int 21h

        mov dl,al

        add dl,48

        mov ah,2h
        int 21h

        inc dl  ;increment the value of dl by 1

        mov ah,2h
        int 21h
        
        dec dl  ;decrement the value of dl by 1
        
        mov ah,2h
        int 21h  
        
         

    endp
    end main

