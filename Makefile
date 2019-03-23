CC = clang
# C標準と矛盾する拡張機能の使用をエラーにする
# ref: https://qnighy.hatenablog.com/entry/2015/09/26/192104
CWFLAGS = -Wall -Wextra -Werror -pedantic-errors
CFLAGS = $(CWFLAGS) -std=gnu11 -g -I src

SRCDIR := ./src
OBJDIR := ./obj
BINDIR := ./bin
SRCS   := $(shell find $(SRCDIR)/{common,ot,editor} -name *.c)
OBJS   := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

.PHONY: all clean generate

all: server client

clean:
	rm -rf $(OBJDIR) $(BINDIR)

generate: server client
	sed -e '1s/^/[/' -e '$$s/,$$/]/' obj/**/*.o.json > compile_commands.json

# 実行可能ファイル以外の全てをコンパイル
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -MJ $@.json -MMD $< -o $@

# 実行可能ファイルをコンパイル
server: $(SRCDIR)/bin/server.c $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

client: $(SRCDIR)/bin/client.c $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

# .d から依存関係を読み込む
-include $(DEPS)
