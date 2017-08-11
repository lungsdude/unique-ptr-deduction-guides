cmake_minimum_required(VERSION 3.0)
project(unique-ptr-deduction-guides)

set(TARGET_NAME ${PROJECT_NAME})

add_library(${TARGET_NAME}
    test.cpp
    deduction_guides.h
)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
