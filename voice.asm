lcd_rs equ p1.5
lcd_rw equ p1.6
lcd_en equ p1.7

org 00h
ljmp start
org 23h
ljmp serial_isr
org 100h

start:
acall hello
acall delay1sec
mov tmod,#20h
mov th1,#0fdh
mov scon,#50h
mov ie,#90h
setb tr1
acall delay1sec
mov p2,#00h
mov r1,#00h

main:jb p3.0,chkloop
acall delay500ms
jmp main

chkloop:
cjne r1,#39h,forward
mov p1,#0fh
acall clear
acall line1
mov dptr,#led
acall dispdata
acall delay500ms
acall hello
jmp main

forward:
cjne r1,#32h,backward
mov p2,#0ah
acall clear
acall line1
mov dptr,#fwd
acall dispdata
acall delay500ms
acall hello
jmp main

backward:
cjne r1,#33h,left
mov p2,#05h
acall clear
acall line1
mov dptr,#bwd
acall dispdata
acall delay500ms
acall hello
jmp main

left:
cjne r1,#34h,right
mov p2,#09h
acall clear
acall line1
mov dptr,#lft
acall dispdata
acall delay500ms
acall hello
jmp main

right:
cjne r1,#35h,rever1
mov p2,#06h
acall clear
acall line1
mov dptr,#rt
acall dispdata
acall delay500ms
acall hello
jmp main

rever1:
cjne r1,#36h,rever2
mov p2,#08h
acall clear
acall line1
mov dptr,#blt
acall dispdata
acall delay500ms
acall hello
jmp main

rever2:
cjne r1,#37h,stop
mov p2,#02h
acall clear
acall line1
mov dptr,#brt
acall dispdata
acall delay500ms
acall hello
jmp main

stop:
cjne r1,#38h,enter
mov p2,#00h
acall clear
acall line1
mov dptr,#st
acall dispdata
acall delay500ms
acall hello
jmp main
enter:jmp main

hello:
acall clear
acall lcd_init
acall line1
mov dptr,#mydata
acall dispdata
acall line2
mov dptr,#mydata1
acall dispdata
ret

mydata:db" VOICE CONTROL ",0
mydata1:db" ROBOT ",0
led:db"CONNECTED",0
fwd:db"FORWARD",0
bwd:db"BACKWARD",0
lft:db"LEFT",0
rt:db"RIGHT",0
blt:db"REVERSE LEFT",0
brt:db"REVERSE RIGHT",0
st:db"STOP",0

dispdata:
next:clr a
movc a,@a+dptr
jz ext
acall dat
inc dptr
jmp next
ext:ret

lcd_init:
mov a,#30h
acall cmnd
mov a,#38h
acall cmnd
mov a,#06h
acall cmnd
mov a,#0ch
acall cmnd
mov a,#01h
acall cmnd
ret

cmnd:
acall delay500ms
acall delay500ms
clr lcd_rs
clr lcd_rw
setb lcd_en
mov p0,a
nop
nop
clr lcd_en
ret

dat:
acall delay500ms
acall delay500ms
setb lcd_rs
clr lcd_rw
setb lcd_en
mov p0,a
nop
nop
clr lcd_en
ret

clear:
mov a,#01h
acall cmnd
ret
line1:
mov a,#80h
acall cmnd
ret
line2:
mov a,#0c0h
acall cmnd
ret

serial_isr:
jb ri,rx_service
jb ti,tx_service
rx_service:
clr ri
mov a,sbuf
mov r1,a
acall delay20ms
setb p3.0
jmp end_isr

tx_service:clr ti
jmp end_isr
end_isr:ret

delay1sec:
mov r3,#10h
l1:mov r4,#200
l2:mov r5,#250
l3:djnz r5,l3
djnz r4,l2
djnz r3,l1
ret
delay500ms:
mov r3,#183
l4:mov r4,#250
l5:djnz r4,l5
djnz r3,l4
ret
delay20ms:
mov r3,#40
l6:mov r4,#250
l7:djnz r4,l7
djnz r3,l6
ret
end