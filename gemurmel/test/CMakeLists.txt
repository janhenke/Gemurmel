add_executable(
        gemurmel_test
        test_varint.cpp
        test_client.cpp
        test_strong_id.cpp
)

set_target_properties(
        gemurmel_test
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_link_libraries(
        gemurmel_test
        PRIVATE gemurmel
        PRIVATE Catch2::Catch2WithMain
)

if (NOT MSVC)
    # Catch2 uses __COUNTER__ which is a C2y extension in C++23
    target_compile_options(
            gemurmel_test
            PRIVATE -Wno-c2y-extensions
    )
endif ()

catch_discover_tests(gemurmel_test)