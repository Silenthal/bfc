TARGET := bf
CC := gcc
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=obj/%.o)

ifdef SystemRoot
    PLATFORM := WIN32
    RMDIR := rmdir /s /q
    RMFILE := del
    MKDIR := mkdir
    EXT := .exe
else
    PLATFORM := UNIX
    RMDIR := rm -rf
    RMFILE := rm -f
    MKDIR := mkdir -p
endif

CFLAGS += -D$(PLATFORM)
CFLAGS += -g3 -O0 -Werror -MMD -pedantic -Wall -Wextra -Wno-missing-field-initializers

.PHONY : all clean

all : dirmake $(TARGET) 

clean :
	-$(RMDIR) obj
	-$(RMFILE) $(TARGET)$(EXT)

ifneq ($(MAKECMDGOALS), clean)
    -include $(OBJS:.o=.d)
endif

dirmake :
	-$(MKDIR) obj

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(ALL_LDFLAGS) $^ $(LDLIBS)

obj/%.o : src/%.c
	$(CC) -c -o $@ $(ALL_CFLAGS) $<