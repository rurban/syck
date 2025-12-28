VER=1.5.1.2
.PHONY: build test clean

build: pkg/syck-$(VER).gem
lib/syck.so: ext/syck/extconf.rb ext/syck/rubyext.c \
	../../lib/syck.c ../../lib/syck.h
pkg/syck-$(VER).gem: syck.gemspec lib/syck.so
	make clean
	rake compile
	rake build
	rake install
test: build
	rake test
clean:
	rake clean
	cd ext/syck && \
	rm lib/syck.so 2>/dev/null; \
	for f in *; do \
	  if grep -q "$$f" MANIFEST; then true; else rm -f "$$f"; fi; \
	done
