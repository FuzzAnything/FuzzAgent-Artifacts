// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// VP8D_SET_DECRYPTOR__value at vp8dx.h:198:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int parse_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val = 0;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

static int parse_uint(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned int)) return 0;
    unsigned int val = 0;
    memcpy(&val, *data, sizeof(unsigned int));
    *data += sizeof(unsigned int);
    *size -= sizeof(unsigned int);
    return val;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Randomize configuration based on input
    unsigned int width = parse_uint(&ptr, &remaining);
    unsigned int height = parse_uint(&ptr, &remaining);
    
    // Clamp dimensions to reasonable values to avoid OOM or invalid states
    if (width == 0 || width > 4096) width = 128;
    if (height == 0 || height > 4096) height = 128;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t err;

    // Try VP8
    iface = vpx_codec_vp8_cx();
    if (iface) {
        err = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
            cfg.g_w = width;
            cfg.g_h = height;
            cfg.g_threads = parse_uint(&ptr, &remaining) % 16;
            cfg.g_profile = parse_uint(&ptr, &remaining) % 4;
            cfg.g_timebase.num = parse_uint(&ptr, &remaining) % 100 + 1;
            cfg.g_timebase.den = parse_uint(&ptr, &remaining) % 100 + 1;
            cfg.rc_target_bitrate = parse_uint(&ptr, &remaining) % 10000 + 1;
            cfg.rc_min_quantizer = parse_uint(&ptr, &remaining) % 63;
            cfg.rc_max_quantizer = parse_uint(&ptr, &remaining) % 63;
            cfg.kf_mode = (enum vpx_kf_mode)(parse_uint(&ptr, &remaining) % 3);
            cfg.kf_min_dist = parse_uint(&ptr, &remaining) % 100;
            cfg.kf_max_dist = parse_uint(&ptr, &remaining) % 100;
            cfg.g_pass = (enum vpx_enc_pass)(parse_uint(&ptr, &remaining) % 3);

            err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Fuzz VP8E_SET_CPUUSED via vpx_codec_control
                int cpu_used = parse_uint(&ptr, &remaining) % 9; // 0-8
                vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);

                // Fuzz VP8E_SET_TUNING via vpx_codec_control
                int tuning = parse_uint(&ptr, &remaining) % 2; // 0 or 1
                vpx_codec_control(&ctx, VP8E_SET_TUNING, tuning);

                // Fuzz VP8D_SET_DECRYPTOR (if supported by interface, though unlikely on encoder)
                // We attempt it to test error handling paths
                vpx_decrypt_init dec_init = {0};
                dec_init.decrypt_cb = NULL;
                dec_init.decrypt_state = NULL;
                vpx_codec_control(&ctx, VP8D_SET_DECRYPTOR, &dec_init);

                vpx_codec_destroy(&ctx);
            }
        }
    }

    // Try VP9 (using vpx_codec_vp9_cx which is available in vpx_encoder.h)
    iface = vpx_codec_vp9_cx();
    if (iface) {
        err = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
            cfg.g_w = width;
            cfg.g_h = height;
            cfg.g_threads = parse_uint(&ptr, &remaining) % 16;
            cfg.g_profile = parse_uint(&ptr, &remaining) % 4;
            cfg.g_timebase.num = parse_uint(&ptr, &remaining) % 100 + 1;
            cfg.g_timebase.den = parse_uint(&ptr, &remaining) % 100 + 1;
            cfg.rc_target_bitrate = parse_uint(&ptr, &remaining) % 10000 + 1;
            cfg.rc_min_quantizer = parse_uint(&ptr, &remaining) % 63;
            cfg.rc_max_quantizer = parse_uint(&ptr, &remaining) % 63;
            cfg.kf_mode = (enum vpx_kf_mode)(parse_uint(&ptr, &remaining) % 3);
            cfg.kf_min_dist = parse_uint(&ptr, &remaining) % 100;
            cfg.kf_max_dist = parse_uint(&ptr, &remaining) % 100;
            cfg.g_pass = (enum vpx_enc_pass)(parse_uint(&ptr, &remaining) % 3);

            err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);

            if (err == VPX_CODEC_OK) {
                // Fuzz VP9E_SET_TUNE_CONTENT via vpx_codec_control
                // VP9E_CONTENT_DEFAULT=0, VP9E_CONTENT_SCREEN=1, VP9E_CONTENT_FILM=2
                int content_type = parse_uint(&ptr, &remaining) % 3; 
                vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, content_type);

                // Fuzz VP8E_SET_CPUUSED (often shared or similar logic in VP9)
                int cpu_used = parse_uint(&ptr, &remaining) % 9;
                vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);

                vpx_codec_destroy(&ctx);
            }
        }
    }

    // Test vpx_codec_control_ directly with invalid parameters to trigger error paths
    vpx_codec_ctx_t dummy_ctx;
    memset(&dummy_ctx, 0, sizeof(dummy_ctx));
    
    // Call with NULL context
    vpx_codec_control_(&dummy_ctx, 0x12345678, 0);
    
    // Call with valid context but invalid control ID
    if (iface) {
        err = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
            cfg.g_w = 64;
            cfg.g_h = 64;
            vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
            if (err == VPX_CODEC_OK) {
                // Send random control IDs
                int random_ctrl = parse_uint(&ptr, &remaining);
                vpx_codec_control_(&ctx, random_ctrl, 0);
                vpx_codec_destroy(&ctx);
            }
        }
    }

    return 0;
}