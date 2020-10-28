PROJECT( TimeSeries )
Set(CMAKE_CXX_FLAGS "-O3 -Wall")
ADD_EXECUTABLE(OSFS.x main.cpp algoritmo.cpp serietemporal.cpp metodoFSW.cpp metodoSerieTemporalEMaxOffLine.cpp punto.cpp recta.cpp vector.cpp)
