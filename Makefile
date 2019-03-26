CC = clang
# C標準と矛盾する拡張機能の使用をエラーにする
# ref: https://qnighy.hatenablog.com/entry/2015/09/26/192104
CWFLAGS = -Wall -Wextra -Werror -pedantic-errors -Wno-pragma-once-outside-header
CFLAGS = $(CWFLAGS) -std=gnu11 -g -I src

SRCDIR := ./src
OBJDIR := ./obj
BINDIR := ./bin

LIB_SRCS := $(shell find $(SRCDIR) -name '*.c' -not -iwholename '$(SRCDIR)/bin/*')
LIB_OBJS := $(LIB_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LIB_DEPS := $(LIB_OBJS:%.o=%.d)

BIN_SRCS := $(shell find $(SRCDIR)/bin -name '*.c')
BIN_OBJS := $(BIN_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BIN_DEPS := $(BIN_OBJS:%.o=%.d)


.PHONY: all clean generate

all: server client

clean:
	rm -rf $(OBJDIR) $(BINDIR)

generate: $(LIB_OBJS) $(BIN_OBJS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $(OBJDIR)/**/*.o.json > compile_commands.json

# 実行可能ファイル以外の全てをコンパイル
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -MJ $@.json -MMD $< -o $@

# 実行可能ファイルをコンパイル
server: $(OBJDIR)/bin/server.o $(LIB_OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

client: $(OBJDIR)/bin/client.o $(LIB_OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

# .d から依存関係を読み込む
-include $(LIB_DEPS) $(BIN_DEPS)
