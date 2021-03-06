cmake_minimum_required(VERSION 2.8.11)
project(FAST)

option(VTK_INTEROP "Enable interoperability with VTK" OFF)
option(ITK_INTEROP "Enable interoperability with ITK" OFF)

set(SOURCES 
    main.cpp
    Object.cpp
    Object.hpp
    SmartPointers.hpp
    ProcessObject.cpp
    ProcessObject.hpp
    ExecutionDevice.cpp
    ExecutionDevice.hpp
    DeviceManager.cpp
    DeviceManager.hpp
    Exception.cpp
    Exception.hpp
    Utility.cpp
    Utility.hpp
    Algorithms/GaussianSmoothingFilter/GaussianSmoothingFilter.cpp
    Algorithms/GaussianSmoothingFilter/GaussianSmoothingFilter.hpp
    Algorithms/SeededRegionGrowing/SeededRegionGrowing.cpp
    Algorithms/SeededRegionGrowing/SeededRegionGrowing.hpp
    Algorithms/SurfaceExtraction/SurfaceExtraction.cpp
    Algorithms/SurfaceExtraction/SurfaceExtraction.hpp
    Data/DataObject.cpp
    Data/DataObject.hpp
    Data/ImageData.cpp
    Data/ImageData.hpp
    Data/DynamicImage.cpp
    Data/DynamicImage.hpp
    Data/Image.cpp
    Data/Image.hpp
    Data/DataTypes.cpp
    Data/DataTypes.hpp
    Data/Surface.cpp
    Data/Surface.hpp
    Data/SurfaceVertex.hpp
    Data/Access/Access.hpp
    Data/Access/OpenCLBufferAccess.cpp
    Data/Access/OpenCLBufferAccess.hpp
    Data/Access/OpenCLImageAccess2D.cpp
    Data/Access/OpenCLImageAccess2D.hpp
    Data/Access/OpenCLImageAccess3D.cpp
    Data/Access/OpenCLImageAccess3D.hpp
    Data/Access/ImageAccess.cpp
    Data/Access/ImageAccess.hpp
    Data/Access/VertexBufferObjectAccess.cpp
    Data/Access/VertexBufferObjectAccess.hpp
    Data/Access/SurfacePointerAccess.cpp
    Data/Access/SurfacePointerAccess.hpp
    Streamers/Streamer.cpp
    Streamers/Streamer.hpp
    Streamers/ImageStreamer.cpp
    Streamers/ImageStreamer.hpp
    Streamers/MetaImageStreamer.cpp
    Streamers/MetaImageStreamer.hpp
    Importers/MetaImageImporter.cpp
    Importers/MetaImageImporter.hpp
    Importers/ImageImporter.cpp
    Importers/ImageImporter.hpp
    Importers/Importer.hpp
    Exporters/ImageExporter.cpp
    Exporters/ImageExporter.hpp
    Exporters/MetaImageExporter.cpp
    Exporters/MetaImageExporter.hpp
    Visualization/SimpleWindow.cpp
    Visualization/SimpleWindow.hpp
    Visualization/WindowWidget.cpp
    Visualization/WindowWidget.hpp
    Visualization/View.cpp
    Visualization/View.hpp
    Visualization/Renderer.cpp
    Visualization/Renderer.hpp
    Visualization/ImageRenderer/ImageRenderer.cpp
    Visualization/ImageRenderer/ImageRenderer.hpp
    Visualization/SliceRenderer/SliceRenderer.cpp
    Visualization/SliceRenderer/SliceRenderer.hpp
    Visualization/SurfaceRenderer/SurfaceRenderer.cpp
    Visualization/SurfaceRenderer/SurfaceRenderer.hpp
    Visualization/VolumeRenderer/VolumeRenderer.cpp
    Visualization/VolumeRenderer/VolumeRenderer.hpp
    Visualization/VolumeRenderer/ColorTransferFunction.cpp
    Visualization/VolumeRenderer/ColorTransferFunction.hpp
    Visualization/VolumeRenderer/OpacityTransferFunction.cpp
    Visualization/VolumeRenderer/OpacityTransferFunction.hpp
)

if(VTK_INTEROP)
    set(SOURCES ${SOURCES}
        Exporters/VTKImageExporter.cpp
        Exporters/VTKImageExporter.hpp
        Importers/VTKImageImporter.cpp
        Importers/VTKImageImporter.hpp
    )
endif()

if(ITK_INTEROP)
    set(SOURCES ${SOURCES}
        Exporters/ITKImageExporter.hpp
        Importers/ITKImageImporter.hpp
    )
endif()

include_directories(
    
    Data/
    Importers/
    Exporters/
    Streamers/
    Algorithms/
    Algorithms/SurfaceExtraction/
    Algorithms/GaussianSmoothingFilter/
    Algorithms/SeededRegionGrowing/
    Data/Access/
    Visualization/
    Visualization/ImageRenderer/
    Visualization/SliceRenderer/
    Visualization/SurfaceRenderer/
    Visualization/VolumeRenderer/
)

# Headers for Qt objects
set(HEADERS 
    Visualization/View.hpp
    Visualization/WindowWidget.hpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFAST_ROOT_DIR=\\\"${PROJECT_SOURCE_DIR}/../\\\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFAST_SOURCE_DIR=\\\"${PROJECT_SOURCE_DIR}/\\\"")


add_subdirectory(OpenCLUtilityLibrary)
find_package(OpenCLUtilityLibrary PATHS "${FAST_BINARY_DIR}/OpenCLUtilityLibrary" REQUIRED)
include(${OpenCLUtilityLibrary_USE_FILE})

# Qt5
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Declarative REQUIRED)

## Boost
find_package(Boost COMPONENTS system thread iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})
link_directories(${Boost_LIBRARY_DIRS})

## GLEW
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(examples ${SOURCES} ${HEADERS_MOC})
add_library(FAST ${SOURCES} ${HEADERS_MOC})

# Qt5
qt5_use_modules(FAST Widgets OpenGL Declarative)
qt5_use_modules(examples Widgets OpenGL Declarative)
target_link_libraries(FAST Qt5::Widgets)
target_link_libraries(examples Qt5::Widgets)

set(LIBRARIES
    OpenCLUtilityLibrary
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${GLEW_LIBRARY}
)

if(VTK_INTEROP)
    find_package(VTK REQUIRED)
    include(${VTK_USE_FILE})
    message("-- Enabling VTK interoperability")
    set(LIBRARIES ${LIBRARIES} ${VTK_LIBRARIES})
endif()

if(ITK_INTEROP)
    find_package(ITK REQUIRED)
    include(${ITK_USE_FILE})
    message("-- Enabling ITK interoperability")
    set(LIBRARIES ${LIBRARIES} ${ITK_LIBRARIES})
endif()

target_link_libraries(examples ${LIBRARIES})
target_link_libraries(FAST ${LIBRARIES})

add_subdirectory(Tests)
