cmake_minimum_required(VERSION 3.20.0)

add_compile_definitions(FSL_OSA_TASK_ENABLE=1)
set(CONFIG_USE_component_osa_zephyr true)

# set(CONFIG_NXP_OSA true)
# set(CONFIG_NXP_OSA_INCLUDE_TASK_API true)

#set(CMAKE_OBJECT_PATH_MAX 250)

# Explicitelly enforcing to find specific Zephyr installation
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(maestro_memtomem)

# Target_sources must be specified directly after find_package. Target "app"
#file(GLOB maestro_sources src/*.c)
#target_sources(app PRIVATE ${maestro_sources})
target_sources(app PRIVATE src/main.c)

zephyr_include_directories(${MAESTRO_OSA_DIR})

# set(MAESTRO_OSA_DIR ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa)


# Include target module directories?
# This library, ...?