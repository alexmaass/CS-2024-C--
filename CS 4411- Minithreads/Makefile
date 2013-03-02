# Makefile for minithreads on x86/NT

# You probably need to modify the MAIN variable to build the desired file
# You may also need to modify some of the example directories below, in case
# your system has different SDK versions or directory structure.

CC = cl.exe
LINK = link.exe

WINSOCKLIB = ws2_32.lib

###############################################################################

# COMPILE OPTIONS FOR VISUAL STUDIO 2008 -------------------------------------------------

# Uncomment these when compiling in Visual Studio 2008 to output a 32 bit binary on a 32 bit machine
#SDKLIBPATH = "C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib"
#VCLIBPATH = "C:\Program Files\Microsoft Visual Studio 9.0\VC\lib"
#WINVER = "WIN32"
#MACHINE = "I386"
#PRIMITIVES = machineprimitives_x86_asm

# Uncomment these when compiling in Visual Studio 2008 to output a 32 bit binary on a 64 bit machine
# Note that you need to open the normal Visual Studio Command Prompt (not the x64 Win64 Command Prompt) to use these options.
#SDKLIBPATH = "C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib"
#VCLIBPATH = "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib"
#WINVER = "WIN32"
#MACHINE = "I386"
#PRIMITIVES = machineprimitives_x86_asm

# Uncomment these when compiling in Visual Studio 2008 to output a 64 bit binary on a 64 bit machine
# Note that you need to open a Visual Studio x64 Win64 Command Prompt to use these options.
#SDKLIBPATH = "C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib\x64"
#VCLIBPATH = "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib\amd64"
#WINVER = "WIN64"
#MACHINE = "X64"
#PRIMITIVES = machineprimitives_x86_64_asm

# COMPILE OPTIONS FOR VISUAL STUDIO 2010 -------------------------------------------------

# Uncomment these when compiling in Visual Studio 2010 to output a 32 bit binary
# Note that you need to open the normal Visual Studio Command Prompt (not the x64 Win64 Command Prompt) to use these options.
#SDKLIBPATH = "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"
#VCLIBPATH = "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib"
#WINVER = "WIN32"
#MACHINE = "I386"
#PRIMITIVES = machineprimitives_x86_asm

# Uncomment these when compiling in Visual Studio 2010 to output a 64 bit binary
# Note that you need to open a Visual Studio x64 Win64 Command Prompt to use these options.
SDKLIBPATH = "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\x64"
VCLIBPATH = "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\amd64"
WINVER = "WIN64"
MACHINE = "X64"
PRIMITIVES = machineprimitives_x86_64_asm

###############################################################################

CFLAGS = /nologo /MTd /W3 /Gm /EHsc /Zi /Od /D $(WINVER) /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"minithreads.pch" /Fo"" /Fd"" /FD /RTC1 /c 
LFLAGS = /nologo /subsystem:console /incremental:no /pdb:"minithreads.pdb" /debug /machine:$(MACHINE) /out:"minithreads.exe" /LIBPATH:$(SDKLIBPATH) /LIBPATH:$(VCLIBPATH)
ASFLAGS = /c /Zi /Zf

LIB = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(WINSOCKLIB)

# change this to the name of the file you want to link with minithreads, 
# dropping the ".c": so to use "sieve.c", change to "MAIN = sieve".

MAIN = test1

SYSTEMOBJ = interrupts.obj \

OBJ = 	random.obj\
	minithread.obj \
	machineprimitives_x86.obj \
	$(PRIMITIVES).obj \
	machineprimitives.obj \
	queue.obj \
	$(MAIN).obj \
	synch.obj 


all: minithreads.exe

.c.obj:
	$(CC) $(CFLAGS) $<

machineprimitives_x86_asm.obj: machineprimitives_x86_asm.S
	ml.exe $(ASFLAGS) machineprimitives_x86_asm.S

machineprimitives_x86_64_asm.obj: machineprimitives_x86_64_asm.S
	ml64.exe $(ASFLAGS) machineprimitives_x86_64_asm.S

minithreads.exe: start.obj end.obj $(OBJ) $(SYSTEMOBJ)
	$(LINK) $(LFLAGS) $(LIB) $(SYSTEMOBJ) start.obj $(OBJ) end.obj $(LFLAGS)

clean:
	-@del /F /Q *.obj
	-@del /F /Q minithreads.pch minithreads.pdb 
	-@del /F /Q minithreads.exe

#depend: 
#	gcc -MM *.c 2>/dev/null | sed -e "s/\.o/.obj/" > depend


include Depend
