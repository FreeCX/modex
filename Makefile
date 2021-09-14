CXX := g++
FLAGS := -Wall -Werror

all: lib app

lib:
	${CXX} ${FLAGS} -fpic -c src/math.cpp src/foo.cpp src/module.cpp
	${CXX} ${FLAGS} -shared -o libmath.so math.o module.o
	${CXX} ${FLAGS} -shared -o libfoo.so foo.o module.o
	mkdir -p modules
	mv lib*.so ./modules/

app:
	${CXX} ${FLAGS} -c src/app.cpp src/module.cpp
	${CXX} ${FLAGS} -ldl app.o module.o -o app

clean:
	${RM} app ./modules/*.so *.o
