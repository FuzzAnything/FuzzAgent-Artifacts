// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Parse basic parameters from input
    uint32_t width = Data[0] + 1;
    uint32_t height = Data[1] + 1;
    uint32_t bit_depth = (Data[2] % 5) + 8; // 8, 10, 12, 14, 16
    uint32_t seed = Data[3];

    // Limit dimensions to reasonable fuzzing bounds
    if (width > 1920 || height > 1080) {
        width = 32;
        height = 32;
    }

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Allocate image
    aom_image_t img;
    aom_img_fmt_t fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
    if (aom_img_alloc(&img, fmt, width, height, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = (size_t)img.stride[0] * height;
    if (Size > plane_size) Size = plane_size;
    memcpy(img.planes[0], Data, Size);
    // Fill other planes with simple patterns if needed
    if (img.planes[1]) {
        memset(img.planes[1], 0x80, plane_size / 2);
    }
    if (img.planes[2]) {
        memset(img.planes[2], 0x80, plane_size / 2);
    }

    // Set options based on fuzz data
    char key[32], value[32];
    snprintf(key, sizeof(key), "crf");
    snprintf(value, sizeof(value), "%d", Data[0] % 64);
    aom_codec_set_option(&ctx, key, value);

    snprintf(key, sizeof(key), "cpu-used");
    snprintf(value, sizeof(value), "%d", Data[1] % 8);
    aom_codec_set_option(&ctx, key, value);

    // Try to set VMAF model path if data allows
    if (Size > 10) {
        write_dummy_file(Data, Size);
        aom_codec_control(&ctx, AV1E_SET_VMAF_MODEL_PATH, "./dummy_file");
    }

    // Try to set denoise block size
    int denoise_size = (Data[2] % 4) + 1; // 1, 2, 3, 4
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_size);

    // Try to set enable intrabc
    int enable_intrabc = (Data[3] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    // Try to get target seq level idx
    int target_level;
    aom_codec_control(&ctx, AV1E_GET_TARGET_SEQ_LEVEL_IDX, &target_level);

    // Encode frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    int frame_count = 0;
    int max_frames = 5;

    while (frame_count < max_frames) {
        aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, 0);
        if (res != AOM_CODEC_OK) {
            // Might be expected if parameters are invalid
        }

        // Check for output
        aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // No free function needed for cx_pkt in standard libaom usage
            // The data is managed by the codec context until destroyed
            aom_codec_iter_t iter = NULL;
            // Just consume the packet to advance the iterator if needed
            // In this context, we just loop until NULL is returned
        }

        pts += duration;
        frame_count++;
    }

    // Flush encoder
    while (1) {
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;

        aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            has_data = 1;
            // No free function needed
        }
        if (!has_data) break;
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}