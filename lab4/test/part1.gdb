file build/part1.elf
target remote :1234
break main.c:55
commands
print PINA 
print state
print PORTB
end
c
c
set PINA = 1
c
set PINA = 0
c
set PINA = 1
c
set PINA = 0
c
