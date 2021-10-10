file build/part5.elf
target remote :1234
break main.c:63
commands
print PINA
print state
print ix
print PORTB
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
set PINA = 2
c
set PINA = 0
c
set PINA = 1
c
set PINA = 0
c
set PINA = 4
c
set PINA = 0
c
set PINA = 4
c
set PINA = 0
c
set PINA = 4
c
set PINA = 1
c
set PINA = 2
c
set PINA = 1
c
set PINA = 0
c
