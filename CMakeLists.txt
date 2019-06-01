#######################################################
# cmake file for building TMVA analysis
# @author Steve Green. Cambridge
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

########################################################

# project name
PROJECT( TMVAAnalysis )

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

### DEPENDENCIES ############################################################

FIND_PACKAGE( ROOT REQUIRED )

FOREACH( pkg ROOT )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
    ENDIF()
ENDFOREACH()

INCLUDE_DIRECTORIES(BEFORE ./include ) # use BEFORE to fix problem on mac osx (case insensitive filesystem)

### LIBRARY AND TOOLS #######################################################

SET( CMAKE_CXX_FLAGS "-std=c++14" )

ADD_EXECUTABLE(TMVAClassifier ./src/TMVAClassifier.cxx)
ADD_EXECUTABLE(TMVAClassificationApplication ./src/TMVAClassificationApplication.cxx)

target_link_libraries(TMVAClassifier /cvmfs/larsoft.opensciencegrid.org/products/root/v6_12_06a/Linux64bit+2.6-2.12-e15-prof/lib/libTMVA.so)
target_link_libraries(TMVAClassificationApplication /cvmfs/larsoft.opensciencegrid.org/products/root/v6_12_06a/Linux64bit+2.6-2.12-e15-prof/lib/libTMVA.so)

INSTALL( TARGETS TMVAClassifier TMVAClassificationApplication DESTINATION ${PROJECT_SOURCE_DIR}/bin PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE )

