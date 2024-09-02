file(GLOB_RECURSE SOURCES
    *.h
    *.c
)

add_executable(vec_ll ${SOURCES})
set_property(TARGET vec_ll PROPERTY C_STANDARD 23)