CC=cl
.PHONY:all clean
varargs_t.exe:varargs_t.c
	cl /D "WIN32" $?
rand_matrix.exe:rand_matrix.c
	cl /D "WIN32" $?
malloc_t.exe:malloc_t.c
	cl /D "WIN32" $?
longint_t.exe:longint_t.c
	cl /D "WIN32" $?
localtime_t.exe:localtime_t.c
	cl /D "WIN32" $?
getcodepos_t.exe:getcodepos_t.c
	cl /D "WIN32" $?
f2c.exe:f2c.c
	cl $?
c2f.exe:c2f.c
	cl $?
creat_t.exe:creat_t.c
	cl /D "WIN32" $?
asm_t.exe:asm_t.c
	cl /D "WIN32" $?
ascii.exe:ascii.c
	cl $?
argv_t.exe:argv_t.c
	cl $?
alarm_t.exe:alarm_t.c
	cl /D "WIN32" $? User32.lib
clean:
	del *.obj *.exe
