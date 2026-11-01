// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Test aom_codec_peek_stream_info
    aom_codec_stream_info_t si = {0};
    // Initialize is_annexb based on fuzzing data to explore different states
    si.is_annexb = (Size > 0 && Data[0] % 2 == 0) ? 1 : 0;
    
    aom_codec_err_t peek_res = aom_codec_peek_stream_info(iface, Data, Size, &si);
    (void)peek_res;

    // Try to initialize a decoder context to test other control functions
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;

    aom_codec_err_t init_res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (init_res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Feed data to the decoder to potentially reach states where control functions are valid
    aom_codec_err_t decode_res = aom_codec_decode(ctx, Data, Size, NULL);
    (void)decode_res;

    // Attempt to get output frames to exercise more code paths
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *frame;
    while ((frame = aom_codec_get_frame(ctx, &iter)) != NULL) {
        if (frame->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process frame if needed, here just ensuring we enter the loop
            (void)frame->data.frame;
        }
    }

    cleanup_codec(ctx);
    return 0;
}