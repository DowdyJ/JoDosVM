CC = g++
CFLAGS = -g -Wall -std=c++11


TARGET = assembler.pls
TARGET_LC3 = lc3.pls


clean:
	-rm Assembler/*.o 
	-rm LC3/*.o
	-rm $(TARGET)
	-rm $(TARGET_LC3)

all: $(TARGET) $(TARGET_LC3)

assembler: $(TARGET)

lc3: $(TARGET_LC3)

$(TARGET): Assembler/Source.o Assembler/Assembler.o Assembler/ConsoleLogger.o Assembler/Utilities.o
	$(CC) $(CFLAGS) -o $(TARGET) Assembler/Source.o Assembler/Assembler.o Assembler/ConsoleLogger.o Assembler/Utilities.o

Assembler/Source.o: Assembler/Source.cpp Assembler/Assembler.h Assembler/ConsoleLogger.h Assembler/Utilities.h
	$(CC) $(CFLAGS) -o Assembler/Source.o -c Assembler/Source.cpp

Assembler/Assembler.o: Assembler/Assembler.cpp Assembler/Assembler.h Assembler/ConsoleLogger.h Assembler/Utilities.h
	$(CC) $(CFLAGS) -o Assembler/Assembler.o -c Assembler/Assembler.cpp

Assembler/ConsoleLogger.o: Assembler/ConsoleLogger.h Assembler/ConsoleLogger.cpp Assembler/Logger.h
	$(CC) $(CFLAGS) -o Assembler/ConsoleLogger.o -c Assembler/ConsoleLogger.cpp

Assembler/Utilities.o: Assembler/Utilities.cpp Assembler/Utilities.h
	$(CC) $(CFLAGS) -o Assembler/Utilities.o -c Assembler/Utilities.cpp


$(TARGET_LC3): LC3/Source.o LC3/Utilities.o LC3/Register.o LC3/ExternalUtilities.o
	$(CC) $(CFLAGS) -o $(TARGET_LC3) LC3/Source.o LC3/Utilities.o LC3/Register.o LC3/ExternalUtilities.o

LC3/Source.o: LC3/Source.cpp LC3/Utilities.h LC3/Register.h LC3/ExternalUtilities.h
	$(CC) $(CFLAGS) -o LC3/Source.o -c LC3/Source.cpp

LC3/Utilities.o: LC3/Utilities.cpp LC3/Utilities.h
	$(CC) $(CFLAGS) -o LC3/Utilities.o -c LC3/Utilities.cpp

LC3/Register.o: LC3/Register.cpp LC3/Register.h LC3/ExternalUtilities.h
	$(CC) $(CFLAGS) -o LC3/Register.o -c LC3/Register.cpp

LC3/ExternalUtilities.o: LC3/ExternalUtilities.cpp LC3/ExternalUtilities.h
	$(CC) $(CFLAGS) -o LC3/ExternalUtilities.o -c LC3/ExternalUtilities.cpp
