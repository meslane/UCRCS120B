file build/part1.elf
target remote :1234
break main.c:152
commands
print PINA
print PORTB
print state
print prev
print holdTime
end
c
set PINA = 1
c
c 
set PINA = 0
c
c
set PINA = 2
c
c
c
c
c
c
c
c
c
c
c
set PINA = 0
c
c
set PINA = 3
c
c
set PINA = 1
c
set PINA = 0
c