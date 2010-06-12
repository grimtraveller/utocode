CC=cl
CFLAG=/nologo /Zi /O2
RM=del
all: Cart

Meal.obj: Meal.h Meal.cpp
	$(CC) $(CFLAG) /c Meal.cpp

SnackCart.obj: Meal.h SnackCart.h SnackCart.cpp
	$(CC) $(CFLAG) /c SnackCart.cpp

StreetSnacks.obj: Meal.h SnackCart.h StreetSnacks.cpp
	$(CC) $(CFLAG) /c StreetSnacks.cpp 

Cart: Meal.obj SnackCart.obj StreetSnacks.obj
	$(CC) $(CFLAG) Meal.obj SnackCart.obj StreetSnacks.obj /FeLiDun.exe

clean:
	$(RM) *.obj LiDun.exe
	$(RM) *.ilk *.pdb
