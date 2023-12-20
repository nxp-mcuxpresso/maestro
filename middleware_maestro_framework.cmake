#Description: Maestro Audio Framework Middleware; user_visible: True
include_guard(GLOBAL)
message("middleware_maestro_framework component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

#OR Logic component
if(${MCUX_DEVICE} STREQUAL "RW612")
    include(middleware_maestro_framework_streamer_RW612)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1166_cm7")
    include(middleware_maestro_framework_streamer_MIMXRT1166_cm7)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1052")
    include(middleware_maestro_framework_streamer_MIMXRT1052)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1064")
    include(middleware_maestro_framework_streamer_MIMXRT1064)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1062")
    include(middleware_maestro_framework_streamer_MIMXRT1062)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1042")
    include(middleware_maestro_framework_streamer_MIMXRT1042)
endif()
if(${MCUX_DEVICE} STREQUAL "MIMXRT1176_cm7")
    include(middleware_maestro_framework_streamer_MIMXRT1176_cm7)
endif()
if(${MCUX_DEVICE} STREQUAL "LPC55S69_cm33_core0")
    include(middleware_maestro_framework_streamer_LPC55S69_cm33_core0)
endif()

include(middleware_maestro_framework_doc)
