P2.out: Proj2app.o SetLimits.o CLife.o
	g++ -g -o P2.out Proj2app.o SetLimits.o CLife.o

Proj2app.o: Proj2app.cpp SetLimits.h CLife.h
	g++ -g -c Proj2app.cpp

SetLimits.o: SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

CLife.o: CLife.cpp CLife.h
	g++ -g -c CLife.cpp

clean:
	rm *.o P2.out

run : P2.out
	clear
	./P2.out

submit : Proj2app.cpp CLife.h CLife.cpp makefile README.txt
	rm -rf tranP2
	mkdir tranP2
	cp Proj2app.cpp tranP2
	cp CLife.h tranP2
	cp CLife.cpp tranP2
	cp makefile tranP2
	cp README.txt tranP2
	tar cfvz tranP2.tgz tranP2
	cp tranP2.tgz ~tiawatts/cs215drop
