# mumble_protocol
find_package(Protobuf REQUIRED)

add_library(
        mumble_protocol
        OBJECT
        src/Mumble.proto
        src/MumbleUDP.proto
)
protobuf_generate(
        TARGET mumble_protocol
)

# Disable specific compiler warnings for generated file (because we cannot fix the warnings)
set_target_properties(
        mumble_protocol
        PROPERTIES
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wno-error=narrowing;-Wno-nullability-extension>$<$<CXX_COMPILER_ID:MSVC>:/wd4244>"
        POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
        mumble_protocol
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories(
        mumble_protocol
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
)

if (WIN32)
    target_compile_definitions(
            mumble_protocol
            PUBLIC
            _WIN32_WINNT=0x0A00
    )
endif ()

target_link_libraries(
        mumble_protocol
        PUBLIC protobuf::libprotobuf
)
