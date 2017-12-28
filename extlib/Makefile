INSTALLDIR=$(shell pwd)/install

LIBEVENT=libevent-2.1.8-stable
LUA51=lua-5.1.5

all: lua libevent

lua:
	(cd $(LUA51) && make linux && make install INSTALL_TOP=$(INSTALLDIR)/lua51)

libevent:
	(cd $(LIBEVENT) && ./configure --prefix=$(INSTALLDIR)/libevent && make install)

clean:
	(cd install && rm -rf *)
	(cd $(LUA51) && make clean)
	(cd $(LIBEVENT) && make clean)
