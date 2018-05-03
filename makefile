CC      = g++
CFLAGS  = -ansi -pedantic -Wall -O2
SOURCES = $(shell find -maxdepth 1 -type f -name "*.cpp")
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
DPFILES = $(patsubst %.cpp,%.d,$(SOURCES))
PROJECT = cout

.PHONY: all clean

# Default-Target
all: $(PROJECT)

# Abhaengigkeiten der Objekt-Dateien importieren
-include $(DPFILES)

# Abhaengigkeiten der Objekt-Dateien generieren
$(DPFILES): %.d: %.cpp
	$(CC) -MM -o $@ $<

# Objekt-Dateien linken
$(PROJECT): $(OBJECTS)
	$(CC) -o $(PROJECT) $+
	
# Objekt-Dateien kompilieren
$(OBJECTS): %.o: %.cpp %.d
	$(CC) -c $(CFLAGS) -o $@ $<

# Dateien loeschen, die auch erzeugt werden koennen
clean:
	@rm $(OBJECTS) $(DPFILES) $(PROJECT)
