CC=$(CC) $(CFLAG)
CFLAG=/nologo /Zi /O2 /INCREMENTAL:NO /DWIN32
all: Eatery

Meal.obj: Meal.h Meal.cpp
	$(CC) $(CFLAG) /c Meal.cpp
Eatery.obj: Meal.h Eatery.h Eatery.cpp
	$(CC) $(CFLAG) /c Eatery.cpp
SnackCart.obj: Meal.h SnackCart.h SnackCart.cpp
	$(CC) $(CFLAG) /c SnackCart.cpp
Restaurant.obj: Meal.h Restaurant.h Restaurant.cpp
	$(CC) $(CFLAG) /c Restaurant.cpp
EateryGuide.obj: Meal.h Eatery.h Restaurant.h SnackCart.h EateryGuide.cpp
	$(CC) $(CFLAG) /c EateryGuide.cpp 

Eatery: Meal.obj Eatery.obj Restaurant.obj SnackCart.obj EateryGuide.obj
	$(CC) $(CFLAG) Meal.obj Eatery.obj Restaurant.obj SnackCart.obj EateryGuide.obj /FeLiDun.exe

clean:
	del *.pdb *.ilk *.obj LiDun.exe
