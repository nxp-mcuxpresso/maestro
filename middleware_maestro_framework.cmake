#Description: Maestro Audio Framework Middleware; user_visible: True
include_guard(GLOBAL)
message("middleware_maestro_framework component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}/inc
)


include(middleware_maestro_framework_doc)
include(middleware_maestro_framework_streamer)
