CFLAGS	= -g -DDEBUG -Wall
#CFLAGS	= -O2
OBJS	= trio.o strio.o
TARGETLIB	= libtrio.a
LDFLAGS	= -L. -ltrio -lm
DOCS	= \
	man/man3/trio_printf.3 \
	man/man3/trio_scanf.3 \
	man/man3/trio_register.3 \
	man/man3/strio.3

AR	= ar
RANLIB	= ranlib
ERASE	= rm -f
GENDOC	= latex2man

PURIFY = purify
PURIFYOPTIONS = -chain-length=16 -first-only=YES

all: $(TARGETLIB) $(TARGET) regression example userdef

test:
	./regression

pure: $(TOBJS) $(OBJS)
	$(PURIFY) $(PURIFYOPTIONS) $(CC) $(CFLAGS) $^ $(LDFLAGS)

regression: regression.o $(TARGETLIB)
	$(CC) -o regression regression.o $(LDFLAGS)

example: example.o $(TARGETLIB)
	$(CC) -o example example.o $(LDFLAGS)

userdef: userdef.o $(TARGETLIB)
	$(CC) -o userdef userdef.o $(LDFLAGS)

$(TARGETLIB): $(OBJS)
	$(AR) ruv $(TARGETLIB) $(OBJS)
	$(RANLIB) $(TARGETLIB)

man/man3/%.3 : tex/%.tex
	$(GENDOC) $< $@

doc: $(DOCS)

clean:
	$(ERASE) *~ core regression example $(TOBJS) $(OBJS) $(TARGET) $(TARGETLIB) example.o regression.o
