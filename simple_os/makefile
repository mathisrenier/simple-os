# compiles everything
all: shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c memorymanager.c DISK_driver.c
	gcc -c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c memorymanager.c DISK_driver.c
	gcc -o mykernel shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o -lm

# removes the .o files
clean-o: shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o
	rm shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o

# removes the .o files and the executable
clean: shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o mykernel
	rm shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o mykernel
