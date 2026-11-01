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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = (size_t)(data[0] & 0x7F) + 1;
    cfg.g_h = (size_t)(data[1] & 0x7F) + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] & 0x7F) + 1;
    cfg.rc_target_bitrate = (data[3] & 0x7F) + 1;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int res;
    int val;

    if (Size < 4) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    val = Data[0] & 1;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    val = Data[1] & 1;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    (void)res;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    val = Data[2] & 1;
    res = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val);
    (void)res;

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    val = Data[3] & 0xFF;
    res = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    val = Data[0] & 1;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ
    val = Data[1] & 1;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    (void)res;

    cleanup_codec(&ctx);

    return 0;
}