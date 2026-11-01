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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_297(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    if (Size < 1) {
        return 0;
    }

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_QM
    int enable_qm = (Data[0] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);
    (void)ret;

    // Fuzz AV1E_SET_QM_V
    if (Size > 1) {
        int qm_v = (int)Data[1];
        ret = aom_codec_control(&ctx, AV1E_SET_QM_V, qm_v);
        (void)ret;
    }

    // Fuzz AV1E_SET_SUPERBLOCK_SIZE
    if (Size > 2) {
        int sb_size = (int)Data[2];
        ret = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
        (void)ret;
    }

    // Fuzz AV1E_SET_LOSSLESS
    if (Size > 3) {
        int lossless = (Data[3] % 2);
        ret = aom_codec_control(&ctx, AV1E_SET_LOSSLESS, lossless);
        (void)ret;
    }

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_Y
    if (Size > 4) {
        int subsampling_y = (int)Data[4];
        ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, subsampling_y);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_CDEF
    if (Size > 5) {
        int enable_cdef = (Data[5] % 2);
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}