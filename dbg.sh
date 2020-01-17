g++ -g -c -o main.o main.cpp
g++ -g -c -o util.o util.cpp
g++ -g -c -o fr.o fr.cpp
g++ -g -c -o linkedlist.o linkedlist.cpp

gcc -o hwr main.o util.o fr.o linkedlist.o
