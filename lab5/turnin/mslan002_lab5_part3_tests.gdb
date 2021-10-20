file build/part3.elf
target remote :1234
break main.c:55
commands
print PINA
print state
print PORTB
end
c
PINA = 1
c
PINA = 0
c
PINA = 1
c
PINA = 0
c
PINA = 1
c
PINA = 0
c
PINA = 1
c
PINA = 0
c