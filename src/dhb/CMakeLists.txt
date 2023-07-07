cmake_minimum_required(VERSION 3.13...3.25)

project(dhb DESCRIPTION "convert between base 2, 10, and 16 numbers"
            LANGUAGES   CXX
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE dhb.cc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE base_conversions
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${DHB_BIN_DIR}
)
