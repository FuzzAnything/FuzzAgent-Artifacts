// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
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

static int fuzz_encoder(const uint8_t *Data, size_t Size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t err;
    aom_codec_cx_pkt_t *pkt;
    aom_fixed_buf_t *global_headers = NULL;
    aom_fixed_buf_t cx_buf;
    uint8_t *cx_data = NULL;

    // Initialize context
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 0) cfg.g_w = Data[0] + 16;
    if (Size > 1) cfg.g_h = Data[1] + 16;
    if (Size > 2) cfg.g_threads = Data[2] % 8;
    if (Size > 3) cfg.g_profile = Data[3] % 4;
    if (Size > 4) cfg.g_bit_depth = (Data[4] % 3 == 0) ? AOM_BITS_8 : (Data[4] % 3 == 1 ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 5) cfg.g_input_bit_depth = cfg.g_bit_depth;
    if (Size > 6) cfg.rc_end_usage = Data[6] % 4;
    if (Size > 7) cfg.rc_target_bitrate = (Data[7] % 100) + 100;
    if (Size > 8) cfg.g_timebase.num = Data[8] + 1;
    if (Size > 9) cfg.g_timebase.den = (Data[9] % 100) + 1;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Test aom_codec_set_option
    if (Size > 10) {
        char key[32], val[32];
        size_t key_len = (Data[10] % 30) + 1;
        size_t val_len = (Data[11] % 30) + 1;
        if (key_len < sizeof(key)) {
            memcpy(key, Data + 12, key_len);
            key[key_len] = '\0';
        }
        if (val_len < sizeof(val)) {
            memcpy(val, Data + 12 + key_len, val_len);
            val[val_len] = '\0';
        }
        aom_codec_set_option(&ctx, key, val);
    }

    // Test aom_codec_enc_config_set
    if (Size > 12) {
        aom_codec_enc_cfg_t fuzz_cfg = cfg;
        fuzz_cfg.g_w = Data[12] + 16;
        fuzz_cfg.g_h = Data[13] + 16;
        aom_codec_enc_config_set(&ctx, &fuzz_cfg);
    }

    // Prepare image for encoding
    memset(&img, 0, sizeof(img));
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.bit_depth = cfg.g_input_bit_depth;
    img.fmt = AOM_IMG_FMT_I420;
    img.planes[0] = malloc(img.w * img.h);
    img.planes[1] = malloc((img.w / 2) * (img.h / 2));
    img.planes[2] = malloc((img.w / 2) * (img.h / 2));
    img.stride[0] = img.w;
    img.stride[1] = img.w / 2;
    img.stride[2] = img.w / 2;

    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        goto cleanup;
    }

    if (Size > 14) {
        size_t img_size = img.w * img.h * img.bit_depth / 8;
        size_t copy_size = (Size - 14) < img_size ? (Size - 14) : img_size;
        memcpy(img.planes[0], Data + 14, copy_size);
    }

    // Test aom_codec_set_cx_data_buf
    if (Size > 15) {
        size_t buf_size = (Data[15] % 1000) + 100;
        cx_data = malloc(buf_size);
        if (cx_data) {
            cx_buf.buf = cx_data;
            cx_buf.sz = buf_size;
            unsigned int pad_before = Data[16] % 10;
            unsigned int pad_after = Data[17] % 10;
            aom_codec_set_cx_data_buf(&ctx, &cx_buf, pad_before, pad_after);
        }
    }

    // Encode frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    int frame_count = 0;
    int max_frames = 5;

    while (frame_count < max_frames) {
        err = aom_codec_encode(&ctx, &img, pts, duration, 0);
        if (err != AOM_CODEC_OK) {
            break;
        }

        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process packet
            }
            // aom_codec_cx_pkt_destroy does not exist; packets are managed by the codec context
            // or returned as part of the iteration. We just continue.
        }
        pts += duration;
        frame_count++;
    }

    // Flush encoder
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet
        }
        // No explicit destroy needed for packets returned by get_cx_data
    }

    // Test aom_codec_get_global_headers
    global_headers = aom_codec_get_global_headers(&ctx);

    // Test aom_codec_error
    const char *error_msg = aom_codec_error(&ctx);
    (void)error_msg;

cleanup:
    if (global_headers) {
        if (global_headers->buf) free(global_headers->buf);
        free(global_headers);
    }
    if (cx_data) free(cx_data);
    if (img.planes[0]) free(img.planes[0]);
    if (img.planes[1]) free(img.planes[1]);
    if (img.planes[2]) free(img.planes[2]);
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 18) {
        return 0;
    }
    fuzz_encoder(Data, Size);
    return 0;
}