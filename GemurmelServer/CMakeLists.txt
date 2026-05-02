add_executable(
        gemurmel_server
        main.cpp
)

set_target_properties(
        gemurmel_server
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        POSITION_INDEPENDENT_CODE ON
        INTERPROCEDURAL_OPTIMIZATION ${LTO_SUPPORTED}
)

target_link_libraries(
        gemurmel_server
        PRIVATE gemurmel
)