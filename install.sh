# Install/update required git submodules: binaryen, hook-cleaner-c, xrpld-hooks
git submodule update --init

# Install wasienv
curl https://raw.githubusercontent.com/wasienv/wasienv/master/install.sh | sh
source ~/.wasienv/wasienv.sh

# Build binaryen
cd binaryen
git submodule update --init
cmake . && make
cd ..

# Build hook-cleaner-c
cd hook-cleaner-c
make
cd ..

# Build guard_checker in xrpld-hooks
cd xrpld-hooks/src/ripple/app/hook
make
cd ..
