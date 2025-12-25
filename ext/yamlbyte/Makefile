.PHONEY: all test check
LIBTOOL = ../../libtool
INCDIR = ../../lib
LIBDIR = ../../lib

all: ybext
ybext: ybext.c $(LIBDIR)/libsyck.la
	$(LIBTOOL) --mode=compile gcc -c -fPIC -DTEST_YBEXT -I$(INCDIR) ybext.c -o ybext.lo
	$(LIBTOOL) --mode=link gcc ybext.lo $(LIBDIR)/libsyck.la -o ybext
test check: ybext.c $(LIBDIR)/libsyck.la
	$(LIBTOOL) --mode=execute ./ybext > ybext.trs
	cmp ybext.exp ybext.trs
clean:
	-rm -rf .libs
	-rm -f ybext.lo ybext.o ybext ybext.trs
