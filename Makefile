CFLAGS	= -g -DDEBUG -Wall
#CFLAGS	= -O2
OBJS	= triostr.o trio.o trionan.o
TARGETLIB	= libtrio.a
LDFLAGS	= -L. -ltrio -lm
AR	= ar
RANLIB	= ranlib
ERASE	= rm -f
GENDOC	= doxygen

PURIFY = purify
PURIFYOPTIONS = -chain-length=16 -first-only=YES

all: $(TARGETLIB) $(TARGET) regression example

test:
	./regression

pure: $(TOBJS) $(OBJS)
	$(PURIFY) $(PURIFYOPTIONS) $(CC) $(CFLAGS) $^ $(LDFLAGS)

regression: regression.o $(TARGETLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

example: example.o $(TARGETLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

compare: compare.o $(TARGETLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

userdef: userdef.o $(TARGETLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGETLIB): $(OBJS)
	$(AR) ruv $(TARGETLIB) $(OBJS)
	$(RANLIB) $(TARGETLIB)

doc::
	$(GENDOC) doc/trio.cfg

clean:
	$(ERASE) *~ core regression example $(TOBJS) $(OBJS) $(TARGET) $(TARGETLIB) example.o regression.o
