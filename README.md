# minimum-ot-editor

A minimum editor implemented with [Operational Transformation](https://operational-transformation.github.io).

![Preview Image](#)

## Features

- Editor
  - [x] ASCII
  - [ ] Unicode
  - [x] Line break
  - [x] Word wrap
  - [x] Move by arrow keys
  - [x] Beep alert for invalid operation (ex: Move cursor out of range)
  - [x] Cursor position highlight of yours
  - [ ] Cursor position highlight of other clients
  - [ ] Copy/Paste
- Operational Transformation
  - [x] Multiple client connections
  - [ ] Insert single charactor (ex: Insert `'c'` by pressing C key)
  - [x] Delete single charactor
  - [ ] Insert multiple charactors (ex: Insert `"str"` by pasting)
  - [ ] Delete multiple charactors
  - [ ] Undo/Redo

## Usage

```console
$ # 1. Build server and client.
$ make

$ # 2. Start server for accepting clients
$ ./bin/server <addr> <port>

$ # 3. Start client and enjoy editting!
$ ./bin/client <addr> <port>
```

### Available Arguments

- `addr`: A server's address to listen
- `port`: A server's port number to listen

### Dependencies

- **clang (required)**
- clangd (optional / for development)
- clang-format (optional / for development)
- VSCode (optional / for development)

## Development

- `make`: Build server and clinent
- `make server`: Build server
- `make client`: Build client
- `make generate`: Generate `compile_commands.json` for clangd
- `make clean`: Cleanup project
