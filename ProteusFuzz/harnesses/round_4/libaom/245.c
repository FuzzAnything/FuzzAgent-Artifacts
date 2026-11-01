// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx || !arg) return 0;
    return aom_codec_control(ctx, control_id, arg);
}

int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Fuzz configuration parameters
    cfg.g_w = Data[0] | (Data[1] << 8);
    cfg.g_h = Data[2] | (Data[3] << 8);
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_w = (cfg.g_w / 16) * 16; // Align to 16
    cfg.g_h = (cfg.g_h / 16) * 16;

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = Data[4] + 1;

    cfg.rc_target_bitrate = Data[5] * 1000;
    cfg.g_bit_depth = (Data[6] % 5) + 8; // 8, 9, 10, 11, 12
    if (cfg.g_bit_depth > 12) cfg.g_bit_depth = 8;

    cfg.g_profile = Data[7] % 4;
    cfg.g_pass = 0;

    // Initialize flags based on bit depth to prevent initialization failure
    unsigned int init_flags = 0;
    if (cfg.g_bit_depth > 8) {
        init_flags = AOM_CODEC_USE_HIGHBITDEPTH;
    }

    err = aom_codec_enc_init(&ctx, iface, &cfg, init_flags);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control parameters
    int8_t noise_level = (int8_t)Data[8];
    int8_t block_size = (int8_t)Data[9];
    int8_t sharpness = (int8_t)Data[10];
    int8_t enable_restoration = (int8_t)Data[11];
    int8_t noise_sensitivity = (int8_t)Data[12];

    fuzz_control(&ctx, AOME_SET_SHARPNESS, &sharpness);
    fuzz_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, &noise_level);
    fuzz_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, &block_size);
    fuzz_control(&ctx, AV1E_SET_ENABLE_RESTORATION, &enable_restoration);
    fuzz_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, &noise_sensitivity);

    // Prepare image
    aom_image_t img;
    int bit_depth = cfg.g_bit_depth;
    int width = cfg.g_w;
    int height = cfg.g_h;
    int plane_size = width * height * (bit_depth / 8);
    if (bit_depth > 8) plane_size = width * height; // Simplified for 10/12 bit

    unsigned char *img_data = malloc(plane_size * 3);
    if (!img_data) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    memset(&img, 0, sizeof(img));
    img.fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) img.fmt = AOM_IMG_FMT_I42016;
    
    img.w = width;
    img.h = height;
    img.bit_depth = bit_depth;
    img.d_w = width;
    img.d_h = height;
    img.r_w = width;
    img.r_h = height;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.planes[0] = img_data;
    img.planes[1] = img_data + plane_size;
    img.planes[2] = img_data + plane_size * 2;
    img.stride[0] = width;
    img.stride[1] = width / 2;
    img.stride[2] = width / 2;
    img.sz = plane_size * 3;

    // Fill image data
    for (int i = 0; i < plane_size * 3 && i < Size - 13; i++) {
        img.planes[0][i % plane_size] = Data[13 + i];
    }
    // Fill chroma planes if needed
    for (int i = 0; i < plane_size && i < Size - 13; i++) {
        img.planes[1][i % plane_size] = Data[13 + i];
        img.planes[2][i % plane_size] = Data[13 + i];
    }

    aom_codec_pts_t pts = 0;
    unsigned long duration = Data[14] + 1;
    aom_enc_frame_flags_t flags = Data[15];

    // Encode frame
    err = aom_codec_encode(&ctx, &img, pts, duration, flags);
    
    // Flush
    err = aom_codec_encode(&ctx, NULL, pts, duration, flags);

    // Cleanup
    free(img_data);
    aom_codec_destroy(&ctx);

    return 0;
}