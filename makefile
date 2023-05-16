.PHONY: install build-hooks build-one set-hooks

setup: install update-definitions

build-set-hooks: build-hooks set-hooks

install:
	npm i
	./install.sh
	cp config-example.json config.json

update-definitions:
	node updateDefinitions.js

build-hooks:
	mkdir -p build
	./build_hooks.sh

build-one-hook:
	wasmcc ./hook-src/$(HOOK_C_FILENAME).c -o ./build/$(HOOK_C_FILENAME).wasm -O0 -Wl,--allow-undefined -I../
	./binaryen/bin/wasm-opt -O2 ./build/$(HOOK_C_FILENAME).wasm -o ./build/$(HOOK_C_FILENAME).wasm
	./hook-cleaner-c/hook-cleaner ./build/$(HOOK_C_FILENAME).wasm
	./xrpld-hooks/src/ripple/app/hook/guard_checker ./build/$(HOOK_C_FILENAME).wasm

set-hooks:
	node ./client/setHooks.js

clean:
	rm -rf build/*
