ifeq ($(OS),Windows_NT)
    CC = x86_64-w64-mingw32-gcc
    TARGET = Emul_FPN.exe
    RM = del /Q
    FIXPATH = $(subst /,\,$1)
else
    CC = gcc
    TARGET = Emul_FPN
    RM = rm -f
    FIXPATH = $1
endif

CFLAGS = -Iheaders
LDFLAGS = -lm
SRC_DIR = source
HEADERS_DIR = headers

ifeq ($(OS),Windows_NT)
    SRC_FILES = $(subst /,\,$(wildcard $(SRC_DIR)/*.c))
else
    SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
endif

OBJS = $(SRC_FILES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(call FIXPATH,$^) $(LDFLAGS) -o $(call FIXPATH,$@)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(call FIXPATH,$(RM) $(TARGET) $(OBJS))

cleanse: 
	$(call FIXPATH,$(RM) $(OBJS))

.PHONY: all clean