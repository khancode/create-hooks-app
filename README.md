# XRPL Hooks App

## Install Dependencies

This project uses Git submodules to manage dependencies. To install and build these dependencies, you'll need to run the following command from the root directory of the project:

`$ npm run setup`

This command will initialize and update the Git submodules, which will download the necessary dependencies for the project, then build them. It will also create a required `config.json` with necessary configuration variables to run the project.

Once the dependencies are installed, you should be able to run the project. If you encounter any issues during the installation process, please refer to the project's documentation or contact the project maintainer for assistance.

## Compile Hook and Install It Onto An Account


Run this command to locally compile an XRPL Hook source file (inside ./hook-src) from .c to .wasm code and install it onto an account by submitting a `SetHook` transaction:

`$ npm run build-set-hook`

Here is what happens in the background:
1. Compiles a Hook source file (C code) to WebAssembly (WASM) code. For example, `./hook-src/starter.c` compiles to `./build/starter.wasm`
2. Optimizes the WASM code
3. Cleans it by removing unnecessary additional exports
4. Run guard_checker on it. This checks if any guard violation has occurred in the Hooks code before submitting it in `SetHook` transaction. For more information, visit [this link](https://xrpl-hooks.readme.io/docs/loops-and-guarding)
5. Converts the compiled WASM to hexadecimal characters then submits it as payload in a `SetHook` transaction
