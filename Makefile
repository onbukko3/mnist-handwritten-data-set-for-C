CC = gcc
TARGET = hwr
OBJS = main.o util.o fr.o linkedlist.o bmpinfo.o

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ 

 : $(OBJS)
	$(CC) -c $< -o $@

clean : 
	rm *.o hwr 



