// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_image_t img;
    aom_fixed_buf_t *global_headers = NULL;

    if (size < 10) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz configuration parameters
    cfg.g_w = (data[0] % 64) + 16;
    cfg.g_h = (data[1] % 64) + 16;
    cfg.g_timebase.num = (data[2] % 100) + 1;
    cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.g_threads = (data[4] % 4) + 1;
    cfg.rc_target_bitrate = (data[5] % 10000) + 100;
    cfg.g_lag_in_frames = (data[6] % 20);
    cfg.g_error_resilient = (data[7] % 3);

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize image
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    memset(img.planes[0], data[8], cfg.g_w * cfg.g_h);
    memset(img.planes[1], data[9], (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], data[10] % 256, (cfg.g_w * cfg.g_h) / 4);

    // Fuzz control commands
    if (size > 12) {
        int partition_info_path[4] = {
            data[12] % 2,
            data[13] % 2,
            data[14] % 2,
            data[15] % 2
        };
        aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, partition_info_path);
    }

    if (size > 16) {
        int error_resilient_mode = data[16] % 3;
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    }

    if (size > 17) {
        int gf_min_pyramid_height = data[17] % 4;
        aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyramid_height);
    }

    if (size > 18) {
        int max_partition_size = (data[18] % 8) + 1;
        aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, max_partition_size);
    }

    // Encode a frame
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Retrieve global headers
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            // Use the buffer to ensure it's valid
            volatile char *p = global_headers->buf;
            (void)p;
        }
        free(global_headers->buf);
        free(global_headers);
    }

    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    fuzz_encoder(Data, Size);
    return 0;
}