.model small
.data
.code 
    main proc   
        
        ;conditional jumps:
        ;jc - jump if carry (carry flag is set)
        ;jnc - jump if NOT carry (carry flag is clear)
        ;jz - jump if zero (zero flag is set)
        ;jnz - jump if NOT zero (zero flag is clear)
        
        ;conditional jumps based on equality:
        ;je - jump if both operands equal each other
        ;jne - jump if both operands NOT equal each other
        ;jcxz - jump if cx = zero
        
        
        mov ax,5
        cmp ax,5
        jz l1
        
        l2:
            mov bx,1
            mov cx,3
            cmp cx,bx
            jz l1
            jc l2
            jnz l3    
            
        l1:
            mov bx,6
            cmp ax,bx
            jc l2
            
        l3:
            cmp bx,cx
            jc l4
            
        l4:
            mov ax,0
            mov bx,0
            mov cx,0
            cmp ax,cx
            je l5
            
        l5:
            mov ax,2
            mov bx,3
            mov cx,4 
            cmp bx,cx  
            jne l6
            
        l6:
            mov ax,3
            mov bx,2
            mov cx,0
            cmp ax,bx
            je l5
            jcxz l7
            
        l7:
            mov ax,0
            mov bx,0
            mov cx,0
            end    
                
    endp
    end main