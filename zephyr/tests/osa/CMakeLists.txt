# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

add_compile_definitions(FSL_OSA_TASK_ENABLE=1)

# Explicitelly enforcing to find specific Zephyr installation
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(maestro_test_osa_cmake)

# Target_sources must be specified directly after find_package. Target "app"
target_sources(app PRIVATE src/main.c)

set_property(TARGET app PROPERTY C_STANDARD 11)