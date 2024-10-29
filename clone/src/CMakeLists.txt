file(GLOB_RECURSE SOURCES
    *.h
    *.c
)

add_executable(clone ${SOURCES})
set_property(TARGET clone PROPERTY C_STANDARD 23)
target_link_libraries(clone m)
