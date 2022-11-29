/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

//! \addtogroup asi_dffi
//! @{
//
// Copyright (c) 2004-2005 SigmaTel, Inc.
//
//! \file mp3_id3v2.h
//! \brief Contains functionality to read MP3 metadata.

#ifndef __ID3V2_H
#define __ID3V2_H

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
int32_t Mp3MetaDataReadID3V2(CCI_Ctx *Ctx, int32_t iIgnored, file_meta_data_t *pMetaData);

#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}