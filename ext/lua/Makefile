# change these to reflect your Lua installation
# Default installation prefix
PREFIX := $(shell pkg-config --prefix lua 2>/dev/null || echo /usr/local)
LUAVER := $(shell pkg-config --variable=V lua 2>/dev/null)
ifeq ($(LUAVER),)
  LUAVER := $(shell pkg-config --variable=major_version lua 2>/dev/null || echo 5.4)
endif
LUAINC := $(shell pkg-config --variable=includedir lua 2>/dev/null || echo $(PREFIX)/include)
LUABIN = $(PREFIX)/bin
ifneq ($(wildcard $(PREFIX)/lib64/lua/.),)
  LUALIB := $(PREFIX)/lib64
else
  LUALIB := $(shell pkg-config --variable=libdir lua 2>/dev/null || echo $(PREFIX)/lib)
endif

# System's libraries directory (where binary libraries are installed)
LUA_LIBDIR = $(LUALIB)/lua/$(LUAVER)
# System's lua directory (where Lua libraries are installed)
LUA_DIR = $(PREFIX)/share/lua/$(LUAVER)

WARN = -Wall
INCS = -I$(LUAINC)
CFLAGS := -g -fPIC $(shell pkg-config --cflags lua 2>/dev/null) $(WARN)
# Better get the @libdir@ from syck
LIBSYCK = -L/usr/local/lib -Wl,-rpath /usr/local/lib -lsyck
LIBS := $(shell pkg-config --libs lua 2>/dev/null) $(LIBSYCK)

MYNAME = syck
OBJS = $(MYNAME).o
T = $(MYNAME).so

all:	$(T)

install: $(T)
	-mkdir -p $(LUA_LIBDIR) $(LUA_DIR)
	cp -f $(T) $(LUA_LIBDIR)/
	cp -f yaml.lua $(LUA_DIR)/

uninstall:
	rm -f $(LUA_DIR)/yaml.lua
	rm -f $(LUA_LIBDIR)/$(T)

test: $(T)
	lua test.lua
	@echo "built and tested successfully. run make install to install, or just move the libs manually"

$(T):	$(OBJS)
	$(CC) -o $@ -shared -fPIC $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $T core core.* a.out test.dump

ready:
