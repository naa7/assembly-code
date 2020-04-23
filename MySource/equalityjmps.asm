.model small
.data
.code   

    main proc  
        
        ;jumps based on unsigned comparisons: 
        ;ja - jump if above (if leftOp > rightOp)
        ;jnbe - jump if not below or equal (smae as ja)
        ;jae - jump if above or equal (if leftOp >= rightOp)
        ;jnb - jump if not below (same as jae)
        ;jb - jump if below (if leftOp < rightOp)
        ;jnae - jump if not above or equal (same as jb)
        ;jbe - jump if below or equal (if leftOp <= rightOp)
        ;jna - jump if not above (same as jbe) 
        
        mov ax,10
        mov bx,11
        mov cx,12 
        
        l3:
            cmp bx,ax
            ja l2
            cmp bx,cx
            jnae l4
        
        l1:
            cmp ax,cx
            jna l3
        
        l2:
            mov ax,1
            mov bx,1
            cmp ax,bx
            jae l3
        
        l4:
            mov cx,5
            mov bx,2
            cmp bx,cx
            jbe l6
        
        l6:
            mov ax,1
            mov bx,2
            cmp ax,bx
            jb l5  
            
        l5:
            end
        
    endp
    end main
        
        