# minimum-ot-editor

A minimum editor implemented with [Operational Transformation](https://operational-transformation.github.io).

- Preview: [Youtube](https://www.youtube.com/watch?v=xDPGVx-9nC0)

## Features

- Editor
  - ⭕ Line break
  - ⭕ Word wrap
  - ⭕ Move by arrow keys
  - ⭕ Beep alert for invalid operation (ex: Move cursor out of range)
  - ⭕ Cursor position highlight of yours
  - ✖ Cursor position highlight of other clients
  - ✖ Copy/Paste
- Operational Transformation
  - ⭕ Insert single charactor (ex: Insert `'c'` by pressing C key)
  - ✖ Insert multiple charactors (ex: Insert `"str"` by pasting)
  - ⭕ Delete single charactor
  - ✖ Delete multiple charactors
  - ✖ Undo/Redo
- Characters
  - ⭕ ASCII
  - ✖ Unicode
- Network
  - ⭕ Multiple client connections
  - ✖ Connection reestablishment

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
