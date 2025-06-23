

cmake_minimum_required(VERSION 3.10)

# Define the project
# Specify CXX language
project(ArcscriptTranspiler_Lib CXX) # Using ArcscriptTranspiler_Lib to differentiate from UE module name

# --- Find Antlr4-runtime Library and Headers ---
# Since antlr4-runtime is no longer a subdirectory, we need to find it.
# CMake will look in standard system locations and paths provided via CMAKE_PREFIX_PATH or hints.

# Find the antlr4-runtime library binary (.a or .dylib)
# You might need to provide hints if it's not in a standard location.
# Example hint: HINTS /path/to/where/antlr4-runtime/is/installed/lib
find_library(ANTLR4_RUNTIME_LIBRARY
    NAMES antlr4-runtime # CMake will look for libantlr4-runtime.a or antlr4-runtime.dylib etc.
    # HINTS /path/to/where/antlr4-runtime/lib # Uncomment and adjust if needed
)

# Find the antlr4-runtime include directory
# Look for a known header file within the include path.
# Example hint: HINTS /path/to/where/antlr4-runtime/is/installed/include
find_path(ANTLR4_RUNTIME_INCLUDE_DIR
    NAMES antlr4-runtime/antlr4-runtime.h # Look for a specific header file
    # HINTS /path/to/where/antlr4-runtime/include # Uncomment and adjust if needed
)

# Handle case where antlr4-runtime is not found
if (NOT ANTLR4_RUNTIME_LIBRARY OR NOT ANTLR4_RUNTIME_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find antlr4-runtime library or headers. Please set CMAKE_PREFIX_PATH or provide hints.")
endif()

# --- Define the ArcscriptTranspiler Library ---

# Gather source files for the main library
# Use file(GLOB ...) cautiously; listing files explicitly is generally safer for robustness,
# but GLOB is quicker for initial setup based on a simple directory structure like 'src'.
# Adjust patterns if your source files are in different subdirectories within src.
file(GLOB ARCSCRIPT_TRANSPILER_SOURCES
    "src/*.cpp"
    # Add other specific source files if not all are in src/ or have different extensions
    # e.g., "src/AnotherSubdirectory/*.cpp"
)

# Define the library target
# We want a dynamic library (.dll on Windows, .dylib on Mac, .so on Linux)
add_library(${PROJECT_NAME} SHARED ${ARCSCRIPT_TRANSPILER_SOURCES}) # ${PROJECT_NAME} expands to ArcscriptTranspiler_Lib

# --- Configure Include Paths ---
# Public include paths make headers available to projects that link against this library.
# Private include paths are only for compiling this library itself.

target_include_directories(${PROJECT_NAME} PUBLIC
    # Headers directly in src/
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    # Generated parser/lexer headers (assuming they are generated into these folders relative to the source root)
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Generated/ArcscriptParser"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Generated/ArcscriptLexer"
    # Include path for antlr4-runtime headers (using the path found by find_path)
    ${ANTLR4_RUNTIME_INCLUDE_DIR}
)

# --- Configure Preprocessor Definitions ---
# Add definitions based on configurations (Debug/Release) and platforms

target_compile_definitions(${PROJECT_NAME} PRIVATE
    # Add any common definitions needed for compiling the library itself
    # e.g., based on what was in <PreprocessorDefinitions> in the .vcxproj

    # Example: Configuration specific definitions
    $<$<CONFIG:Debug>:_DEBUG> # Add _DEBUG in Debug configuration
    $<$<CONFIG:Debug>:DEBUG_BUILD> # Add your custom debug definition
    $<$<CONFIG:Release>:NDEBUG> # Add NDEBUG in Release configuration
    $<$<CONFIG:Release>:RELEASE_BUILD> # Add your custom release definition

    # Add definitions that might be specific to your project's logic,
    # found in the .vcxproj's <PreprocessorDefinitions>
    # Example: "MY_PROJECT_DEFINE"
)

# --- Link Dependencies ---
# Link the ArcscriptTranspiler library against the found antlr4-runtime library.

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${ANTLR4_RUNTIME_LIBRARY} # Link against the antlr4-runtime library binary found earlier
    # Add any other external libraries ArcscriptTranspiler links against here
    # e.g., some_other_library.lib becomes some_other_library (CMake handles extensions)
    # You might need platform-specific linkage here as well:
    # $<$<OR:$<TARGET_OS:Windows>,$<TARGET_OS:Linux>>:dl> # Example: Link against libdl on Unix-like systems
    # $<$<TARGET_OS:Darwin>:-framework CoreFoundation> # Example: Link against a macOS framework
)

# --- Installation (Optional but Recommended) ---
# Define rules to install headers and the built library file
# This is useful if you were installing this library system-wide or for another project,
# though for UE External modules, you usually just copy the built files manually.
# Still, defining install rules is good practice.

install(TARGETS ${PROJECT_NAME}
    DESTINATION lib # Install the built library to the 'lib' subdirectory in the install prefix
)

install(DIRECTORY src/ # Install header files from src/
    DESTINATION include/${PROJECT_NAME} # Install them to include/ArcscriptTranspiler_Lib
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" # Only install header files
)

# Add install rules for the generated headers in src/Generated if needed
install(DIRECTORY src/Generated/ArcscriptParser/
    DESTINATION include/${PROJECT_NAME}/Generated/ArcscriptParser
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY src/Generated/ArcscriptLexer/
    DESTINATION include/${PROJECT_NAME}/Generated/ArcscriptLexer
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# --- End of CMakeLists.txt ---