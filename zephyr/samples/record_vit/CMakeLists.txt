# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

add_compile_definitions(FSL_OSA_TASK_ENABLE=1)
set(CONFIG_USE_component_osa_zephyr true)

# Explicitelly enforcing to find specific Zephyr installation
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(maestro_record_vit)

# Target_sources must be specified directly after find_package. Target "app"
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/app_streamer_element_pcm.c)
target_sources(app PRIVATE src/app_streamer_element_vit.c)

target_include_directories(app PRIVATE src/)