file build/part1.elf
target remote :1234
break main.c:101
commands
print PORTB
print state
end
c
c
c
c
c
c