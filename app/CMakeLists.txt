#add .a https://blog.csdn.net/yoyo_newbie/article/details/74427938
cmake_minimum_required (VERSION 2.8)
project(cpplibrary)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
if(CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

message("tjy")
set(SRCPATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)

message("${CMAKE_CURRENT_SOURCE_DIR}")
message("${SRCPATH}")

include_directories("${SRCPATH}/media/include")
include_directories("${SRCPATH}")
set(TARGET_NAME ${PROJECT_NAME})
message("target name ${TARGET_NAME}")
message("yuv dir ${PUBLIC_YUV_LIBS}")


message("heiheihei")
set(SRC_PATH ${SRCPATH}/media)
set(SRC_J4A_PATH ${SRCPATH}/j4a)
set(SRC_NIO_PATH ${SRCPATH}/java/nio)
set(SRC_OS_PATH ${SRCPATH}/java/os)
set(SRC_UTIL_PATH ${SRCPATH}/java/util)
message("SRC_PATH ${SRC_PATH}")
message("COMMON_PATH ${COMMON_PATH}")
set(SOURCES
	${SRC_PATH}/MediaCodec.c
	${SRC_PATH}/MediaFormat.c
	${SRC_PATH}/PlaybackParams.c
	${SRC_J4A_PATH}/j4a_allclasses.c
	${SRC_J4A_PATH}/j4a_base.c
	${SRC_NIO_PATH}/ByteBuffer.c
	${SRC_NIO_PATH}/Buffer.c
	${SRC_UTIL_PATH}/ArrayList.c
	${SRC_OS_PATH}/Build.c
	${SRC_OS_PATH}/Bundle.c
	${SRCPATH}/media_player_test.cpp
	${SRCPATH}/AndroidMediaCodec.cpp
	${SRCPATH}/JniHandler.cpp
)


set (PUBLIC_LINK_LIBS dl log android)

add_library (${TARGET_NAME} SHARED ${SOURCES})
target_link_libraries(${TARGET_NAME} ${PUBLIC_LINK_LIBS})