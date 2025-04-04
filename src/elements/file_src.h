/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FILESRC_H
#define FILESRC_H

/*!
 *  @file    file_src.h
 *  @brief   This is the header file for the file source element.
 */

#include "streamer_element.h"

#ifdef FILE_ENABLE_FATFS
#include "ff.h"
#endif // FILE_ENABLE_FATFS
#include "file_utils.h"

/*!
 * Default file read size
 */
#define FILESRC_DEFAULT_CHUNK_SIZE 1024

/*!
 * File Source Element data structure
 */
struct _ElementFileSrc
{
    int32_t (*change_state)(struct _StreamElement *, PipelineState); /*!< @brief Change state function pointer */
    int32_t (*set_property)(struct _StreamElement *,
                            uint16_t prop,
                            uintptr_t val); /*!< @brief Set property function pointer */
    int32_t (*get_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint64_t *val_ptr); /*!< @brief Get property function pointer */
    void *parent;                               /*!< @brief Parent pointer */
    int8_t key;                                 /*!< @brief Key */
    int8_t depth;                               /*!< @brief Depth */
    PipelineState state;                        /*!< @brief Pipeline state */
    StreamElementType type;                     /*!< @brief Element type */
    PadSrc src_pad[MAX_NUM_PADS];               /*!< @brief Source pads */
    PadSink sink_pad[MAX_NUM_PADS];             /*!< @brief Sink pads */
    struct _StreamElement *sibling;             /*!< @brief Sibling pointer */

    int32_t fd;                                 /*!< @brief File descriptor */
    char location[MAX_LOCATION_PATH_LENGTH];    /*!< @brief File location */
    int8_t *buffer;                             /*!< @brief Buffer pointer */
    uint32_t chunk_size;                        /*!< @brief Chunk size */
    uint8_t end_of_stream;                      /*!< @brief End of stream flag */
    uint8_t time_seekable;                      /*!< @brief Time seekable flag */
    uint8_t byte_seekable;                      /*!< @brief Byte seekable flag */
    uint32_t read_position;                     /*!< @brief Last read offset */
    uint32_t duration;                          /*!< @brief File duration */
    uint32_t size;                              /*!< @brief File size */
    uint8_t file_type;                          /*!< @brief File type */
    uint32_t sample_rate;                       /*!< @brief Sample rate */
    uint8_t num_channels;                       /*!< @brief Number of channels */
    uint8_t bit_width;                          /*!< @brief Bit width */
    uint32_t g_read_position[MAX_NUM_PADS];     /*!< @brief Global read position */
    int32_t g_fd[MAX_NUM_PADS];                 /*!< @brief Global file descriptor */
};
typedef struct _ElementFileSrc ElementFileSrc;

/*!
 * Stream builder decoder table
 * This struct is for the decoder file formats table definition
 */
typedef struct
{
    char *extension;  /*!< @brief Extension name */
    int decoder_type; /*!< @brief Relate with DecoderType */
    int parser_type;  /*!< @brief Relate with ParserType */
} StreamerFileSrcConfig;

/*!
 * @ingroup coreapi
 * @{
 */

/*
 * FUNCTION PROTOTYPES
 */

/*!
 *
 * @brief this function initializes the file source element and source pad.
 * All the pad's handlers are intialized.
 *
 * @param element pointer to element
 *
 * @returns StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesrc_init(StreamElement *element);

/*!
 *
 * @brief    Set a destination file to be read by file source element
 *
 * @param  element element handle
 * @param  path pointer to a file path
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesrc_set_location(ElementHandle element, char *path);

/*!
 *
 * @brief    This function gets the location of the file
 *
 * @param  element element handle
 * @param  pp_path location path
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesrc_get_location(ElementHandle element, char **pp_path);

/*!
 *
 * @brief    Set the chunk size in which the file source sends the data
 * in push mode
 *
 * @param  element element handle
 * @param  chunk_size chunk size
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size);
/*!
 *
 * @brief    Get the chunk size in which the file source sends the data
 * in push mode
 *
 * @param  element element handle
 * @param  chunk_size chunk size
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesrc_get_push_chunk_size(ElementHandle element, uint64_t *chunk_size);
/*!
 *
 * @brief    Get the file source decoder and parser type
 *
 * @param  filename name of the file
 * @param  config file source information
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_GENERAL means either file does not contain an extension or
 * the file type is not supported
 *
 */
int32_t file_src_get_decoder_type(char *filename, StreamerFileSrcConfig *config);

/*! @} */

#endif
