.model small
.data
    saveflags dw ? ;variable name is saveflags and dw means two bytes and ? means no value assigned yet
.code
    main proc

        mov ax,1
        push ax	;stores data (value of ax) to the top of the stack.(first "unused"spot located at the top of the stack)
        pop cx	;retrieves data from the top of the stack and assign it to cx
        mov dl,cl
        add dl,48 

        ;pushf and popf are used whenever backup of the flags is needed for a 16-bit version
        ;pushfd and popfd are used whenever backup of the flags is needed for a 32-bit version
        
        pushf ;saves the state of the flags and push it to the top of the stack   
        pop saveflags  ;retrieves the value of pushf and saves it to the variable saveflags
        
        push saveflags ;pushes the value of saveflags to the top of the stack
        popf ;retrieves or restores the state of the flags
        
        ;pusha pushes the value of all the 16-bit registers to the top of the stack in the following order: ax,cx,dx,bx,sp,bp,si,di
        ;popa retrieves the value of all the 16-bit registers from the top of the stack and restores them
        ;pushad and popad work the same but for the 32-bit version
        
        mov ax,1
        mov bx,1
        mov cx,1
        
        pusha ;pushes the value of all the registers to the top of the stack
        
        mov ax,5
        mov bx,5
        mov cx,5
        
        popa ;retrieves the values of ax,bx,cx which is 1
        
        mov ah,2h
        int 21h

    endp
    end main

