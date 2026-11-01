// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
    if (cfg) {
        free(cfg);
    }
}

static int fuzz_encoder_with_options(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t *cfg = malloc(sizeof(aom_codec_enc_cfg_t));
    if (!cfg) return 0;

    memset(&ctx, 0, sizeof(ctx));
    aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0);

    // Fuzz config parameters based on input
    if (Size > 0) cfg->g_w = Data[0] ? Data[0] : 128;
    if (Size > 1) cfg->g_h = Data[1] ? Data[1] : 128;
    if (Size > 2) cfg->g_timebase.num = Data[2] ? Data[2] : 1;
    if (Size > 3) cfg->g_timebase.den = Data[3] ? Data[3] : 1;
    // Removed g_fps_num and g_fps_den as they are not members of aom_codec_enc_cfg
    if (Size > 6) cfg->g_threads = Data[6] % 16;
    if (Size > 7) cfg->g_lag_in_frames = Data[7];
    if (Size > 8) cfg->rc_target_bitrate = (Data[8] * 1000) + 100;
    if (Size > 9) cfg->g_error_resilient = Data[9] % 3;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        cleanup_encoder(&ctx, cfg);
        return 0;
    }

    // Fuzz aom_codec_set_option
    if (Size > 10) {
        char key[64], val[64];
        int key_len = Data[10] % 32;
        int val_len = Data[11] % 32;
        if (key_len > 0 && val_len > 0 && (key_len + val_len + 12) <= Size) {
            memcpy(key, Data + 12, key_len);
            key[key_len] = '\0';
            memcpy(val, Data + 12 + key_len, val_len);
            val[val_len] = '\0';
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Prepare a dummy frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg->g_w, cfg->g_h, 1);
    memset(img.planes[0], 128, cfg->g_w * cfg->g_h);
    memset(img.planes[1], 128, (cfg->g_w * cfg->g_h) / 4);
    memset(img.planes[2], 128, (cfg->g_w * cfg->g_h) / 4);

    aom_enc_frame_flags_t flags = 0;
    if (Size > 12) flags = Data[12];

    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Get global headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Use the data to ensure it's not null
        if (headers->buf && headers->sz > 0) {
            // Fixed: headers->buf is void*, cast to uint8_t* to access bytes
            volatile uint8_t c = ((uint8_t*)headers->buf)[0];
            (void)c;
        }
        free(headers->buf);
        free(headers);
    }

    aom_img_free(&img);
    cleanup_encoder(&ctx, cfg);
    return 0;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz encoder path
    fuzz_encoder_with_options(Data, Size);

    return 0;
}