cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(LIB_NAME RWP)
set(INCLUDE_DIR include)
set(PUBLIC_INCLUDE_DIR ${INCLUDE_DIR}/public)
set(PRIVATE_INCLUDE_DIR ${INCLUDE_DIR}/private)
set(SRC_DIR src)

project(${LIB_NAME} LANGUAGES CXX)

set(PUBLIC_FILES
	${PUBLIC_INCLUDE_DIR}/RWP/RWP.hpp
)
set(PRIVATE_FILES
	${PRIVATE_INCLUDE_DIR}/RWP/Msg.hpp
	${PRIVATE_INCLUDE_DIR}/RWP/Util.hpp
	${PRIVATE_INCLUDE_DIR}/RWP/Crypt.hpp
)
set(SRC_FILES
	${SRC_DIR}/Msg.cpp
	${SRC_DIR}/Util.cpp
	${SRC_DIR}/Crypt.cpp
)

set(ALL_FILES
	${PUBLIC_FILES}
	${PRIVATE_FILES}
	${SRC_FILES}
)

add_executable(${LIB_NAME} ${ALL_FILES} main.cpp)

if(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /std:c++20)
else()
    target_compile_options(${LIB_NAME} PRIVATE -std=c++20)
endif()