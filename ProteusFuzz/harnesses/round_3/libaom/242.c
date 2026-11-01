// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int get_int_from_data(const uint8_t *d, size_t s) {
    if (s < 4) return 0;
    return (int)((d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3]);
}

static void fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    (void)ret;

    // AV1E_SET_TIER_MASK
    if (size >= 4) {
        int tier_mask = get_int_from_data(data, size);
        aom_codec_control(ctx, AV1E_SET_TIER_MASK, tier_mask);
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    if (size >= 8) {
        int x = get_int_from_data(data + 4, size - 4);
        aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, x);
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_Y
    if (size >= 12) {
        int y = get_int_from_data(data + 8, size - 8);
        aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, y);
    }

    // AV1E_SET_RTC_EXTERNAL_RC
    if (size >= 16) {
        int external_rc = get_int_from_data(data + 12, size - 12);
        aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, external_rc);
    }

    // AV1E_SET_SUPERBLOCK_SIZE
    if (size >= 20) {
        int sb_size = get_int_from_data(data + 16, size - 16);
        aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
    }

    // AV1E_SET_LOSSLESS
    if (size >= 24) {
        int lossless = get_int_from_data(data + 20, size - 20);
        aom_codec_control(ctx, AV1E_SET_LOSSLESS, lossless);
    }
}

int LLVMFuzzerTestOneInput_242(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set some config parameters
    cfg.g_w = (Data[0] % 4096) + 16;
    cfg.g_h = (Data[1] % 4096) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    cfg.g_threads = (Data[5] % 8) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}