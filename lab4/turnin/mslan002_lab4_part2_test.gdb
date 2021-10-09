file build/part2.elf
target remote :1234
break main.c:78
commands
print PINA
print state
print PORTC
end
c
c
set PINA = 1
c
set PINA = 0
c
set PINA = 2
c
set PINA = 1
c
set PINA = 0
c
set PINA = 3
c
