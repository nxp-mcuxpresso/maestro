/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  app_Streamer_element_pcm.c
 *  @brief Implementation of application dependent audio source - pcm element (microphone)
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(demo_appStreamerElm_pcm, LOG_LEVEL_WRN);
#include <zephyr/audio/dmic.h>

#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "app_streamer_element_pcm.h"

#define BLOCK_SIZE(_sample_rate, _number_of_channels) \
	(BYTES_PER_SAMPLE * (_sample_rate / 10) * _number_of_channels)

#define DEMO_RECORD_FRAME_LENGTH_MS (30u)
#define DEMO_RECORD_DMA_BUFFER_COUNT (6u)

struct maestro_pcm_config
{
    uint32_t sample_rate;
    uint32_t bit_width;
    uint8_t num_channels;
    bool transfer;
    bool dummy_tx;
    int volume;
};
static struct maestro_pcm_config s_record_config = {0};

static struct pcm_stream_cfg s_pcm_stream_cfg = {0};
static struct dmic_cfg s_dmic_cfg = {0};
static struct k_mem_slab s_memSlab;
static uint8_t * s_memSlab_buffer;
struct device const * s_dev_dmic = DEVICE_DT_GET(DT_NODELABEL(dmic0));

volatile static int8_t PDM_started = 0;     /* Indicates that the PDM transfer has already started. */

void streamer_pcm_init(void)
{
    LOG_DBG("PCM Init\r\n.");

    return;
}

int streamer_pcm_rx_open(uint32_t num_buffers)
{
    PDM_started = 0;

    /* Check if dmic peripheral is ready */
	if (!device_is_ready(s_dev_dmic))
    {
		LOG_ERR("Dmic device %s is not ready", s_dev_dmic->name);
		return -1;
	}

    return 0;
}

void streamer_pcm_rx_close(void)
{
    LOG_DBG("Streamer close");

    if(dmic_trigger(s_dev_dmic, DMIC_TRIGGER_STOP) != 0)
    {
        __ASSERT(false, "DMIC cannot be stopped");
        LOG_ERR("PCM Close Error");
    }
}

int streamer_pcm_read(uint8_t *data, uint32_t size)
{
    int retVal          = -1;
    uint32_t readSize_B = 0;
    void *readBuffer    = NULL;

    if (PDM_started == 0)
    {
        retVal = dmic_trigger(s_dev_dmic, DMIC_TRIGGER_START);
        if (retVal < 0)
        {
            LOG_ERR("START trigger failed: %d", retVal);
        }
        PDM_started = 1;

        return 0;
    }

    retVal = dmic_read(s_dev_dmic, 0, &readBuffer, &readSize_B, 1000);
    LOG_DBG("Read called. Size to read: %u. \r\n", readSize_B);
	if (retVal < 0)
    {
		LOG_ERR("Read failed: %d", retVal);
        return -1;
	}

    if(size < readSize_B)
    {
        LOG_ERR("Recorded data loss and skew. Size: %u, ReadSIze: %u", size, readSize_B);
    } else if(size > readSize_B)
    {
        LOG_ERR("Returned wrong data located in padding and skew. Size: %u, ReadSIze: %u", size, readSize_B);
    } else
    {
        memcpy(data, readBuffer, size);
    }

    LOG_DBG("# SLAB FREE. Before: %u\r\n", s_memSlab.info.num_used);
    k_mem_slab_free(&s_memSlab, readBuffer);
    LOG_DBG("# SLAB FREE. After: %u\r\n", s_memSlab.info.num_used);

    return 0;
}

int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume)
{
    int retVal;

    if(num_channels != 1)
    {
        LOG_ERR("CUrrently Supported only 1 channel.");
        return -1;
    }

    s_record_config.bit_width    = bit_width;
    s_record_config.sample_rate  = sample_rate;
    s_record_config.num_channels = num_channels;
    s_record_config.dummy_tx     = dummy_tx;

    uint32_t bytesPerSecond  = ((bit_width/8) * sample_rate * num_channels);
    uint32_t bytesPerBuffer  = (bytesPerSecond * DEMO_RECORD_FRAME_LENGTH_MS / 1000);
    uint32_t bytesPerBuffers = (bytesPerBuffer * DEMO_RECORD_DMA_BUFFER_COUNT);

    /* PCM Streams configuration */
    s_pcm_stream_cfg.pcm_width  = s_record_config.bit_width;
    s_pcm_stream_cfg.pcm_rate   = s_record_config.sample_rate;
    s_pcm_stream_cfg.block_size = bytesPerBuffer;
    s_pcm_stream_cfg.mem_slab   = & s_memSlab;

    s_memSlab_buffer = k_aligned_alloc(32, bytesPerBuffers);
    if(s_memSlab_buffer == NULL)
    {
        LOG_ERR("Allocation of memory slab buffer failed");
        return -1;
    }

    retVal = k_mem_slab_init(& s_memSlab, s_memSlab_buffer, bytesPerBuffer, DEMO_RECORD_DMA_BUFFER_COUNT);
    if(retVal != 0)
    {
        LOG_ERR("Initialization of memory slab failed.");
        return -1;
    }

    s_dmic_cfg.streams = &s_pcm_stream_cfg;

    /* Configuration of DMIC I/O */
    s_dmic_cfg.io.max_pdm_clk_dc = 60;
    s_dmic_cfg.io.max_pdm_clk_freq = 3500000;
    s_dmic_cfg.io.min_pdm_clk_dc = 40;
    s_dmic_cfg.io.min_pdm_clk_freq = 1000000;
    s_dmic_cfg.io.pdm_clk_skew = 0;
    s_dmic_cfg.io.pdm_data_pol = 0;

    /* Mapping Channels to Streams */
    s_dmic_cfg.channel.req_num_chan = s_record_config.num_channels;
    s_dmic_cfg.channel.req_num_streams = 1;

    s_dmic_cfg.channel.req_chan_map_hi =
        dmic_build_channel_map(0, 0, PDM_CHAN_LEFT);

    retVal = dmic_configure(s_dev_dmic, &s_dmic_cfg);
	if (retVal < 0)
    {
		LOG_ERR("Failed to configure the driver: %d", retVal);
		return -1;
	}

    return 0;
}

void streamer_pcm_getparams(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
    *sample_rate  = s_record_config.sample_rate;
    *bit_width    = s_record_config.bit_width;
    *num_channels = s_record_config.num_channels;
}

int streamer_pcm_mute(bool mute)
{
    LOG_DBG("Streamer Mute");
    return 0;
}

int streamer_pcm_set_volume(int volume)
{
    __ASSERT(false, "Streamer Mute called!");

    return 0;
}
