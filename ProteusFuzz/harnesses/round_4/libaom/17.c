// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

// Helper to safely cleanup codec context
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment: Get decoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // 2. Initialize codec context
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: allow_highbitdepth does not exist in this version, using allow_lowbitdepth or omitting.
    // High bitdepth is usually default or controlled by other flags in newer versions.
    // We leave it as default (0) to avoid compilation error.
    // cfg.allow_lowbitdepth = 0; // Default is usually fine.

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Fuzzing Logic

    // Test aom_codec_peek_stream_info (Does not require full decode, just header parsing)
    aom_codec_stream_info_t si_peek;
    memset(&si_peek, 0, sizeof(si_peek));
    si_peek.is_annexb = 0; // Try default
    if (Size > 0) {
        aom_codec_peek_stream_info(iface, Data, Size, &si_peek);
        
        // Try with annexb = 1
        si_peek.is_annexb = 1;
        aom_codec_peek_stream_info(iface, Data, Size, &si_peek);
    }

    // Test aom_codec_decode
    // We pass the fuzzed data directly. If it's not valid AV1, it should return an error gracefully.
    aom_codec_err_t decode_res = aom_codec_decode(&ctx, Data, Size, NULL);
    
    // Fixed: AOM_CODEC_SUCCESS is not a standard error code in aom_codec_err_t enum for this context.
    // We check for AOM_CODEC_OK. Other states might be handled by checking if frames are available.
    if (decode_res == AOM_CODEC_OK) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        
        // Iterate over frames
        while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            // Access frame data to ensure it's valid
            if (img->planes[0]) {
                // Do nothing, just touching memory to trigger potential issues
            }
            // aom_img_free is typically handled by the codec context internally for decoded frames
        }
    }

    // Test aom_codec_get_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);

    // Note: aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED and 
    // aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO are skipped 
    // as their signatures were not provided in the prompt.

    // 4. Cleanup
    cleanup_codec(&ctx);

    return 0;
}