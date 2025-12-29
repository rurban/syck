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
	$(MAKE) -C ext/php
	$(MAKE) -C ext/yamlbyte
	$(MAKE) -C ext/lua

ext-test:
	$(MAKE) -C ext/ruby test
	$(MAKE) -C ext/php test
	$(MAKE) -C ext/yamlbyte test
	$(MAKE) -C ext/lua test

ext-clean:
	$(MAKE) -C ext/ruby clean
	$(MAKE) -C ext/php  clean
	$(MAKE) -C ext/yamlbyte  clean
	$(MAKE) -C ext/lua  clean
