cmake_minimum_required(VERSION 3.13)

SET (SOURCES
    srcs/main.c
	srcs/debug.c
	srcs/fill_file.c
	srcs/find_section.c
	srcs/rc4.c
    )

SET (INCLUDES
    woody.h
    )

INCLUDE_DIRECTORIES(includes)

add_executable(woody ${SOURCES})