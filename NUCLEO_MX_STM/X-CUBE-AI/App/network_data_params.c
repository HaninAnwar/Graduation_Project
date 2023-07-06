/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Thu Jul  6 16:00:33 2023
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
const ai_u64 s_network_weights_array_u64[21] = {
  0xbf1e58ebbf81474eU, 0x3ecf8212bf939a9cU, 0xbf70789d3dfd9630U, 0xbea18b71bde94f5aU,
  0xbecfe36cbf80a712U, 0xbf8abdfbbede68a7U, 0xbdbafd2e3d6b8dedU, 0x3df8d900bef780f2U,
  0x3f0860313f3c2d9bU, 0x3f91ef453f2a4e05U, 0x3ebedc9e3ec9c3cdU, 0x3f7d0483bf393fc3U,
  0x3ccd16183f727121U, 0xbf0754c43f666c08U, 0x3f7ba7d1bdc6cd08U, 0x3ecd8cf33eeb0992U,
  0x3f271564be45c917U, 0x3fb895773f27e04dU, 0x3f0bc1183f35b464U, 0xbf7028c2bf928257U,
  0xbf24bdb6U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

