// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "aom/aom_decoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_encoder.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom decoding APIs.
 * Targets:
 * - aom_codec_dec_init_ver
 * - aom_codec_decode
 * - aom_codec_get_stream_info
 * - aom_codec_peek_stream_info
 * - aom_codec_control (for AV1D_GET_FRAME_HEADER_INFO and AV1D_GET_FRAME_SIZE)
 */

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_flags_t flags = 0;

    // Initialize decoder
    aom_codec_err_t init_res = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
    if (init_res != AOM_CODEC_OK) {
        // Initialization failed, try peek stream info without decoder context
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.is_annexb = 1;
        aom_codec_peek_stream_info(iface, Data, Size, &si);
        return 0;
    }

    // Try to get stream info before decoding (might fail, but safe to call)
    aom_codec_stream_info_t si_pre;
    memset(&si_pre, 0, sizeof(si_pre));
    aom_codec_get_stream_info(&ctx, &si_pre);

    // Decode the input data
    aom_codec_err_t decode_res = aom_codec_decode(&ctx, Data, Size, NULL);

    // Try to get stream info after decoding
    aom_codec_stream_info_t si_post;
    memset(&si_post, 0, sizeof(si_post));
    aom_codec_get_stream_info(&ctx, &si_post);

    // Try to control the decoder to get frame header info (if supported)
    aom_codec_err_t ctrl_res_header = aom_codec_control(&ctx, AV1D_GET_FRAME_HEADER_INFO, NULL);
    (void)ctrl_res_header;

    // Try to control the decoder to get frame size (if supported)
    aom_codec_err_t ctrl_res_size = aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, NULL);
    (void)ctrl_res_size;

    // Also try peek_stream_info with the same data for variety
    aom_codec_stream_info_t si_peek;
    memset(&si_peek, 0, sizeof(si_peek));
    si_peek.is_annexb = 1;
    aom_codec_peek_stream_info(iface, Data, Size, &si_peek);

    // Cleanup
    cleanup_decoder(&ctx);

    return 0;
}