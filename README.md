# minimum-ot-editor

A minimum editor implemented with [Operational Transformation](https://operational-transformation.github.io).

![Preview Image](#)

## Features

- Editor
  - :white_check_mark: Line break
  - :white_check_mark: Word wrap
  - :white_check_mark: Move by arrow keys
  - :white_check_mark: Beep alert for invalid operation (ex: Move cursor out of range)
  - :white_check_mark: Cursor position highlight of yours
  - :heavy_multiplication_x: Cursor position highlight of other clients
  - :heavy_multiplication_x: Copy/Paste
- Operational Transformation
  - :white_check_mark: Insert single charactor (ex: Insert `'c'` by pressing C key)
  - :heavy_multiplication_x: Insert multiple charactors (ex: Insert `"str"` by pasting)
  - :white_check_mark: Delete single charactor
  - :heavy_multiplication_x: Delete multiple charactors
  - :heavy_multiplication_x: Undo/Redo
- Characters
  - :white_check_mark: ASCII
  - :heavy_multiplication_x: Unicode
- Network
  - :white_check_mark: Multiple client connections
  - :heavy_multiplication_x: Connection reestablishment

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
