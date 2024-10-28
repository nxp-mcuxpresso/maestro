if(CONFIG_MAESTRO_OSA_USE)

  set(OSA_DIR ${ZEPHYR_CURRENT_MODULE_DIR}/zephyr/osa/)
  set(OSA_LIB OSA)
  set(FSL_OSA_TASK_ENABLE 1)

  zephyr_library_named(${OSA_LIB})

  zephyr_library_sources(
    ${OSA_DIR}/fsl_os_abstraction_zephyr.c
  )
  zephyr_include_directories(
    ${OSA_DIR}
  )

  set_property(TARGET ${OSA_LIB} PROPERTY C_STANDARD 11)

  message("Maestro OSA Zephyr layer was included.\r\n")

else()

  # TODO comment/remove
  message("Maestro OSA not included!\r\n")

endif() # CONFIG_MAESTRO_OSA_USE
