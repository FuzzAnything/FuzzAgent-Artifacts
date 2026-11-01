// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int quantizer_val;
    int64_t quantizer_val_64;
    aom_codec_err_t ret;

    if (!ctx) return 0;

    // Try AOME_GET_LAST_QUANTIZER
    ret = aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val);
    if (ret != AOM_CODEC_OK) {
        // Ignore error
    }

    // Try AOME_GET_LAST_QUANTIZER_64
    ret = aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64);
    if (ret != AOM_CODEC_OK) {
        // Ignore error
    }

    // Try AV1E_SET_QUANTIZER_ONE_PASS
    if (size >= 2) {
        int val = (data[0] << 8) | data[1];
        ret = aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, val);
        if (ret != AOM_CODEC_OK) {
            // Ignore
        }
    }

    return 0;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int base_q_idx;
    aom_codec_err_t ret;

    if (!ctx) return 0;

    // Try AOMD_GET_BASE_Q_IDX
    ret = aom_codec_control(ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    if (ret != AOM_CODEC_OK) {
        // Ignore error
    }

    return 0;
}

static int fuzz_error_string(const uint8_t *data, size_t size) {
    aom_codec_err_t err;
    const char *str;

    if (size >= 4) {
        err = *(const aom_codec_err_t *)(data);
    } else {
        // Use some common error codes if data is too short
        err = AOM_CODEC_MEM_ERROR;
    }

    str = aom_codec_err_to_string(err);
    if (str) {
        // Just ensure it doesn't crash
        (void)strlen(str);
    }

    return 0;
}

static int fuzz_global_headers(aom_codec_ctx_t *ctx) {
    aom_fixed_buf_t *headers;

    if (!ctx) return 0;

    headers = aom_codec_get_global_headers(ctx);
    if (headers) {
        // Check buffer validity
        if (headers->buf && headers->sz > 0) {
            // Do nothing, just ensure no crash
        }
        // Free the buffer as per documentation
        free(headers->buf);
        free(headers);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *enc_iface = NULL;
    aom_codec_iface_t *dec_iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;
    aom_image_t img;
    aom_codec_err_t err;
    int ret;

    if (Size < 4) {
        return 0;
    }

    // Initialize encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, enc_iface, &cfg, 0);
    if (ret == AOM_CODEC_OK) {
        // Create a dummy image for encoding
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
        memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
        memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

        // Encode one frame
        aom_codec_encode(&ctx, &img, 0, 1, 0);

        // Fuzz encoder controls
        fuzz_encoder_control(&ctx, Data, Size);

        // Fuzz global headers
        fuzz_global_headers(&ctx);

        aom_img_free(&img);
    }

    cleanup_codec(&ctx);

    // Initialize decoder
    dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;

    ret = aom_codec_dec_init(&ctx, dec_iface, &dec_cfg, 0);
    if (ret == AOM_CODEC_OK) {
        // Fuzz decoder controls
        fuzz_decoder_control(&ctx, Data, Size);
    }

    cleanup_codec(&ctx);

    // Fuzz error string conversion
    fuzz_error_string(Data, Size);

    return 0;
}