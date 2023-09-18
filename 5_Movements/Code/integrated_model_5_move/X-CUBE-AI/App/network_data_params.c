/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Aug 15 22:11:30 2023
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
const ai_u64 s_network_weights_array_u64[205] = {
  0xbdc072193f0ba547U, 0x3f214249be816373U, 0x3f1fb18abec46ca1U, 0x3f0a9cfd3cbacddeU,
  0x3f07b9ff3f35e835U, 0x3f0e09b6c01b6379U, 0xc0231e723f3caa31U, 0xc007f09c3ed97a95U,
  0xbfb2899f3e81fe83U, 0x3ec6cbcdbfbae3d6U, 0x3eb0a084401c33fbU, 0x3fe16d98bf05c126U,
  0x3f3bec6bbe76531fU, 0xbff92daa3d2dababU, 0x3f02a8bebd920a2aU, 0xbf8c7a1fbf310b0cU,
  0xbe3c9c293e327770U, 0x3ce61e4cbdd40c26U, 0x3e40b4273f146f61U, 0xbda448ca3f585091U,
  0xbe9e2f9e3f97158eU, 0x3f868d323f3ab614U, 0x3fdae3adc00c5a37U, 0x4027acebbf7efdfcU,
  0xbf3220cd3f1fd51eU, 0x3f6cf253becc2233U, 0xbf8286f1bf756dd1U, 0xbf3f593dbf3e72feU,
  0xbf5525f8bed4643cU, 0xbff115a03f6869cbU, 0x3ea5b528beda3eabU, 0x3e6c802bbecc264fU,
  0xbe81ad1cbe55daf5U, 0x3e93d465bee4dec8U, 0xbda063603f2d5ed1U, 0x3b6dc599bf00dbe6U,
  0xbe263b57bf620fb1U, 0x3dea0cf3be4afa27U, 0x3ed6dc63bde3fd23U, 0x3e9bffcebf145b02U,
  0xbe26ad843f17ba43U, 0x3d83c9253d68eff8U, 0x3e31d83dbf277635U, 0x3ecb4f9fbe568304U,
  0x3d4082b3bea4b5a3U, 0x3ec2f28abefcf2b2U, 0x3e5a1f5b3e4eee76U, 0x3e9b52393e367a91U,
  0xbd3ce582bf56d210U, 0xbe60da2c3ed26a3eU, 0xbe64ff8cbe23c377U, 0x3f374c243e8fd166U,
  0xbeae8bf23ee5d36bU, 0xbe0212dc3ef39eb9U, 0xbf530756be6b84eaU, 0x3ea704d53dc2ca0bU,
  0xbf6d8f02bd8f5440U, 0xbf4f9408bd6c2d2cU, 0x3d461959be0de1a7U, 0xbe604d8fbf44199dU,
  0x3e3273893da103f7U, 0xbf11baa73e972f63U, 0x3d97e0283dcea884U, 0xbccd7a0dbead37a2U,
  0x3e8a555abccb4d24U, 0x3f0be0943ed22a0aU, 0x3ef99b11bef9292aU, 0x3df063543eb201aaU,
  0xbf5a99dd3ee70109U, 0xbf23d5df3e858234U, 0xbdfb30673bc8071fU, 0xbf433ea33d4b5387U,
  0xbef117f23ea851b4U, 0xbe1ce5463e0f0170U, 0x3f1c0f85bec1ae40U, 0x3f225f2c3ed1c1b4U,
  0x3e54a415bdb28b13U, 0xbd983d00be3209ccU, 0x3eced7e13e88a6d9U, 0xbf1796da3e6f8d92U,
  0xba8833e13ec3b71cU, 0x3e469d763f2cc3c4U, 0xbe7647d03eb5c4a3U, 0x3f0e4dcc3d42d141U,
  0x3e9b7a443e2e3758U, 0xbe9f9f253e256f52U, 0xbf2938913f130ac8U, 0xbf2f18d33ec6779cU,
  0x3f37af2abe81885aU, 0xbefcee763c9a13c8U, 0xbf4b8dea3f1586bcU, 0x3f0d44b43ea25ef3U,
  0xbf2d14edbe328effU, 0xbef99547bd8b0eb8U, 0x3edc8cad3e7f8b4fU, 0xbf129d97bd88ff98U,
  0xbf3245623f1c4abcU, 0x3ef6ace63c5ba17eU, 0x3ecf08d8bf298abcU, 0x3f29ecbbbd2cbdb6U,
  0xbeb0a0683f10cf22U, 0x3f1ec29dbd68b362U, 0x3f29ae653f13e4deU, 0xbec660e43de4da8bU,
  0x3f0563bb3f1d4b8cU, 0xbe3ea7a33f0690eaU, 0xbe30d0163d45abd8U, 0x3ed249ec3d7b4e38U,
  0xbe4d44353f262554U, 0xbe08e6503ea277f2U, 0x3ef9d1023ec90430U, 0xbe4d7cc63ea970b1U,
  0xbe5346ae3ee319e1U, 0x3f30e52a3f0d7b38U, 0x3ec8144ebb883af8U, 0xbe9731203dcc88e4U,
  0xbed909a23e379687U, 0xbf4024d9bdbecb2dU, 0x3e551013bf1f2e4fU, 0xbdada454bf537be6U,
  0xbd8656c9bee65845U, 0xbf13c466bee0b0edU, 0x3f51dbbbbf03a1f9U, 0x3f3ee69c3f174095U,
  0x3d78758b3e3b8b5eU, 0xbecd0d213f206584U, 0x3e59388f3e21c4e4U, 0xbf1c6fa3bf153b08U,
  0x3e4483493e5acd2bU, 0xbd6925753c532bbeU, 0x3f1d94293f05344dU, 0x3ed2fe253ed321e0U,
  0x3f86ec07beaf6c61U, 0xbedfed21bef088c4U, 0xbf620f8fbe7d00b6U, 0xbe91145ebf22bfeeU,
  0xbd5919073db431faU, 0x3f34582dbdbf33e5U, 0x3ecec6d03f167aa8U, 0x3f84c7fc3e787fa7U,
  0x3f6e5d1bbf9619f2U, 0x3d78feba3e94a434U, 0x3f07cf423f0f03d7U, 0x3ee31386bed498ccU,
  0x3f847ce1bec54245U, 0xbee8c4633ec6e150U, 0xbe5af1e43ebd4409U, 0x3efe49653f2b0b69U,
  0xbedda0213e300751U, 0x3f06089e3ebca7f0U, 0x3e8f7bfc3ee78a83U, 0xbe52a1ecbe3efcd3U,
  0x3e88bea1bdd2a514U, 0x3f3e069e3ed84e70U, 0xbe416fa3be5c9c82U, 0xbee983943f4e60deU,
  0xbec5d292bd870a35U, 0xbedb3254bf178d22U, 0xbdd956863dfa14c6U, 0xbec48e8fbd8dc383U,
  0xbec2b1633ef9e7aaU, 0x3eb44a9cbe19f8b6U, 0xbfa69d90be3b591dU, 0xbe1b4c183f3140fdU,
  0x3f9fe6eabe44e29aU, 0xbed9f4483f98fa34U, 0xbee6a5d6be2dc508U, 0x3f298e903dd3307bU,
  0x3efb98d9bda72f2dU, 0xbe3027e4be074a22U, 0xbf14bcc63fc467ceU, 0xbdf7ea0cbe0bc17dU,
  0xbf0305a4bef27d43U, 0x3cbe3dfcbf002eedU, 0x3dc18dd4bef9ac55U, 0xbee06440bd08bf73U,
  0x3ee7553c3bf8a332U, 0x3f03b690bea9f201U, 0x3f3236d3bdca51dcU, 0xbdaddb7f3ec29eebU,
  0x3d961bc03e1679e7U, 0x3eb09ed03efe9129U, 0x3f339b963b8df68eU, 0x3ed3a4c43ef28923U,
  0x3f0b4fd0bfbd64aaU, 0xbfc7d1453fc8ad9dU, 0xbec56f493ed85da1U, 0xbf831c12bf76e6a4U,
  0xbf08da82be74e36fU, 0xbdfb507abed0b7b6U, 0x3e4dc2993d9b393bU, 0xbec551063ec99e5fU,
  0x3fd627cb3f56a3aaU, 0x3f2f5cecbe03e1cbU, 0xbf1d16803ec2dab5U, 0x3f413728bf18f0b5U,
  0x3f05eba43e4af72dU, 0x3f4ce3373f0d28eaU, 0x3ddd234b3efad12dU, 0xbd6459b33f02c2d9U,
  0xbe6d5a06bee223ceU, 0x3eb164d7bcf6c031U, 0x3f0c9fe93ec56f01U, 0x3ef44f473e8345fcU,
  0xbef74f53be373770U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

