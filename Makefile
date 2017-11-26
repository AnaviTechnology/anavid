TARGET   = anavid

CC       = gcc
CFLAGS   = -std=gnu99 -Wall -I.

LINKER   = gcc -o
LFLAGS   = -Wall -I. -lm -lwiringPi -lwiringPiDev -lpaho-mqtt3c -lpthread

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

prefix	 = /usr/local

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p ${BINDIR}
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p ${OBJDIR}
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

install: $(BINDIR)/$(TARGET)
	install -m 0755 $(BINDIR)/$(TARGET) $(prefix)/bin

.PHONY: install
