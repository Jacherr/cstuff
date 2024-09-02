file(GLOB_RECURSE SOURCES
    *.h
    *.c
)

add_executable(arena ${SOURCES})
set_property(TARGET arena PROPERTY C_STANDARD 23)