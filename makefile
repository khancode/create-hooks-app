.PHONY: install build

setup: install update-definitions

build-set-hook: build set-hook

install:
	npm i
	./install.sh
	cp config-example.json config.json

update-definitions:
	node update-definitions.js

build:
	mkdir -p build
	wasmcc ./hook-src/starter.c -o ./build/starter.wasm  -O0 -Wl,--allow-undefined -I../
	./binaryen/bin/wasm-opt -O2 ./build/starter.wasm -o ./build/starter.wasm
	./hook-cleaner-c/hook-cleaner ./build/starter.wasm
	./xrpld-hooks/src/ripple/app/hook/guard_checker ./build/starter.wasm

set-hook:
	node ./client/set-hook.js

clean:
	rm -rf build/*
