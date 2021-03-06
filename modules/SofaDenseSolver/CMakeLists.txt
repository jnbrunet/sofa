cmake_minimum_required(VERSION 3.12)
project(SofaDenseSolver LANGUAGES CXX)

sofa_find_package(Newmat REQUIRED BOTH_SCOPES)
sofa_find_package(SofaBase REQUIRED)
sofa_find_package(SofaGeneralLinearSolver REQUIRED)

set(SOFADENSESOLVER_SRC "src/${PROJECT_NAME}")

set(HEADER_FILES
    ${SOFADENSESOLVER_SRC}/config.h.in
    ${SOFADENSESOLVER_SRC}/initSofaDenseSolver.h
    )

set(SOURCE_FILES
    ${SOFADENSESOLVER_SRC}/initSofaDenseSolver.cpp
    )

list(APPEND HEADER_FILES
    ${SOFADENSESOLVER_SRC}/LULinearSolver.h
    ${SOFADENSESOLVER_SRC}/NewMatMatrix.h
    ${SOFADENSESOLVER_SRC}/NewMatVector.h
    )
list(APPEND SOURCE_FILES
    ${SOFADENSESOLVER_SRC}/LULinearSolver.cpp
    ${SOFADENSESOLVER_SRC}/NewMatCGLinearSolver.cpp
    ${SOFADENSESOLVER_SRC}/NewMatCholeskySolver.cpp
    )

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaBaseLinearSolver SofaGeneralLinearSolver)
target_link_libraries(${PROJECT_NAME} PUBLIC newmat)

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${Sofa_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "SofaDenseSolver"
    RELOCATABLE "plugins"
)
