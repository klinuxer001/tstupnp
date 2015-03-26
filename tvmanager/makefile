
TOPDIR=..
INCLUDE=-I$(TOPDIR)/upnp/inc -I$(TOPDIR)/ixml/inc 

LDS=-lupnp -pthread -lthreadutil -lixml

CFLAGS=-DUPNP_HAVE_DEBUG -DUPNP_HAVE_CLIENT -DUPNP_HAVE_DEVICE
CFLAGS+=-DUPNP_HAVE_WEBSERVER -DUPNP_HAVE_TOOLS
CFLAGS+=-DMAIN_TEST

SRCS=tvmanager_tvctl.c debug.c

all:tvmanager_tvctl FORCE

%.o:%.c
	gcc -c $< $(CFLAGS) $(INCLUDE)

OBJS=tvmanager_tvctl.o debug.o

tvmanager_tvctl: $(OBJS)
	gcc -g -o $@ $^ $(INCLUDE) $(LDS) 

clean:
	rm -f tvmanager_tvctl $(OBJS)

FORCE:

