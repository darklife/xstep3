       CC = gcc
   CFLAGS = -Wall -O3 -fomit-frame-pointer -funroll-loops -pipe -I/usr/X11R6/include -I/usr/local/include
   LIBX11 = -L/usr/X11R6/lib -lX11 -lXext
 LIBXSTEP = -L/usr/local/lib -lxstep

all: xstepfiles

xstepfiles:
	$(CC) $(CFLAGS) xstepfiles.c -o xstepfiles $(LIBX11) $(LIBXSTEP)
clean:
	-rm -f xstepfiles
install: xstepfiles
	cp xstepfiles /usr/local/bin
uninstall:
	-rm /usr/local/bin/xstepfiles
