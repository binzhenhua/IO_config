LIBNAME = IOConfig
OBJECTS = ${LIBNAME}.o
#LIBHEADER = ${LIBNAME}.h
STATICLIB  = lib${LIBNAME}.a

LIBPATH = ./lib

all: $(STATICLIB)

CFLAGS = -g -Wall -fPIC
CFLAGS += $(CFLAG)

CXX = g++
CC = gcc

# How to make an object file
%.o: %.cpp %.h
#	@echo "=*=  making object $@ =*="
	${CXX} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o *~ $(LIBPATH)/$(STATICLIB)

${STATICLIB}: ${OBJECTS}
	@echo "=*= making static library $@ =*="
	@rm -f $(LIBPATH)/$(STATICLIB)
	@ar q $@ ${OBJECTS}
	mv $(STATICLIB) $(LIBPATH)
	rm $(OBJECTS)
    #ranlib $(STATICLIB)

#install:
#	cp $(STATICLIB) $(LIBHEADER) $(LIBPATH)

strip:
	strip $(STATICLIB)
