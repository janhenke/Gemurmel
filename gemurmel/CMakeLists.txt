add_library(
        gemurmel
)

target_sources(
        gemurmel
        PRIVATE
        src/client.cpp
        src/protocol.cpp
        src/server.cpp
        src/varint.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/include/gemurmel/gemurmel_export.h
        PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/gemurmel/asio.hpp
        include/gemurmel/client.hpp
        include/gemurmel/client_event.hpp
        include/gemurmel/model.hpp
        include/gemurmel/protocol.hpp
        include/gemurmel/server.hpp
        include/gemurmel/strong_id.hpp
        include/gemurmel/detail/protocol.hpp
        include/gemurmel/detail/thread_safe_queue.hpp
        include/gemurmel/detail/varint.hpp
)

generate_export_header(
        gemurmel
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/gemurmel/gemurmel_export.h"
)

set_target_properties(
        gemurmel
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
        INTERPROCEDURAL_OPTIMIZATION ${LTO_SUPPORTED}
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_definitions(
        gemurmel
        PRIVATE ASIO_NO_DEPRECATED
        PRIVATE SPDLOG_USE_STD_FORMAT
)

if (WIN32)
    target_compile_definitions(
            gemurmel
            PUBLIC
            _WIN32_WINNT=0x0A00
    )
endif ()

target_include_directories(
        gemurmel
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(
        gemurmel
        PRIVATE mumble_protocol
)

find_package(OpenSSL REQUIRED)
if(GEMURMEL_USE_BOOST_ASIO)
    find_package(Boost REQUIRED COMPONENTS system)
    target_link_libraries(
            gemurmel
            PUBLIC Boost::system
            PRIVATE OpenSSL::SSL
    )
    target_compile_definitions(
            gemurmel
            PUBLIC GEMURMEL_USE_BOOST_ASIO
    )
else()
    find_package(asio REQUIRED)
    target_link_libraries(
            gemurmel
            PUBLIC asio::asio
            PRIVATE OpenSSL::SSL
    )
endif()

if (GEMURMEL_BUILD_TEST)
    add_subdirectory(test)
endif ()
