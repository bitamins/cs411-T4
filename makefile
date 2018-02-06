CC=g++

make: front.cpp
	$(CC) -o test front.cpp -I/usr/include/mysql++ -I/usr/include/mysql -lmysqlpp 
