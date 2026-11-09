#
#  680x0 Emulator (c) 1999,2001 by Marcelo Samsoniuk
# 
	    CC = gcc
            LD = ld
 	 CCOPS = -Wall -O6 -pipe -fomit-frame-pointer -funroll-loops -mpentium 
        INCDIR = -I/usr/local/include -I/usr/X11R6/lib
        LIBDIR = -L/usr/local/lib -L/usr/X11R6/lib
#         DEFS = -DDEBUG
          LIBS = -lX11 -lxstep
        CFLAGS = $(DEFS) $(INCDIR)
       LDFLAGS = $(LIBDIR) $(LIBS)
          OBJS = main.o address.o endian.o peek.o poke.o ucode.o util.o dump.o
        TARGET = xstep68k
      BINSTALL = install -o root -g root -m 755 -s
        PREFIX = /usr/local

all:: $(TARGET)

$(TARGET):: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

install: all
	$(BINSTALL) $(TARGET) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(TARGET)
 
clean:
	$(RM) $(TARGET) $(OBJS) core
