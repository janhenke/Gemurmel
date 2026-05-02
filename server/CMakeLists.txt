add_executable(
        mumble_server
        src/main.cpp
)
if (WIN32)
    target_sources(
            mumble_server
            PUBLIC
            src/application.manifest
    )
endif ()

set_target_properties(
        mumble_server
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

find_package(libpqxx CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(
        mumble_server
        PRIVATE mumble_protocol
        PRIVATE libpqxx::pqxx
        PRIVATE spdlog::spdlog
)
