cmake_minimum_required(VERSION 3.10)
project(Laboratorio1)

include(FetchContent)

set(SERVER_SRC
    src/server.cpp
    src/libs/tcpServer.cpp
    src/libs/utils.cpp
)
set(CLIENT_SRC
    src/client.cpp
    src/libs/tcpClient.cpp
    src/libs/utils.cpp
    src/libs/cryptoUtils.cpp
)
set(DECRYPT_SRC
    src/decrypt.cpp
    src/libs/utils.cpp
    src/libs/cryptoUtils.cpp
)

FetchContent_Declare(Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
    GIT_TAG 99f14233eab1d4f7f49c2af4ec836f2e701c445e # HEAD as of 2022-05-28
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

add_executable(client ${CLIENT_SRC})
add_executable(server ${SERVER_SRC})
add_executable(decrypt ${DECRYPT_SRC})

target_link_libraries(decrypt PRIVATE sodium)
target_link_libraries(client PRIVATE sodium)

include_directories(include)

