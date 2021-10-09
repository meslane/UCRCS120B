file build/part2.elf
target remote :1234
break main.c:7
commands
print PINA
print PORTC
end
c
set PINA = 0x0F
c
set PINA = 12
c
set PINA = 9
c
set PINA = 7
c
set PINA = 4
c
set PINA = 2
c
set PINA = 0
c
