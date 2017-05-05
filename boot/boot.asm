; ObsidianOS bootloader
;
; First stage must fit in 446 bytes (image builder will check)
; Jobs for 1st stage loader:
; -Copy self to end of low memory to make room for system image
; -Set up stack, segment regs
; -Get drive geometry
; -Load system image sectors at 0x500
; -Enter protected mode with trivial GDT
; -Jump to kernel entry point
;
; Can't rely on either starting cs:ip being 0x7C0:0 or 0:0x7C00,
; so bootloader is completely location independent
;
; set up stack and data right above the 1st stage position
; stack will grow towards this data but will never reach it
;
; Note: initial conditions at boot:
; org 0x3E: the offset of code within boot sector
; ax = # of kernel sectors
; bx = first kernel sector (aka start of FS data area)
; cx = kernel entry point (absolute memory address)
; dl = disk # (possibly used for getting drive geometry)
;
; first, copy whole bootloader from 0x7C00 to 0x8000, then far jump to it

; temporarily save cx in sp
mov sp, cx
mov si, 0x8000
mov es, si
mov cx, 256
; zero di
xor di, di
; zero ds
mov ds, di
mov si, 0x7C00
; copy boot sector from 0x0:0x7C00 (ds:si) to 0x8000:0 (es:di)
rep movsw
; far jump to resume label in new copy
; note: build.py puts an "org 0x3E" at the start of this file, so actual total address is 0x80000 + 0x3E + resume
jmp 0x8000:resume

resume:
jmp fillFile

; restore cx
mov cx, sp
; set up ds, ss
mov si, 0x8200
mov ds, si
mov ss, si
mov bp, 0x1000
mov sp, bp

; save ax/bx/cx
mov [bp + 0], ax
mov [bp + 2], bx
mov [bp + 4], cx

; from now on:
; [bp + 0] = # of kernel sectors to read
; [bp + 2] = first kernel sector
; [bp + 4] = kernel entry point

; get disk geometry
mov ah, 0x8
xor bx, bx
mov es, bx
mov di, bx
int 0x13
; save cylinders to 0x7E00, heads to 0x7E02, sectors/track to 0x7E04
; sectors/track in cl 5:0
xor ax, ax
mov al, cl
and al, 0x3F
mov [4], ax
; heads in dh
mov al, dh
mov [2], ax
; cylinders: in cl 7:6 (high), and ch 7:0 (low)
mov al, ch
shr cl, 6
mov ah, cl
mov [0], ax

; read [bp + 0] sectors (starting at [bp + 2]) to 0x500
mov ax, 0x50
mov es, ax

; prepare to read 16 sectors starting at 32 to es:0
mov ax, [bp + 2]
mov bx, 0
mov cx, [bp + 0]

readLoop:
push ax
push cx
;call readLBA
pop cx
pop ax
inc ax
add bx, 512
loopnz readLoop

fillFile:

; set video mode to 13h
mov ah, 0
mov al, 0x13
int 0x10

; fill with orange (0x2A)

mov ax, 0xA000
mov es, ax

xor bx, bx
fillLoop:
mov word [es:bx], 0x2A2A
inc bx
inc bx
cmp bx, 64000 
jne fillLoop

; read sector into start of video mem to test
mov ax, 32
xor bx, bx
call readLBA

stop:
jmp stop

; readLBA reads the sector ax (LBA) to es:bx
; handles the LBA -> CHS conversion
; preserves bx
; note: div z ==> ax /= z, dx = ax % z
readLBA:
  push bp
  mov bp, sp
  ; [bp - 2] = ax = LBA
  push ax
  ; ax now free for computations
  div word [4]
  ; ax = temp, dx = CHS sector - 1
  inc dx
  ; dx = CHS sector, save it
  push dx
  div word [2]
  ; ax = CHS cylinder
  ; dx = CHS head
  mov dh, dl
  mov ch, al
  shr ax, 8
  shl ax, 6
  or cl, al
  mov ah, 2
  mov al, 1
  mov dl, 0x80
  int 0x13
  pop dx
  pop ax
  pop bp
  ret
