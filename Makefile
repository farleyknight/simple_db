build:
	g++ main.cpp --std=c++11
	./a.out
build_parser:
	g++ -I /usr/local/Cellar/boost/1.64.0_1/include sql_parser.cpp --std=c++11
	./a.out
