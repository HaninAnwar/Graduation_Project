/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Thu Jul  6 22:03:28 2023
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[27] = {
  0x3ea6c4863e528bc3U, 0xbf324067bccc49dcU, 0x3d99bc5c3e95bbc2U, 0xbe57dabdbf3a40a4U,
  0xbf9a804fbebe14d9U, 0xbf4313d53f141015U, 0xbe67df99bf5909e3U, 0xbf386051bf893fa5U,
  0x3e9590d2bd4567c0U, 0x3fa4ed613f8453b9U, 0x3f853dc43e1439c4U, 0x3f88d8573df48bc0U,
  0xbfc92475bec4eadaU, 0x3eaf95c0bf6f1a5cU, 0xbf253254bec31c40U, 0xbe5f6105bee5ad60U,
  0x3f478ab6bee819edU, 0xbec8a6f13f632985U, 0xbf3b5a7cbfa5eee4U, 0xbfcadd223f8098b8U,
  0xbfba6c783e4fa1cfU, 0xbf785806bebe5172U, 0xbd811b883f6989bfU, 0x3f9ee5a83ead39beU,
  0x3ca37a82bee3610eU, 0x3ee179f33ef16ca4U, 0xbf64a2f1U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

