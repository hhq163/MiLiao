cmake_minimum_required(VERSION 2.6)
PROJECT(http_server)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
AUX_SOURCE_DIRECTORY(./controller SRC_LIST)
AUX_SOURCE_DIRECTORY(./model SRC_LIST)

#SET(LIBRARY_OUTPUT_PATH ./)
SET(EXECUTABLE_OUTPUT_PATH  ./)
if(APPLE)
    SET(PB_LIB_DIR ../core/pb/lib/mac)
elseif(UNIX)
    SET(PB_LIB_DIR ../core/pb/lib/linux)
endif()

SET(MYSQL_INCLUDE_DIR /usr/local/mysql/include)
SET(MYSQL_LIB /usr/local/mysql/lib)

ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11 -DENCRYPT)

INCLUDE_DIRECTORIES(./ ./controller ./model ../core ../core/jsoncpp ../protocol ../core/http
../core/pb/ ${MYSQL_INCLUDE_DIR})
LINK_DIRECTORIES(./ ../core ${PB_LIB_DIR} ${MYSQL_LIB} /usr/local/mysql/lib)

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})
ADD_EXECUTABLE(http_server ${SRC_LIST})

TARGET_LINK_LIBRARIES(http_server pthread core protobuf-lite mysqlclient_r hiredis curl crypto rt) 
 
