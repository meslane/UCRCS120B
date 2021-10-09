file build/part4.elf
target remote :1234
break main.c:7
commands
print PINA
print PORTB
print PORTC
end
c
set PINA = 0x81
c
set PINA = 0xFF
c
set PINA = 0x24
c
