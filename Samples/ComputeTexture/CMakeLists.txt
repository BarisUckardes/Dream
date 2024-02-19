cmake_minimum_required(VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include(${CMAKE_SOURCE_DIR}/Scripts/CMake/FetchLibraries.cmake)
# ------------------------------------------------------------

# ---------------------- CMAKE FETCH -------------------------
FetchContent_MakeAvailable(glm)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Samples/ComputeTexture/**.cpp" "${CMAKE_SOURCE_DIR}/Samples/ComputeTexture/**.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Samples/ComputeTexture" FILES ${SUBDIRECTORIES})
add_executable(ComputeTexture main.cpp ${SUBDIRECTORIES})
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
target_include_directories(ComputeTexture PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(Runtime PUBLIC "${glm_SOURCE_DIR}")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(ComputeTexture PRIVATE Runtime)
target_compile_definitions(ComputeTexture PUBLIC RES_PATH="${CMAKE_SOURCE_DIR}/Resources")
# ------------------------------------------------------------


# -------------------- REFLECTION PARSER ---------------------
###SetReflectionParser(EditorPlayer)
# ------------------------------------------------------------