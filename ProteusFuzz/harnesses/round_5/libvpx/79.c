// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/* 
 * Helper to initialize encoder context with fuzzed config
 * Returns 0 on success, -1 on failure
 */
static int init_encoder_context(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg) {
    const vpx_codec_iface_t *iface;
    vpx_codec_err_t err;

    /* Try VP9 first, fall back to VP8 if needed */
    iface = vpx_codec_vp9_cx();
    if (!iface) {
        iface = vpx_codec_vp8_cx();
        if (!iface) return -1;
    }

    err = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

/* 
 * Helper to clean up encoder context
 */
static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    /* Require minimum size to safely access data indices */
    if (Size < 17) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t custom_buf;
    unsigned int pad_before, pad_after;
    vpx_codec_err_t err;

    /* Initialize config with defaults */
    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
            return 0;
        }
    }

    /* Fuzz config parameters safely using available data */
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_threads = (Data[2] % 16) + 1;
    cfg.g_profile = (Data[3] % 4);
    cfg.g_bit_depth = (vpx_bit_depth_t)(Data[4] % 3); 
    if (cfg.g_bit_depth == 0) cfg.g_bit_depth = VPX_BITS_8;
    else if (cfg.g_bit_depth == 1) cfg.g_bit_depth = VPX_BITS_10;
    else cfg.g_bit_depth = VPX_BITS_12;
    
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[5] % 60) + 1;
    
    cfg.rc_end_usage = (enum vpx_rc_mode)(Data[6] % 4);
    cfg.rc_target_bitrate = (Data[7] % 100) + 10;
    cfg.rc_min_quantizer = (Data[8] % 64);
    cfg.rc_max_quantizer = (Data[9] % 63) + cfg.rc_min_quantizer + 1;
    
    cfg.kf_mode = (enum vpx_kf_mode)(Data[10] % 3);
    cfg.kf_min_dist = (Data[11] % 100) + 1;
    cfg.kf_max_dist = (Data[12] % 100) + cfg.kf_min_dist;

    /* Initialize encoder */
    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Fuzz vpx_codec_enc_config_set */
    /* Reconfigure with potentially different settings */
    vpx_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.g_threads = (Data[13] % 16) + 1;
    new_cfg.rc_target_bitrate = (Data[14] % 1000) + 10;
    err = vpx_codec_enc_config_set(&ctx, &new_cfg);
    /* Ignore error for fuzzing exploration, but check for crashes */

    /* Prepare custom buffer for vpx_codec_set_cx_data_buf */
    size_t buf_size = (Size > 100) ? Size - 100 : 1024;
    if (buf_size == 0) buf_size = 1024;
    
    custom_buf.buf = malloc(buf_size);
    if (!custom_buf.buf) {
        cleanup_encoder_context(&ctx);
        return 0;
    }
    custom_buf.sz = buf_size;
    
    /* Fuzz padding values */
    pad_before = (Data[15] % 100);
    pad_after = (Data[16] % 100);

    /* Call vpx_codec_set_cx_data_buf */
    err = vpx_codec_set_cx_data_buf(&ctx, &custom_buf, pad_before, pad_after);
    /* Check return value but continue to explore state */

    /* Test resetting buffer to NULL */
    err = vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);

    /* Cleanup */
    free(custom_buf.buf);
    cleanup_encoder_context(&ctx);

    return 0;
}