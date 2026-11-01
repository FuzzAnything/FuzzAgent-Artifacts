// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *enc_iface;
    aom_codec_iface_t *dec_iface;
    aom_codec_err_t res;
    int use_encoder = (Data[0] % 2) == 0;
    int use_decoder = !use_encoder;
    
    // Initialize configuration based on input data
    if (use_encoder) {
        enc_iface = aom_codec_av1_cx();
        if (!enc_iface) return 0;

        if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz configuration parameters
        cfg.g_w = (Data[1] % 512) + 16;
        cfg.g_h = (Data[2] % 512) + 16;
        cfg.g_timebase.num = (Data[3] % 100) + 1;
        cfg.g_timebase.den = (Data[4] % 100) + 1;
        cfg.g_threads = (Data[5] % 8) + 1;
        cfg.rc_target_bitrate = (Data[6] * 256 + Data[7]) + 100;

        res = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz aom_codec_set_option
        if (Size > 8) {
            char key[64], val[64];
            size_t key_len = (Data[8] % 32) + 1;
            size_t val_len = (Data[9] % 32) + 1;
            
            if (key_len < sizeof(key) && val_len < sizeof(val)) {
                memcpy(key, &Data[10], key_len);
                key[key_len] = '\0';
                memcpy(val, &Data[10 + key_len], val_len);
                val[val_len] = '\0';
                
                aom_codec_set_option(&enc_ctx, key, val);
            }
        }

        // Fuzz aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE
        if (Size > 10) {
            int val = Data[10];
            aom_codec_control(&enc_ctx, AV1E_SET_ERROR_RESILIENT_MODE, val);
        }

        // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
        if (Size > 11) {
            int val = Data[11];
            aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, val);
        }

        // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX
        if (Size > 12) {
            int val = Data[12];
            aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);
        }

        // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_SUPERRES
        if (Size > 13) {
            int val = Data[13];
            aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_SUPERRES, val);
        }

        // Fuzz aom_codec_get_global_headers
        aom_fixed_buf_t *hdrs = aom_codec_get_global_headers(&enc_ctx);
        if (hdrs) {
            free(hdrs->buf);
            free(hdrs);
        }

        cleanup_encoder(&enc_ctx);
    } else if (use_decoder) {
        dec_iface = aom_codec_av1_dx();
        if (!dec_iface) return 0;

        res = aom_codec_dec_init(&dec_ctx, dec_iface, NULL, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz aom_codec_set_option for decoder
        if (Size > 8) {
            char key[64], val[64];
            size_t key_len = (Data[8] % 32) + 1;
            size_t val_len = (Data[9] % 32) + 1;
            
            if (key_len < sizeof(key) && val_len < sizeof(val)) {
                memcpy(key, &Data[10], key_len);
                key[key_len] = '\0';
                memcpy(val, &Data[10 + key_len], val_len);
                val[val_len] = '\0';
                
                aom_codec_set_option(&dec_ctx, key, val);
            }
        }

        // Fuzz aom_codec_get_global_headers for decoder
        aom_fixed_buf_t *hdrs = aom_codec_get_global_headers(&dec_ctx);
        if (hdrs) {
            free(hdrs->buf);
            free(hdrs);
        }

        cleanup_decoder(&dec_ctx);
    }

    return 0;
}