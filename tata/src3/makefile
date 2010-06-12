all: Eatery

Meal.o: Meal.h Meal.cpp
	g++ -c Meal.cpp
Eatery.o: Meal.h Eatery.h Eatery.cpp
	g++ -c Eatery.cpp
SnackCart.o: Meal.h SnackCart.h SnackCart.cpp
	g++ -c SnackCart.cpp
Restaurant.o: Meal.h Restaurant.h Restaurant.cpp
	g++ -c Restaurant.cpp
EateryGuide.o: Meal.h Eatery.h Restaurant.h SnackCart.h EateryGuide.cpp
	g++ -c EateryGuide.cpp 

Eatery: Meal.o Eatery.o Restaurant.o SnackCart.o EateryGuide.o
	g++ Meal.o Eatery.o Restaurant.o SnackCart.o EateryGuide.o -o LiDun

clean:
	rm *.o LiDun
