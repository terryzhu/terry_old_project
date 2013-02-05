#	$(CC) -g -c -o stdio.o stdio.c
#	$(CC) -g -c -o test.o test.S
#	$(CC) -g -c -o main.o main.c
#	$(CC) -g -c -o gdi.o gdi.c
#	$(CC) -g -c -o interrupt.o interrupt.c
#	$(CC) -g -c -o wnd.o wnd.c


CC=arm-elf-gcc
LD=arm-elf-ld
OBJCP=arm-elf-objcopy
OBJTEMP=test.o main.o stdio.o gdi.o interrupt.o wnd.o
OBJTARGET=tmp.o
TARGET=C.bin
CCFLAGS=-g -c -o
all: $(TARGET)
$(TARGET): $(OBJTARGET)
	$(OBJCP) -O binary -S $< $@	
$(OBJTARGET):$(OBJTEMP)
#$+所有的依赖文件，以空格分开，并以出现的先后为序，可能包含重复的依赖文件。
	$(LD) -T test.lds -g $+ -o $@
test.o: test.s
	$(CC) $(CCFLAGS) $@ $<
%.o: %.c
	$(CC) $(CCFLAGS) $@ $<

clean:
	del $(OBJTEMP)
	
