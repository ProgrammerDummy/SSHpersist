cmake_minimum_required(VERSION 3.16)
project(SSHPersist LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where libssh is installed.
# For you on macOS (Apple Silicon + Homebrew):
set(LIBSSH_ROOT "/opt/homebrew")

set(LIBSSH_INCLUDE_DIR "${LIBSSH_ROOT}/include")
set(LIBSSH_LIBRARY_DIR "${LIBSSH_ROOT}/lib")

add_executable(sshpersist
    app.cpp
    # add other source files here
)

# Include + link dirs
target_include_directories(sshpersist PRIVATE
    "${LIBSSH_INCLUDE_DIR}"
)

target_link_directories(sshpersist PRIVATE
    "${LIBSSH_LIBRARY_DIR}"
)

target_link_libraries(sshpersist PRIVATE
    ssh
)

# ---- RPATH so the binary can find libssh at runtime on macOS ----
if(APPLE)
    # Embed /opt/homebrew/lib into the runtime search path
    set_target_properties(sshpersist PROPERTIES
        BUILD_RPATH "${LIBSSH_LIBRARY_DIR}"
        INSTALL_RPATH "${LIBSSH_LIBRARY_DIR}"
    )
endif()

