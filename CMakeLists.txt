cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Threads REQUIRED)
project(turtletrafficctrl)
set(src
    Main.cpp
    FieldController.hpp
    GetIterator.hpp
    GPSInformation.hpp
    HttpException.hpp
    ProtectionAreaController.hpp
    ProtectionAreaInformation.hpp
    TurtleTrafficController.hpp
)
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")
endif()
add_executable(turtletrafficctrl ${src})
target_include_directories(turtletrafficctrl PUBLIC httplib)
target_include_directories(turtletrafficctrl PUBLIC json/include)
if(WIN32)
  target_link_libraries(turtletrafficctrl Ws2_32)
endif()
