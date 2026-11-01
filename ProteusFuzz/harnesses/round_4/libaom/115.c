// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int32_t val1, val2, val3, val4, val5, val6;

    // Extract values from input data safely
    val1 = *(const int32_t *)(Data);
    val2 = *(const int32_t *)(Data + 4);
    val3 = *(const int32_t *)(Data + 8);
    val4 = *(const int32_t *)(Data + 12);
    val5 = *(const int32_t *)(Data + 16);
    val6 = *(const int32_t *)(Data + 20);

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val1);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val3);

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val4);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val5);

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}