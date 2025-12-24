LIBTOOL = ../../libtool
INCDIR = ../../lib
LIBDIR = ../../lib

all: ybext
ybext: ybext.c $(LIBDIR)/libsyck.la
	$(LIBTOOL) --mode=compile gcc -c -fPIC -DTEST_YBEXT -I$(INCDIR) ybext.c -o ybext.lo
	$(LIBTOOL) --mode=link gcc ybext.lo $(LIBDIR)/libsyck.la -o ybext
	$(LIBTOOL) --mode=execute ./ybext
