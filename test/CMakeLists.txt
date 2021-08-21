project(test)

include_directories(${CMAKE_SOURCE_DIR}/code/include)

find_package(Qt5 COMPONENTS Core Widgets)


add_executable(log2file Log2FileTest.cpp)
target_link_libraries(log2file PRIVATE qlog)


find_package(Qt5 COMPONENTS Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_executable(logtable LogTableTest.cpp)
target_link_libraries(logtable PRIVATE qlog Qt::Core Qt::Widgets)