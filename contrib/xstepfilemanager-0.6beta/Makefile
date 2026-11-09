       CC = gcc
   CFLAGS = -Wall -pedantic -O3 -fomit-frame-pointer -funroll-loops -pipe -I/usr/X11R6/include #-I/usr/local/include
   LIBX11 = -L/usr/X11R6/lib -lX11 -lXext
 LIBXSTEP = -L/usr/local/lib -lxstep

all: xstepfilemanager

xstepfilemanager:
	$(CC) $(CFLAGS) xstepfilemanager.c -o xstepfilemanager $(LIBX11) $(LIBXSTEP)
	strip xstepfilemanager
	-ln -s xstepfilemanager xrun
	-ln -s xstepfilemanager xviewer
clean:
	-rm -f xstepfilemanager
	-rm -f xstepfilemanager xrun
	-rm -f xstepfilemanager xviewer
install: xstepfilemanager
	cp xstepfilemanager /usr/local/bin
uninstall:
	-rm -f /usr/local/bin/xstepfilemanager

