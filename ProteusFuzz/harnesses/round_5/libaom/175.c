// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;

    // Initialize encoder
    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Extract parameters from fuzz input
    int param1 = (Size >= 1) ? Data[0] : 0;
    int param2 = (Size >= 2) ? Data[1] : 0;
    int param3 = (Size >= 3) ? Data[2] : 0;
    int param4 = (Size >= 4) ? Data[3] : 0;
    int param5 = (Size >= 5) ? Data[4] : 0;
    int param6 = (Size >= 6) ? Data[5] : 0;

    // Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, param1);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, param2);

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, param3);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, param4);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, param5);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, param6);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}