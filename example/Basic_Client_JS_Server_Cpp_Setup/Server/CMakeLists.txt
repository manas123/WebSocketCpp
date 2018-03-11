message( "*************************************" )
message( "*************************************" )
message( "************ WebsocketCPP ***********" )
message( "*************************************" )
message( "*************************************" )

## CMAKE INITIALISATION
PROJECT(WebSocketsAPI)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_modules")
## END CMAKE INITIALISATION

## PROJECT LANGUAGE
ENABLE_LANGUAGE(CXX C)
## END PROJECT LANGUAGE

# check prerequisites
if(NOT Boost_FOUND)
	set(Boost_USE_MULTITHREADED ON)		# default is ON, use -mt variant
	set(Boost_USE_STATIC_LIBS ON)		# default is OFF, use boost static library
#	set(Boost_USE_STATIC_RUNTIME ON)	# default is not set.
	set(BOOST_COMPONENTS ${BOOST_COMPONENTS} regex)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_REGEX")

	find_package(Boost 1.50.0 COMPONENTS ${BOOST_COMPONENTS} serialization thread system chrono)
	if(NOT Boost_FOUND)
		set(BOOST_ROOT "C:/Program Files/boost/boost_1_50_0")		# if not set, it will let cmake to find it.

		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/x86)			# might need to set this too.
		set(Boost_NO_SYSTEM_PATHS ON)								# default is OFF, set to ON if BOOST_ROOT is set
		find_package(Boost 1.50.0 COMPONENTS ${BOOST_COMPONENTS} serialization thread system chrono REQUIRED)

	endif()
	include_directories(SYSTEM ${Boost_INCLUDE_DIR})
endif()

# Don't warn about zero-length format strings, which we sometimes
SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-zero-length")
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
find_package(Threads REQUIRED)

SET (EXTRA_LIBRARIES pthread rt crypto)

## PROJCET INCLUDES DIRECTORIES

INCLUDE_DIRECTORIES(include)

SET(SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.cpp
						${CMAKE_CURRENT_LIST_DIR}/../../../src/websocket.cpp
						${CMAKE_CURRENT_LIST_DIR}/../../../include/server_ws.hpp
						${CMAKE_CURRENT_LIST_DIR}/../../../include/client_ws.hpp
)

add_executable(websocket ${SOURCES})

target_link_libraries(websocket ${Boost_LIBRARIES})
target_link_libraries(websocket ${EXTRA_LIBRARIES})
target_link_libraries(websocket log4cpp)
target_link_libraries(websocket ${OPENSSL_CRYPTO_LIBRARY})
target_link_libraries(websocket ${CMAKE_THREAD_LIBS_INIT})


message( "*************************************" )
message( "*************************************" )
message( "***************END*******************" )
message( "*************************************" )
message( "*************************************" )