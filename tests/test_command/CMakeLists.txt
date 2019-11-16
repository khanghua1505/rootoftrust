cmake_minimum_required(VERSION 3.15)
project(test_command C)

set(CMAKE_C_STANDARD 99)

set(ED25519_SRC
        "src/ed25519/fe.c"
        "src/ed25519/ge.c"
        "src/ed25519/keypair.c"
        "src/ed25519/sc.c"
        "src/ed25519/sign.c"
        "src/ed25519/verify.c"
        "src/ed25519/seed.c")
set(ED25519_INC src/ed25519)

set(SHA3_SRC src/sha3/sha3.c)
set(SHA3_INC src/sha3)

set(SRC
        src/keys.c
        src/plaintext.c
        src/serial.c
        src/test.c
        src/main.c)
set(INC src)

include_directories(${ED25519_INC} ${SHA3_INC} ${INC})

add_executable(test ${ED25519_SRC} ${SHA3_SRC} ${SRC})
target_link_libraries(test -lserialport)