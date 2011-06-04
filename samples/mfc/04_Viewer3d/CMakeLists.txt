PROJECT(Viewer3d)

FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HEADERS src/*.h)
FILE(GLOB RES res/*.rc)
SET(PCH src/StdAfx.h src/StdAfx.cpp)

ADD_EXECUTABLE(Viewer3d WIN32 ${SRCS} ${HEADERS} ${PCH} ${RES}) 