# XRPL Hooks App

## Install Dependencies

1. This project uses [`jq` command-line tool](https://stedolan.github.io/jq/download/) to allow `makefile` to read variables from `config.json`. Please ensure it's installed.

    `$ brew install jq`

2. This project uses Git submodules to manage dependencies. To install and build these dependencies, you'll need to run the following command from the root directory of the project:

    `$ npm run setup`

    This command will initialize and update the Git submodules, which will download the necessary dependencies for the project, then build them. It will also create a required `config.json` with necessary configuration variables to run the project.

Once the dependencies are installed, you should be able to run the project. If you encounter any issues during the installation process, please refer to the project's documentation or contact the project maintainer for assistance.

## Compile Hook and Install It Onto An Account


Run this command to locally compile an XRPL Hook source file (inside ./hook-src) from .c to .wasm code and install it onto an account by submitting a `SetHook` transaction:

`$ npm run build-set-hooks`

Here is what each command does in the background:
1. `wasmcc` - compiles a Hook source file (C code) to WebAssembly (WASM) code. For example, `./hook-src/starter.c` compiles to `./build/starter.wasm`
2. `wasm-opt` - optimizes the WASM code `./build/starter.wasm`
3. `hook-cleaner` - cleans it by removing unnecessary additional exports
4. `guard_checker` - this checks if any guard violation has occurred in the Hooks code before submitting it in `SetHook` transaction. For more information, visit [this link](https://xrpl-hooks.readme.io/docs/loops-and-guarding)
5. Converts the compiled WASM to hexadecimal characters then submits it as payload in a `SetHook` transaction

# Hooks v3 Debug Stream


One can view Hooks application logs for debugging/development by visiting this URL with the last part being the account that has hook(s) installed on.

For example:
https://hooks-testnet-v3-debugstream.xrpl-labs.com/r47BrSvzszRDWDay33MvKs6gpWJGers2Wb/
