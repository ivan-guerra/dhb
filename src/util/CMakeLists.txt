cmake_minimum_required(VERSION 3.13...3.25)

project(util DESCRIPTION "dhb argument validation and formatting"
                         LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE dhb_util.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${DHB_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE base_conversions
)
