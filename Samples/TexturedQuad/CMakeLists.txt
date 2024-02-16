cmake_minimum_required(VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include(${CMAKE_SOURCE_DIR}/Scripts/CMake/FetchLibraries.cmake)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Sample/TexturedQuad/**.cpp" "${CMAKE_SOURCE_DIR}/Sample/TexturedQuad/**.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Sample/TexturedQuad" FILES ${SUBDIRECTORIES})
add_executable(TexturedQuad main.cpp ${SUBDIRECTORIES})
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
target_include_directories(TexturedQuad PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(TexturedQuad PRIVATE Runtime)
# ------------------------------------------------------------


# -------------------- REFLECTION PARSER ---------------------
###SetReflectionParser(EditorPlayer)
# ------------------------------------------------------------