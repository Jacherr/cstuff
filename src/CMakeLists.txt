file(GLOB_RECURSE SOURCES
    *.h
    *.c
)

add_executable(cstuff ${SOURCES})
set_property(TARGET cstuff PROPERTY C_STANDARD 23)