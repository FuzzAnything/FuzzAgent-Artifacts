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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cfg.cpu_used = 0; to fix compilation error on older libaom versions

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Feed some data to the decoder to potentially trigger state changes
    if (size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!init_decoder(&ctx, Data, Size)) {
        return 0;
    }

    int ret = 0;
    int32_t int_val;
    aom_codec_err_t err;

    // 1. aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT
    err = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &int_val);
    (void)err; (void)int_val;

    // 2. aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &int_val);
    (void)err; (void)int_val;

    // 3. aom_codec_control_typechecked_AV1D_GET_TILE_COUNT
    err = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &int_val);
    (void)err; (void)int_val;

    // 4. aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS
    err = aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &int_val);
    (void)err; (void)int_val;

    // 5. aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &int_val);
    (void)err; (void)int_val;

    // 6. aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &int_val);
    (void)err; (void)int_val;

    cleanup_codec(&ctx);
    return 0;
}