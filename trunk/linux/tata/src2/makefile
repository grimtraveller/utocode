all: Cart

Meal.o: Meal.h Meal.cpp
	g++ -c Meal.cpp

SnackCart.o: Meal.h SnackCart.h SnackCart.cpp
	g++ -c SnackCart.cpp

StreetSnacks.o: Meal.h SnackCart.h StreetSnacks.cpp
	g++ -c StreetSnacks.cpp 

Cart: Meal.o SnackCart.o StreetSnacks.o
	g++ Meal.o SnackCart.o StreetSnacks.o -o LiDun

clean:
	rm *.o LiDun
