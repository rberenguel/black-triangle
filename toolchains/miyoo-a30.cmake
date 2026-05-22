set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain from: brew install messense/macos-cross-toolchains/arm-unknown-linux-gnueabihf
# or: https://github.com/miyoo-mini/miyoo-mini-toolchain
set(TOOLCHAIN_PREFIX arm-linux-gnueabihf)
find_program(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc   REQUIRED)
find_program(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++   REQUIRED)
find_program(CMAKE_AR           ${TOOLCHAIN_PREFIX}-ar     REQUIRED)
find_program(CMAKE_STRIP        ${TOOLCHAIN_PREFIX}-strip  REQUIRED)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Miyoo A30 / Mini Plus: Cortex-A53, but soft-float ABI toolchain is what the
# community uses for OnionOS / MiniUI compatibility
set(CMAKE_C_FLAGS_INIT   "-mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard")
