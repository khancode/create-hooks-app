.PHONY: install build

HOOK_C_FILENAME := $(shell jq -r '.HOOK_C_FILENAME' config.json)

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
	wasmcc ./hook-src/$(HOOK_C_FILENAME).c -o ./build/$(HOOK_C_FILENAME).wasm  -O0 -Wl,--allow-undefined -I../
	./binaryen/bin/wasm-opt -O2 ./build/$(HOOK_C_FILENAME).wasm -o ./build/$(HOOK_C_FILENAME).wasm
	./hook-cleaner-c/hook-cleaner ./build/$(HOOK_C_FILENAME).wasm
	./xrpld-hooks/src/ripple/app/hook/guard_checker ./build/$(HOOK_C_FILENAME).wasm

set-hook:
	node ./client/set-hook.js

clean:
	rm -rf build/*
