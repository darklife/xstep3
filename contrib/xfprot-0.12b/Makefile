       CC = gcc
   CFLAGS = -Wall -Wshadow -pedantic -O3 -fomit-frame-pointer -funroll-loops -pipe -I/usr/X11R6/include #-I/usr/local/include
   LIBX11 = -L/usr/X11R6/lib -lX11 -lXext
 LIBXSTEP = -L/usr/local/lib -lxstep

all: xfprot

xfprot:
	$(CC) $(CFLAGS) xfprot.c -o xfprot $(LIBX11) $(LIBXSTEP)
	strip xfprot
clean:
	-rm -f xfprot
install:
	mkdir /usr/local/xfprot
	cp xfprot /usr/local/xfprot
	cp scan.sh /usr/local/xfprot
	cp update.sh /usr/local/xfprot
	cp virinfo.sh /usr/local/xfprot
	cp virlist.sh /usr/local/xfprot
	cp virtest.sh /usr/local/xfprot
	cp xfprot /usr/local/xfprot
	cp COPYING /usr/local/xfprot
	cp README /usr/local/xfprot
	ln -s /usr/local/xfprot/xfprot /usr/bin/xfprot
uninstall:
	-rm /usr/local/xfprot/xfprot
	-rm /usr/local/xfprot/scan.sh
	-rm /usr/local/xfprot/update.sh
	-rm /usr/local/xfprot/virinfo.sh
	-rm /usr/local/xfprot/virlist.sh
	-rm /usr/local/xfprot/virtest.sh
	-rm /usr/local/xfprot/COPYING
	-rm /usr/local/xfprot/README
	-rm /usr/bin/xfprot
	-rmdir /usr/local/xfprot
