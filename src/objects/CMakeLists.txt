project(objects)

add_library(${PROJECT_NAME}
    SHARED
        Asset.h
        Asset.cpp
        Object.h
        Object.cpp
        interface/ITransformable.h
        interface/Transformable.cpp
        interface/Transformable.h
        interface/IVisible.h
        light/ILightSource.h
        light/IPointLight.h
        light/DirectLight.h
        light/DirectLight.cpp
        light/PointLight.h
        light/PointLight.cpp
        light/SpotLight.h
        light/SpotLight.cpp
        light/LightSource.cpp
)

add_library(edu::objects ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        assimp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GLAD_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)
