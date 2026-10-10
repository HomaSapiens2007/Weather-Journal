CC = g++
TARGET = WeatherJournal
OBJS = main.o WeatherJournal.o

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) -lm
main.o: main.cpp WeatherJournal.h 
		$(CC) -c main.cpp
WeatherJournal.o: WeatherJournal.cpp WeatherJournal.h
		$(CC) -c WeatherJournal.cpp
clean:
	rm -f $(OBJS) $(TARGET)
