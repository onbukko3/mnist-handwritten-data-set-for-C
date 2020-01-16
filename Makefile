CC = gcc
TARGET = hwr
#THREAD = -lpthread -D _USE_THREAD
OBJECTS = util.o fr.o linkedlist.o main.o 

#$(TARGET) : $(OBJECTS)
	#$(CC) -o $@ $^ $(THREAD) 

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^

 : $(OBJECTS)
	$(CC) -c -g -o $@ $<

clean : 
	rm *.o hwr 



