PROJECT(mfcsample)

ADD_DEFINITIONS(-D_AFXDLL)
SET(CMAKE_MFC_FLAG 2) 
SET(SOURCES src/mfcsample.cpp)
SET(PCH src/StdAfx.cpp)

ADD_LIBRARY(mfcsample SHARED ${SRCS} ${PCH})
