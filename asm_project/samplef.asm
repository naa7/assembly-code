  assume cs:code,ds:code
code segment
   org 100h
start:

    jmp real_start

    mov ax,2020
	
print_num:
    push dx
	push cx
	push bx
	push ax
	
	sub cx,cx
	mov bx,10
	
l1:	cwd 
	idiv bx
	push dx
	inc  cx
	or   ax,ax
	jnz l1
	
l2: pop  dx
    push cx
    add  dl,'0'
	mov  ah,6
	int  21h
	pop  cx
	loop l2
	
	mov dl,' '
	mov ah,6
	int 21h
	
	pop ax
	pop bx
	pop cx
	pop dx 
	ret
	
	
real_start:

    mov si,1
	mov di,1
	mov cx,10
l3:	xchg si,di
	mov ax,si
	call print_num
	add si,di
	
	loop l3
	

	
	mov  ax,4c00h
	int  21h
   
code ends
     end start