.model small
.data
.code
    main proc
        
        mov dl,0
        mov al,11111111b
        add al,al ;1111 1111 + 1111 1111 = 1111 1110 and cf is set to 1
        adc dl,dl ;0000 0000 + 0000 0000 = 0000 0001 adc added the carry flag (cf) to dl register or carried the value into dl
        
        mov ah,7
        mov bh,1
        sub bh,2 ;will trigger cf and is set to 1 since 2 is larger than bh
        sbb ah,0 ;will add the value of cf to 0 and then will subtract from ah -> 0+1=1 -> 7 -1 = 6 
        
        mov ah,0
        mov al,'8' ;-> AX = 0038h
        add al,'2' ;-> AX = 006Ah
        aaa ;converts ASCII to hexadecimal -> AX = 0100h
        or ax,3030h ;-> AX = 3130h = '10'
         
    endp
    end main