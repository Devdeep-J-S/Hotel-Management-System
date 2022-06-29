# you can put include files in this directory
IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -pthread 

# directory where object files will be created
ODIR=obj
# you can put library files in this directory
LDIR =../lib

# create string with ../include/<header filename> in our case ../include/myheader.h
_DEPS = header.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# create string with obj/<object filename>  (obj folder will be created under current i.e. source directory)
_LIBOBJ =  assure.o  customer.o waiter.o chef.o
LIBOBJ = $(patsubst %,$(ODIR)/%,$(_LIBOBJ))
LIBSRC = assure.c customer.c waiter.c chef.c 
_APPOBJ = main.o
APPOBJ = $(patsubst %,$(ODIR)/%,$(_APPOBJ))

$(ODIR)/assure.o: assure.c 
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

$(ODIR)/chef.o: chef.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)
	
$(ODIR)/waiter.o: waiter.c 
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

$(ODIR)/customer.o: customer.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

	
$(LDIR)/libourdynamic.so:$(LIBOBJ)
	$(CC) -shared -o $@ $^
	
$(APPOBJ): main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
main.out: $(APPOBJ) $(LDIR)/libourdynamic.so
	$(CC) -o $@ $^ $(CFLAGS) $(LDIR)/libourdynamic.so
	./main.out
.PHONY: clean

# removes object files and temporary files created with ~ at the end of file name in current i.e. source and include directory
clean: 
	
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f main.out $(LDIR)/libourdynamic.so
	
.PHONY: all
all: $(LDIR)/libourdynamic.so main.out

