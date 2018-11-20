file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(lll ${sources} ${headers})
target_link_libraries(lll PUBLIC evmasm devcore)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")
message(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
