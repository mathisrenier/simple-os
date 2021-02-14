# compiles everything
all: shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c
	gcc -c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c
	gcc -o mykernel shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o

# removes the .o files
clean-o: shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o
	rm shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o

# removes the .o files and the executable
clean: shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o mykernel
	rm shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o mykernel