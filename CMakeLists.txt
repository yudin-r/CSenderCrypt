cmake_minimum_required(VERSION 3.10)
project(CSenderCrypt C)

set(CMAKE_C_STANDARD 99)

find_package(CURL REQUIRED)

add_executable(CSenderCrypt main.c
        email_sender.h
        email_sender.c
        smtp_config.h
        smtp_config.c
        rsa.c
        rsa.h)

target_include_directories(CSenderCrypt PRIVATE ${CURL_INCLUDE_DIRS})

target_link_libraries(CSenderCrypt ${CURL_LIBRARIES})