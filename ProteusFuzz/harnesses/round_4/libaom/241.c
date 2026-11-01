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

static int get_int_from_data(const uint8_t *d, size_t s, int default_val) {
    if (s < 4) return default_val;
    int val = ((int)d[0] << 24) | ((int)d[1] << 16) | ((int)d[2] << 8) | (int)d[3];
    return val;
}

int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure width and height safely
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 1. aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY
    int intra_dct_only = get_int_from_data(Data, Size, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF
    int enable_cdef = get_int_from_data(Data + 4, Size > 4 ? Size - 4 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING
    int enable_keyframe_filtering = get_int_from_data(Data + 8, Size > 8 ? Size - 8 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, enable_keyframe_filtering);

    // 4. aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL
    int loopfilter_level = get_int_from_data(Data + 12, Size > 12 ? Size - 12 : 0, 0) % 64;
    aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_level);

    // 5. aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING
    int skip_postproc = get_int_from_data(Data + 16, Size > 16 ? Size - 16 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);

    // 6. aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_Y
    int chroma_subsampling = get_int_from_data(Data + 20, Size > 20 ? Size - 20 : 0, 0) % 4;
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, chroma_subsampling);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}