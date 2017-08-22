global init_tasks

extern task_start

section .data

%define shell_size          shell_end - shell
shell:                      incbin "../shell/shell.bin"
shell_end:

shell1_info:
    .addr       dd  shell
    .size       dd  shell_size
    .stdin      dd  tty1
    .stdout     dd  tty1
    .stderr     dd  tty1
    .tty        dd  1
    .stack      dd  0x10000
    .heap       dd  0x100000
    .pwd        dd  pwd
    .name       dd  shell_name
    .server_name    dd none

shell2_info:
    .addr       dd  shell
    .size       dd  shell_size
    .stdin      dd  tty2
    .stdout     dd  tty2
    .stderr     dd  tty2
    .tty        dd  2
    .stack      dd  0x10000
    .heap       dd  0x100000
    .pwd        dd  pwd1
    .name       dd  shell_name
    .server_name    dd none

shell3_info:
    .addr       dd  shell
    .size       dd  shell_size
    .stdin      dd  stty0
    .stdout     dd  stty0
    .stderr     dd  stty0
    .tty        dd  2
    .stack      dd  0x10000
    .heap       dd  0x100000
    .pwd        dd  pwd
    .name       dd  shell_name
    .server_name    dd none

pwd db "/", 0
pwd1 db "/dev", 0
shell_name db "shell", 0
none db 0

tty1 db '/dev/tty1', 0
tty2 db '/dev/tty2', 0
tty3 db '/dev/tty3', 0
stty0 db '/dev/stty0', 0

section .text

bits 32

init_tasks:
    push shell1_info
    call task_start
    add esp, 4
    push shell2_info
    call task_start
    add esp, 4
    ;push shell3_info
    ;call task_start
    ;add esp, 4
    ret
