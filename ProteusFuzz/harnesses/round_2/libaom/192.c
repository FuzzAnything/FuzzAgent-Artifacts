// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (!ctx || !cfg) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure width/height if possible, ensuring valid ranges
    if (size >= 8) {
        uint32_t width = *(uint32_t *)data;
        uint32_t height = *(uint32_t *)(data + 4);
        if (width > 0 && width < 4096) cfg->g_w = width;
        if (height > 0 && height < 4096) cfg->g_h = height;
    }
    // Ensure minimum valid dimensions
    if (cfg->g_w == 0) cfg->g_w = 16;
    if (cfg->g_h == 0) cfg->g_h = 16;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int initialized = 0;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    initialized = init_encoder(&ctx, &cfg, Data, Size);
    if (!initialized) return 0;

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    // Expected arg: aom_chroma_sample_t (int)
    if (Size > 0) {
        int chroma_x = Data[0] % 3; // 0, 1, or 2
        aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, chroma_x);
    }

    // Fuzz AV1E_SET_ENABLE_DIST_8X8
    // Note: This constant might not exist in all versions. 
    // If it causes compile errors, it indicates the specific libaom version doesn't have it.
    // We will skip it if the constant is not defined to ensure compilation.
#ifdef AV1E_SET_ENABLE_DIST_8X8
    if (Size > 1) {
        int enable_dist = Data[1] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_8X8, enable_dist);
    }
#endif

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    // Expected arg: aom_color_primaries_t (int)
    if (Size > 2) {
        int prim = Data[2] % 20; // Limit to reasonable range
        aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, prim);
    }

    // Fuzz AV1E_SET_ENABLE_OBMC
    // Expected arg: int (0 or 1)
    if (Size > 3) {
        int enable_obmc = Data[3] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_OBMC, enable_obmc);
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    // Expected arg: int (frequency)
    if (Size > 4) {
        int freq = Data[4] % 100 + 1;
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq);
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    // Expected arg: int (0 or 1)
    if (Size > 5) {
        int enable_wedge = Data[5] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_wedge);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}