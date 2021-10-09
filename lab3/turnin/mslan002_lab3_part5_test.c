file build/part5.elf
target remote :1234
break main.c:11
commands
print PINB
print PIND
print weight
print PORTB
end
c
set PIND = 15
set PINB = 1
c
set PIND = 64
set PINB = 0
c
set PIND = 1
set PINB = 0
c
