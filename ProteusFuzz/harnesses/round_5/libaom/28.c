// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int fuzz_decode(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;
    int ret = 0;

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 32;
    cfg.h = 32;

    // Get decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize decoder
    err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Feed data to decoder
    if (Size > 0) {
        err = aom_codec_decode(&ctx, Data, Size, NULL);
        if (err != AOM_CODEC_OK && err != AOM_CODEC_ERROR) {
            aom_codec_destroy(&ctx);
            return 0;
        }
    }

    // Try to get a frame
    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame if needed
        (void)frame;
    }

    // Invoke target API functions
    // Attempt 1: AOMD_GET_ALTREF_PRESENT
    int altref_present;
    err = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
    (void)err;

    // Attempt 2: AOMD_GET_FRAME_FLAGS
    int frame_flags;
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)err;

    // Attempt 3: AOMD_GET_LAST_REF_UPDATES
    // REF_FRAMES is typically defined as 4 in aom_codec.h or aom_internal.h
    // We use a safe constant size based on standard AV1 reference frames
    int last_ref_updates[4];
    memset(last_ref_updates, 0, sizeof(last_ref_updates));
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, last_ref_updates);
    (void)err;

    // Attempt 4: AOMD_GET_FRAME_CORRUPTED
    int frame_corrupted;
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    (void)err;

    // Attempt 5: AOMD_GET_LAST_REF_USED
    int last_ref_used[4];
    memset(last_ref_used, 0, sizeof(last_ref_used));
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, last_ref_used);
    (void)err;

    // Attempt 6: AV1D_GET_TILE_COUNT
    int tile_count;
    err = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    (void)err;

    aom_codec_destroy(&ctx);
    return ret;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz the decoder
    fuzz_decode(Data, Size);

    return 0;
}