#
# I feel like saying, "The magic happens here!"  But it doesn't.
#
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = lib tests

EXTRA_DIST = m4/ax_append_compile_flags.m4 \
	m4/ax_append_flag.m4 \
	m4/ax_check_compile_flag.m4 \
	m4/ax_gcc_func_attribute.m4 \
	m4/ax_require_defined.m4
dist_doc_DATA = README README.BYTECODE README.EXT ChangeLog TODO RELEASE AUTHORS

.PHONY: ext ext-test ext-clean
ext:
	$(MAKE) -C ext/ruby
	cd ext/php && phpize && ./configure --with-syck=$(prefix) && make
	$(MAKE) -C ext/yamlbyte
	$(MAKE) -C ext/lua
	cd ext/perl && perl Makefile.PL && make

ext-test:
	$(MAKE) -C ext/ruby test
	$(MAKE) -C ext/php test
	$(MAKE) -C ext/yamlbyte test
	$(MAKE) -C ext/lua test
	$(MAKE) -C ext/perl test

ext-clean:
	$(MAKE) -C ext/ruby clean
	$(MAKE) -C ext/php  clean
	$(MAKE) -C ext/yamlbyte  clean
	$(MAKE) -C ext/lua  clean
	-$(MAKE) -C ext/perl  clean;
	$(MAKE) -C ext/perl  clean;
