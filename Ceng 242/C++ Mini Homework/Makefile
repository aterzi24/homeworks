test1:
	g++ -std=c++11 TestGroup1.cpp Book.cpp HardCopy.cpp SoftCopy.cpp -o test
test2: 
	g++ -std=c++11 TestGroup2.cpp Book.cpp  SoftCopy.cpp -o test
test3: 
	g++ -std=c++11 TestGroup3.cpp Book.cpp  SoftCopy.cpp -o test
test4: 
	g++ -std=c++11 TestGroup4.cpp Book.cpp  SoftCopy.cpp -o test
test5: 
	g++ -std=c++11 TestGroup5.cpp Book.cpp  SoftCopy.cpp -o test
run:
	./test
valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
