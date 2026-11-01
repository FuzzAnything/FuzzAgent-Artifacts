// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Select operation mode based on first byte
    int mode = Data[0] % 4;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    // Limit payload size to prevent allocation of huge buffers
    if (payload_size > 1024 * 1024) {
        payload_size = 1024 * 1024;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize configurations
    vpx_codec_enc_cfg_t cfg_enc;
    vpx_codec_dec_cfg_t cfg_dec;
    memset(&cfg_enc, 0, sizeof(cfg_enc));
    memset(&cfg_dec, 0, sizeof(cfg_dec));

    // Set some default values to avoid uninitialized memory usage
    cfg_enc.g_w = 128;
    cfg_enc.g_h = 128;
    cfg_enc.g_threads = 1;
    cfg_enc.g_profile = 0;
    cfg_enc.g_bit_depth = VPX_BITS_8;
    cfg_enc.g_input_bit_depth = 8;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    cfg_enc.g_pass = VPX_RC_ONE_PASS;
    cfg_enc.rc_end_usage = VPX_VBR;
    cfg_enc.rc_target_bitrate = 100;
    cfg_enc.kf_mode = VPX_KF_DISABLED;
    cfg_enc.kf_min_dist = 12;
    cfg_enc.kf_max_dist = 12;

    cfg_dec.threads = 1;
    cfg_dec.w = 128;
    cfg_dec.h = 128;

    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;

    switch (mode) {
        case 0: {
            // Test vpx_codec_enc_init_ver
            iface = vpx_codec_vp8_cx();
            if (!iface) break;
            
            res = vpx_codec_enc_init_ver(&ctx, iface, &cfg_enc, 0, VPX_ENCODER_ABI_VERSION);
            if (res == VPX_CODEC_OK) {
                // Try vpx_codec_control_ with various IDs
                // Use payload data to fuzz control parameters if needed
                // For simplicity, we try a few standard control IDs
                int ctrl_ids[] = {VP8E_SET_SCALEMODE, VP8E_SET_CPUUSED, VP8E_SET_TOKEN_PARTITIONS};
                for (int i = 0; i < 3; i++) {
                    int val = (payload_size > 0) ? payload[0] : 0;
                    vpx_codec_control_(&ctx, ctrl_ids[i], val);
                }
                
                // Cleanup
                vpx_codec_destroy(&ctx);
            } else {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail; // Suppress unused variable warning
            }
            break;
        }
        case 1: {
            // Test vpx_codec_dec_init_ver
            iface = vpx_codec_vp8_dx();
            if (!iface) break;

            res = vpx_codec_dec_init_ver(&ctx, iface, &cfg_dec, 0, VPX_DECODER_ABI_VERSION);
            if (res == VPX_CODEC_OK) {
                // Test vpx_codec_decode with fuzzed data
                res = vpx_codec_decode(&ctx, payload, payload_size, NULL, 0);
                if (res != VPX_CODEC_OK) {
                    const char *detail = vpx_codec_error_detail(&ctx);
                    (void)detail;
                }
                
                // Flush decoder
                vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
                
                vpx_codec_destroy(&ctx);
            } else {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail;
            }
            break;
        }
        case 2: {
            // Test vpx_codec_enc_init_ver with invalid ABI version
            iface = vpx_codec_vp8_cx();
            if (!iface) break;
            
            // Intentionally use wrong version to trigger ABI_MISMATCH
            res = vpx_codec_enc_init_ver(&ctx, iface, &cfg_enc, 0, 999);
            if (res != VPX_CODEC_OK) {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail;
            }
            // No destroy needed on failure per docs
            break;
        }
        case 3: {
            // Test vpx_codec_dec_init_ver with invalid ABI version
            iface = vpx_codec_vp8_dx();
            if (!iface) break;

            // Intentionally use wrong version
            res = vpx_codec_dec_init_ver(&ctx, iface, &cfg_dec, 0, 999);
            if (res != VPX_CODEC_OK) {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail;
            }
            // No destroy needed on failure per docs
            break;
        }
    }

    return 0;
}