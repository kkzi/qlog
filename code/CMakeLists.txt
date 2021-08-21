add_definitions(-DQLOG_EXPORTS)


include_directories(include/${PROJECT_NAME})

file(GLOB_RECURSE SRCS *.h *.cpp)

aux_source_directory(include SRCS)
aux_source_directory(src SRCS)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Core)
