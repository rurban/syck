.PHONY: build
build: clean
	ruby install.rb config
	ruby install.rb setup
clean:
	-rm config.save
	cd ext/syck && \
	for f in *; do \
	  if grep -q "$$f" MANIFEST; then true; else rm "$$f"; fi; \
	done
