file build/part1.elf
target remote :1234
break main.c:7
commands
print PINA
print PINB
print PORTC
end
c
set PINA = 0x01
c
set PINB = 0x01
c
set PINA = 0x80
set PINB = 0x03
c
set PINA = 0xFF
set PINB = 0xFF
c
