// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static aom_codec_ctx_t *init_codec(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;

    memset(codec, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(codec);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    // Use fuzzing data to configure width/height within safe bounds
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)data;
        uint32_t h = *(const uint32_t *)(data + 4);
        // Ensure dimensions are valid and not too large
        if (w > 0 && w < 8192 && h > 0 && h < 8192) {
            cfg.g_w = w;
            cfg.g_h = h;
        } else {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *codec = init_codec(Data, Size);
    if (!codec) return 0;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    int intra_dct_only = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    int enable_tpl_model = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl_model);

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    int force_video_mode = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    // Valid range for color primaries is typically 0-23, clamp to safe range
    int color_primaries = Data[0] % 24; 
    aom_codec_control(codec, AV1E_SET_COLOR_PRIMARIES, color_primaries);

    // Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ
    int enable_chroma_deltaq = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    int enable_superres = (Data[0] % 2);
    aom_codec_control(codec, AV1E_SET_ENABLE_SUPERRES, enable_superres);

    // Perform a dummy encode to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, codec->config.enc->g_w, codec->config.enc->g_h, 1);
    
    // Fill image with dummy data using the correct size calculation
    // Size = stride * height for each plane
    memset(img.planes[0], 0x80, img.stride[0] * img.d_h);
    memset(img.planes[1], 0x80, img.stride[1] * img.d_h);
    memset(img.planes[2], 0x80, img.stride[2] * img.d_h);

    // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
    aom_codec_encode(codec, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_codec(codec);

    return 0;
}