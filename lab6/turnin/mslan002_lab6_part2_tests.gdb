file build/part2.elf
target remote :1234
break main.c:134
commands
print PINA
print PORTB
print state
print prior
end
c
c
set PINA = 1
c
c
set PINA = 0
c
set PINA = 1
c
set PINA = 0
c
c
c