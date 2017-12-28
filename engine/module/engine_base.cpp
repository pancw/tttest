#include "engine_base.h"

struct event_base* base;
struct event *signal_int;
struct event *timeout;

extern lua_State *GlobalL;

void lua_shutdown()
{
	int top = lua_gettop(GlobalL);
	lua_pushcclosure(GlobalL, error_fun, 0); 
	lua_getglobal(GlobalL, "ShutDown");
	int result = lua_pcall(GlobalL, 0, 0, -2);
	if (result) {
		printf("[lua-call(%d)]: %s\n", 1, lua_tostring(GlobalL, -1));
	}   
	lua_settop(GlobalL, top);
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{   
	int top = lua_gettop(GlobalL);
	lua_pushcclosure(GlobalL, error_fun, 0); 
	lua_getglobal(GlobalL, "Tick");
	int result = lua_pcall(GlobalL, 0, 0, -2);
	if (result) {
		printf("[lua-call(%d)]: %s\n", 1, lua_tostring(GlobalL, -1));
	}   
	lua_settop(GlobalL, top);
}   

static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event *signal = (struct event*)arg;
	printf("signal_cb: got signal %d\n", event_get_signal(signal));

	event_del(signal);
	lua_shutdown();	
	event_base_loopbreak(base);
	event_free(signal_int);
	event_free(timeout);
	event_base_free(base);

	printf("free base.\n");
}

void engine_base_init()
{
	// struct event timeout;
	struct timeval tv;
	int flags;

	/* Initalize the event library */
	base = event_base_new();

	/* Initalize one event */
	signal_int = evsignal_new(base, SIGINT, signal_cb, event_self_cbarg());
	event_add(signal_int, NULL);

	timeout = event_new(base, -1, EV_PERSIST, timeout_cb, (void*) timeout);
	// event_assign(&timeout, base, -1, EV_PERSIST, timeout_cb, (void*) timeout);
	evutil_timerclear(&tv);
	tv.tv_sec = 1;
	event_add(timeout, &tv);
}

void engine_loop()
{
	event_base_dispatch(base);
}
