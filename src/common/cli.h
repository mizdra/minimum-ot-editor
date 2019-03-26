#pragma once

#include <arpa/inet.h>

// コマンドライン引数をパースする.
// 引数は以下の形式で渡される.
// ./server <addr> <port>
// ./client <addr> <port>
void parse_args(int argc, char** argv, in_addr_t* addr, in_port_t* port);
