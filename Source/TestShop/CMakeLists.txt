cmake_minimum_required(VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include(${CMAKE_SOURCE_DIR}/Scripts/CMake/FetchLibraries.cmake)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/TestShop/**.cpp" "${CMAKE_SOURCE_DIR}/Source/TestShop/**.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/TestShop" FILES ${SUBDIRECTORIES})
add_executable(TestShop main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ---------------- WINDOWS EXECUTALE OPTIONS -----------------
#if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#	set_target_properties(EditorPlayer PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
#	set_target_properties(EditorPlayer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
#	set_target_properties(EditorPlayer PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${CMAKE_BINARY_DIR}/Debug")
#	set_target_properties(EditorPlayer PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_BINARY_DIR}/Debug")
#	set_target_properties(EditorPlayer PROPERTIES VS_DEBUGGER_COMMAND "EditorPlayer.exe")
#endif()
# ------------------------------------------------------------

# -------------------- INCLUDE LIBRARIES ---------------------
target_include_directories(TestShop PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(TestShop PRIVATE Runtime)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(TestShop PUBLIC PLAYER_API=__declspec\(dllexport\))
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_definitions(TestShop PUBLIC PLAYER_API=__attribute__\(\(visibility\(\"default\"\)\)\))
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_definitions(TestShop PUBLIC PLAYER_API=__attribute__\(\(visibility\(\"default\"\)\)\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------

# -------------------- REFLECTION PARSER ---------------------
###SetReflectionParser(EditorPlayer)
# ------------------------------------------------------------