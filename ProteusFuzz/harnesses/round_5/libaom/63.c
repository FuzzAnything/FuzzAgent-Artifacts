// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    int32_t val = (int32_t)value;
    return aom_codec_control(ctx, control_id, val);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(uint32_t));
    int width = (seed & 0xFFFF) % 2048 + 16;
    int height = ((seed >> 16) & 0xFFFF) % 2048 + 16;
    int bit_depth = (seed % 2 == 0) ? 8 : 10;
    int color_space = (seed % 3); 

    if (color_space == 0) {
        width = (width + 1) & ~1;
        height = (height + 1) & ~1;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0; 
    cfg.g_usage = AOM_USAGE_REALTIME;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    
    if (Size > 4) {
        size_t img_size = (size_t)img.planes[0] * (size_t)img.stride[0] + 
                          (size_t)img.planes[1] * (size_t)img.stride[1] + 
                          (size_t)img.planes[2] * (size_t)img.stride[2];
        size_t copy_size = (Size - 4) < img_size ? (Size - 4) : img_size;
        memcpy(img.planes[0], Data + 4, copy_size);
    }

    aom_enc_frame_flags_t flags = 0;
    if (seed & 1) flags |= AOM_EFLAG_FORCE_KF;

    aom_codec_encode(&ctx, &img, 0, 1, flags);
    aom_img_free(&img);

    int controls_to_test[6];
    int values[6];
    int count = 0;

    if (seed & 0x10) {
        controls_to_test[count] = AV1E_SET_ENABLE_RESTORATION;
        values[count] = (seed >> 8) & 0xFF;
        count++;
    }
    if (seed & 0x20) {
        controls_to_test[count] = AV1E_SET_ENABLE_TX_SIZE_SEARCH;
        values[count] = (seed >> 16) & 0xFF;
        count++;
    }
    if (seed & 0x40) {
        controls_to_test[count] = AV1E_SET_ENABLE_GLOBAL_MOTION;
        values[count] = (seed >> 24) & 0xFF;
        count++;
    }
    if (seed & 0x80) {
        controls_to_test[count] = AV1E_SET_AQ_MODE;
        values[count] = seed % 4; 
        count++;
    }
    if (seed & 0x100) {
        controls_to_test[count] = AV1E_SET_MODE_COST_UPD_FREQ;
        values[count] = (seed >> 4) & 0xFF;
        count++;
    }
    if (seed & 0x200) {
        controls_to_test[count] = AV1E_SET_CDF_UPDATE_MODE;
        values[count] = seed % 3; 
        count++;
    }

    if (count == 0) {
        int all_controls[] = {
            AV1E_SET_ENABLE_RESTORATION,
            AV1E_SET_ENABLE_TX_SIZE_SEARCH,
            AV1E_SET_ENABLE_GLOBAL_MOTION,
            AV1E_SET_AQ_MODE,
            AV1E_SET_MODE_COST_UPD_FREQ,
            AV1E_SET_CDF_UPDATE_MODE
        };
        for (int i = 0; i < 6; i++) {
            controls_to_test[i] = all_controls[i];
            values[i] = (seed >> (i * 4)) & 0xFF;
        }
        count = 6;
    }

    for (int i = 0; i < count; i++) {
        fuzz_encoder_control(&ctx, controls_to_test[i], values[i]);
        fuzz_encoder_control(&ctx, controls_to_test[i], 0);
        fuzz_encoder_control(&ctx, controls_to_test[i], 1);
    }

    aom_codec_destroy(&ctx);

    return 0;
}