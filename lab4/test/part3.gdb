file build/part3.elf
target remote :1234
break main.c:59
commands
print PINA
print PORTB
print state
print PORTC
end
c
c
set PINA = 4
c
set PINA = 0
c
set PINA = 1
c
set PINA = 0
c
set PINA = 4
c
set PINA = 2
c
set PINA = 0
c
set PINA = 0x80
c

