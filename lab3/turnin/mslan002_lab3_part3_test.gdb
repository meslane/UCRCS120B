file build/part3.elf
target remote :1234
break main.c:7
commands
print PINA
print PORTC
end
c
set PINA = 0x3F
c
set PINA = 0x7F
c
