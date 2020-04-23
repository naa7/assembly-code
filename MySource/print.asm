.model small
.data
.code

mov ah,1h       ;code for read character
int 21h         ;dos interrupt "do it"

mov dl,al       ;copy character from al to dl

mov ah,2h       ;code for write character
int 21h         ;dos interrupt "do it"

end

