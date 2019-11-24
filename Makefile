#makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -g

rectangle_search: rectangle_search.o
	$(CXX) $(CXXFLAGS) -o rectangle_search rectangle_search.o

clean:
	rm rectangle_search rectangle_search.o
