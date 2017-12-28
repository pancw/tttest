MACHINE = $(shell uname -m)
ifeq ($(MACHINE), x86_64)
MBITS = 64
else
MBITS = 32
endif

INSTALLDIR=$(shell pwd)/../extlib/install
ROOT=$(shell pwd)

BIN=game_server

DEPS=$(shell find ./ -name "*.h")
C_SRC=$(shell find ./ -name "*.c")
CPP_SRC=$(shell find ./ -name "*.cpp")
OBJ=$(C_SRC:%.c=%.o) $(CPP_SRC:%.cpp=%.o)

INCLUDE= \
	-I $(ROOT)/include \
	-I $(INSTALLDIR)/lua51/include \
	-I $(INSTALLDIR)/libevent/include

LIB= \
	$(INSTALLDIR)/lua51/lib/liblua.a \
    $(INSTALLDIR)/libevent/lib/libevent.a \
    $(INSTALLDIR)/libevent/lib/libevent_core.a \
    $(INSTALLDIR)/libevent/lib/libevent_extra.a \
    $(INSTALLDIR)/libevent/lib/libevent_pthreads.a
		

$(BIN): $(OBJ) $(LIB)
	g++ -std=c++11 -m$(MBITS) -o $@ $^ $(LIB) -ldl

%.o: %.c $(DEPS)
	gcc -m$(MBITS) -c $< -o $@ $(INCLUDE)

%.o: %.cpp $(DEPS)
	g++ -std=c++11 -m$(MBITS) -c $< -o $@ $(INCLUDE)

clean:
	rm -rf $(OBJ) $(BIN)
