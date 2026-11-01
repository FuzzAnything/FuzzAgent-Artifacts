// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int i = 0;
    int ret;

    // AV1E_SET_SUPERBLOCK_SIZE: expects an int (0-2)
    if (size > i) {
        int val = data[i++] % 3;
        ret = aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
        (void)ret;
    }

    // AV1E_SET_QM_U: expects an int (0-1)
    if (size > i) {
        int val = data[i++] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_QM_U, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_QM: expects an int (0-1)
    if (size > i) {
        int val = data[i++] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
        (void)ret;
    }

    // AV1E_SET_MV_COST_UPD_FREQ: expects an int (0-255)
    if (size > i) {
        int val = data[i++];
        ret = aom_codec_control(ctx, AV1E_SET_MV_COST_UPD_FREQ, val);
        (void)ret;
    }

    // AV1E_SET_LOSSLESS: expects an int (0-1)
    if (size > i) {
        int val = data[i++] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_LOSSLESS, val);
        (void)ret;
    }

    // AV1E_SET_TIER_MASK: expects an int (0-1)
    if (size > i) {
        int val = data[i++] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_TIER_MASK, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_296(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(NULL, &cfg, 0);

    // Use a small width/height to avoid huge allocations
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}