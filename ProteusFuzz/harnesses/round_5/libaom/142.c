// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;
    const aom_codec_iface_t *iface;

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize the decoder context
    err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_decode
    err = aom_codec_decode(&ctx, Data, Size, NULL);
    // We ignore the return value here as we are fuzzing, but in production code,
    // we would handle errors appropriately.

    // Attempt to retrieve frame information using control commands
    // These commands are typically called after decoding to get frame-specific data.
    int val;
    
    // AOMD_GET_LAST_QUANTIZER
    err = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &val);
    (void)err; // Suppress unused variable warning

    // AOMD_GET_SB_SIZE
    aom_superblock_size_t sb_size;
    err = aom_codec_control(&ctx, AOMD_GET_SB_SIZE, &sb_size);
    (void)err;

    // AOMD_GET_FRAME_FLAGS
    int frame_flags;
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)err;

    // AOMD_GET_LAST_REF_UPDATES
    int ref_updates;
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &ref_updates);
    (void)err;

    // AOMD_GET_FRAME_CORRUPTED
    int frame_corrupted;
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    (void)err;

    // Cleanup
    cleanup_decoder(&ctx);

    return 0;
}