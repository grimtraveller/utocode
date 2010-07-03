;Name:	boot.asm
;Desc:	bios load disk0,1
;Usage:	$>apt-get install nasm
;		$>nasm boot.asm -o boot.bin
;		$>mkisofs -R -b boot.bin -no-emul-boot -boot-load-size 4 -o myos.iso boot.bin
;		$>
;Author:zuohaitao
;Date:	2009 02 06

%define BOOT_DEBUG
%ifdef BOOT_DEBUG
	org 01000h
%else
	org 07c00h	;告诉编译器程序加载到7c00处
%endif ;BOOT_DEBUG
	mov ax,cs	
	mov ds,ax
	mov es,ax	
	call DisStr	;调用显示字符串例程
	jmp $		;无限循环
DisStr:
	mov ax, BootMessage
	mov bp, ax	;es:bp = 串地址
	mov cx, 16	;cx = 串长度
	mov ax, 01301h ;ah = 13, al = 01h
	mov bx, 000ch	;页号为0 (bh=0) 黑低红字(bl=0Ch,高亮)
	mov dl, 0
	int 10h
	ret
BootMessage:	db	"hello, OS world"
times 510-($-$$) db 0
dw 0xaa55
