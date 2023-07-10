/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Mon Jul 10 22:10:32 2023
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
const ai_u64 s_network_weights_array_u64[90] = {
  0x3e03ed283e93b1cdU, 0x3ef33aa5bec637e6U, 0xbe33d95abe85b27eU, 0xbf796c6000000000U,
  0x3fc63319bd83eb58U, 0x3ed7ac68bfdfe8a9U, 0x3f12fc29bf1a6268U, 0xbf768084be6b4b8fU,
  0x3eea6757bf99803bU, 0xbf6f559bbe1ade02U, 0xbff83fe93e4625c0U, 0xbf91db083f91f3d0U,
  0xc01120433f01e4deU, 0xbe90b2743e815487U, 0xbeef8e63bf424623U, 0xbf041f2dbfa25e25U,
  0x3fb08b57c01d836cU, 0xbf61868abea0b9c9U, 0xbf9c274f3f81ee4dU, 0x3f680d56bfbb6400U,
  0x3c9e0e443f778ec6U, 0x3e4677323eee38d2U, 0x3f510d80bd595484U, 0x3f2461a13e141ed6U,
  0xbea915993ec8b8d9U, 0xbe6d5c913ea85234U, 0x3d26ba80be9846baU, 0xbeacb07cbf982b50U,
  0x3d3963c2beac7b23U, 0xbeee3f00be7e4ef0U, 0xbf654637bf1f0907U, 0xbf5137dc3e9e82caU,
  0xbec1998d3e8974cdU, 0x3ec7525a3d2a61c8U, 0xbf154e02bf161babU, 0xbf1b701fbec01432U,
  0x3c95f6f3be9a84c6U, 0xbe79fbc0bd542075U, 0x3dd44befbf1b9b85U, 0x3edee45e3d8e702cU,
  0xbca32d823f41fd79U, 0xbda8e5a4beef26c1U, 0x3f12a0c23ed4d472U, 0x3de14280be824194U,
  0x3f1963c1bf072fa2U, 0xbe57482d3f498e68U, 0xbdafb3bd3eea43c8U, 0xbef1a9a9bf2f7a8eU,
  0xbf18e68b3e64d28aU, 0xbf03a458be8d0586U, 0xbf653a2ebeba727eU, 0x3f079644be7a68a7U,
  0xbe9c0902bf27990eU, 0xbf10aa6c3f0a38a0U, 0x3e17aff63f03366fU, 0xbf8c0e2cbd4ece61U,
  0xbef52fe2bea947b8U, 0xbf211548be54eb90U, 0xbf10ddaabf1bb3f6U, 0xbe2d54ce3ed1a074U,
  0x3e617e56bf354d83U, 0x3ee6db8ebeb66e8fU, 0xbe7df2ea3d392636U, 0x3e714d863e34373aU,
  0xbe675acebdeff930U, 0x3f604882bd7a412cU, 0x3ea656e03e741aaaU, 0xbf12515d3f92720cU,
  0x3f94e7f93ef1ebefU, 0xbd9bcccfbe797b43U, 0x3ee0cc343ea5bbbaU, 0xbe8a1b3dbf37ec33U,
  0xbf5214eabe0b01e0U, 0xbec65f6c3f0854c3U, 0x3e8d16c8bf38dd73U, 0x3f220d143f7e80baU,
  0xbf1561a4bdb29b5eU, 0x3e8b293a3f18f3edU, 0xbe65548dbcc65983U, 0xbf6081c2be89a742U,
  0x3f04eb0c3f471e97U, 0x3e37e70abf361a7cU, 0xbe9759803f504136U, 0xbe0b73843f40936aU,
  0x3ef967d1bd0e31f4U, 0x3f5c3974bf0d92feU, 0xbf10afffbf20e4ebU, 0xbe2149e83f040002U,
  0xbf21b18c3e27cb04U, 0x3cd533c0U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

