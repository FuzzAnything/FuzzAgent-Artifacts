// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
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

int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int tier;
    int tuning;
    int max_drop_ms;
    int enable_intrabc;
    int external_rc;
    int enable_chroma_deltaq;

    // Initialize encoder configuration
    // aom_codec_enc_config_default requires iface, cfg, and usage
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize config with usage flag (0 for default)
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract parameters from fuzz data if available
    if (Size >= 1) tier = Data[0];
    if (Size >= 2) tuning = Data[1];
    if (Size >= 3) max_drop_ms = Data[2];
    if (Size >= 4) enable_intrabc = Data[3];
    if (Size >= 5) external_rc = Data[4];
    if (Size >= 6) enable_chroma_deltaq = Data[5];

    // Initialize codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_TIER_MASK
    ret = aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier);
    (void)ret;

    // Fuzz AOME_SET_TUNING
    ret = aom_codec_control(&ctx, AOME_SET_TUNING, tuning);
    (void)ret;

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, max_drop_ms);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    (void)ret;

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    ret = aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, external_rc);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}