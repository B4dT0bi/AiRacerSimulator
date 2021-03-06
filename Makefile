SYSCONF_LINK = g++
CPPFLAGS     = -g 
LDFLAGS      = -Wall -L/usr/lib/x86_64-linux-gnu/  
LIBS         = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system


DESTDIR = ./
TARGET  = carRacing

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)


