// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec && codec->priv) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings
    uint32_t width = (Data[0] % 32) + 16;
    uint32_t height = (Data[1] % 32) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Prepare a dummy image frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 0, img.stride[0] * height);
    memset(img.planes[1], 128, img.stride[1] * ((height + 1) / 2));
    memset(img.planes[2], 128, img.stride[2] * ((height + 1) / 2));

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    int64_t rtc_val = 0;
    if (Size > 4) {
        rtc_val = *(const int64_t *)(Data + 4);
    }
    aom_codec_control(&codec, AV1E_SET_RTC_EXTERNAL_RC, rtc_val);

    // Fuzz AV1E_SET_TIER_MASK
    int tier_mask = 0;
    if (Size > 12) {
        tier_mask = *(const int *)(Data + 12);
    }
    aom_codec_control(&codec, AV1E_SET_TIER_MASK, tier_mask);

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    int color_primaries = 0;
    if (Size > 16) {
        color_primaries = *(const int *)(Data + 16);
    }
    aom_codec_control(&codec, AV1E_SET_COLOR_PRIMARIES, color_primaries);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    int enable_ref_frame_mvs = 0;
    if (Size > 20) {
        enable_ref_frame_mvs = *(const int *)(Data + 20);
    }
    aom_codec_control(&codec, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs);

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    int chroma_subsampling_x = 0;
    if (Size > 24) {
        chroma_subsampling_x = *(const int *)(Data + 24);
    }
    aom_codec_control(&codec, AV1E_SET_CHROMA_SUBSAMPLING_X, chroma_subsampling_x);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    int enable_interintra_wedge = 0;
    if (Size > 28) {
        enable_interintra_wedge = *(const int *)(Data + 28);
    }
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge);

    // Encode a frame to exercise the codec
    aom_codec_frame_flags_t flags = 0;
    if (Size > 32) flags = Data[32];
    aom_codec_encode(&codec, &img, 0, 1, flags);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&codec);

    return 0;
}