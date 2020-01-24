g++ -g -c -o main.o main.cpp
g++ -g -c -o util.o util.cpp
g++ -g -c -o fr.o fr.cpp
g++ -g -c -o linkedlist.o linkedlist.cpp
g++ -g -c -o bmpinfo.o bmpinfo.cpp

gcc -o hwr main.o util.o fr.o linkedlist.o bmpinfo.o
