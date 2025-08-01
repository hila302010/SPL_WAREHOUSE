all : clean run 

run: bin/main.o bin/WareHouse.o bin/Volunteer.o bin/Order.o bin/Action.o bin/Customer.o
	@echo 'Building Target: main'
	g++ -o ./bin/run ./bin/main.o ./bin/WareHouse.o ./bin/Volunteer.o ./bin/Order.o ./bin/Action.o ./bin/Customer.o

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/main.cpp -o ./bin/main.o

bin/WareHouse.o: src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/WareHouse.cpp -o ./bin/WareHouse.o

bin/Volunteer.o: src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/Volunteer.cpp -o ./bin/Volunteer.o

bin/Order.o: src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/Order.cpp -o ./bin/Order.o

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/Action.cpp -o ./bin/Action.o


bin/Customer.o:src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c ./src/Customer.cpp -o ./bin/Customer.o


clean:
	rm -f bin/*
