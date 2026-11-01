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

static int read_int(const uint8_t *d, size_t s) {
    if (s < 4) return 0;
    int v = (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
    return v;
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    int ret = 0;
    int val;

    // AV1E_SET_SUPERBLOCK_SIZE
    val = read_int(data, size);
    ret = aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
    (void)ret;

    // AV1E_SET_ENABLE_QM
    val = read_int(data + 4, size > 4 ? size - 4 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
    (void)ret;

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    val = read_int(data + 8, size > 8 ? size - 8 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
    (void)ret;

    // AV1E_SET_QM_U
    val = read_int(data + 12, size > 12 ? size - 12 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_QM_U, val);
    (void)ret;

    // AV1E_SET_ENABLE_KEYFRAME_FILTERING
    val = read_int(data + 16, size > 16 ? size - 16 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
    (void)ret;

    // AV1E_SET_CHROMA_SUBSAMPLING_Y
    val = read_int(data + 20, size > 20 ? size - 20 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzzing logic
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}