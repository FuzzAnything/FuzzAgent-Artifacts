// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"

// Declare the interface getter functions that should be available in libvpx
extern vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_cx(void);

static vpx_codec_iface_t* get_decoder_iface(void) {
    // Try to get a valid decoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_dx();
    if (!iface) {
        iface = vpx_codec_vp9_dx();
    }
    return iface;
}

static vpx_codec_iface_t* get_encoder_iface(void) {
    // Try to get a valid encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        iface = vpx_codec_vp9_cx();
    }
    return iface;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_iface_t* dec_iface = get_decoder_iface();
    if (!dec_iface) return 0;

    vpx_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = (Data[0] % 8) + 1;  // 1-8 threads
    dec_cfg.w = 320;
    dec_cfg.h = 240;

    // Test vpx_codec_dec_init_ver
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 
                                                        0, VPX_DECODER_ABI_VERSION);
    if (init_result != VPX_CODEC_OK) {
        // Test vpx_codec_error_detail on failure
        const char* error_detail = vpx_codec_error_detail(&dec_ctx);
        (void)error_detail; // Use the result to avoid unused variable warning
        return 0;
    }

    // Test vpx_codec_decode with the fuzzer input
    size_t decode_size = Size > 1000 ? 1000 : Size;
    vpx_codec_decode(&dec_ctx, Data, decode_size, NULL, 0);

    // Test vpx_codec_control_ with various control IDs
    // Use different control IDs based on input data
    int ctrl_id = Data[0] % 256;
    vpx_codec_control_(&dec_ctx, ctrl_id, NULL);

    // Test vpx_codec_error_detail after operations
    const char* detail = vpx_codec_error_detail(&dec_ctx);
    (void)detail;

    // Initialize encoder context for testing encoder functions
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t* enc_iface = get_encoder_iface();
    if (enc_iface && Size > 10) {
        // Simple encoder initialization with default config
        vpx_codec_enc_cfg_t enc_cfg;
        // Use vpx_codec_enc_config_default if available
        // For now, we'll skip encoder initialization if we can't get config
        // and just test with a minimal setup
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        enc_cfg.rc_target_bitrate = 256;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_profile = 0;
        enc_cfg.g_bit_depth = VPX_BITS_8;
        enc_cfg.g_input_bit_depth = 8;
        enc_cfg.rc_min_quantizer = 2;
        enc_cfg.rc_max_quantizer = 56;
        enc_cfg.rc_end_usage = VPX_VBR;
        
        if (vpx_codec_enc_init(&enc_ctx, enc_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
            
            // Test vpx_codec_set_cx_data_buf
            vpx_fixed_buf_t buf;
            unsigned int buf_size = (Data[1] % 1024) + 64;
            buf.buf = malloc(buf_size);
            if (buf.buf) {
                buf.sz = buf_size;
                unsigned int pad_before = Data[2] % 16;
                unsigned int pad_after = Data[3] % 16;
                
                vpx_codec_set_cx_data_buf(&enc_ctx, &buf, pad_before, pad_after);
                
                // Test with NULL buffer to restore default
                vpx_codec_set_cx_data_buf(&enc_ctx, NULL, 0, 0);
                
                free(buf.buf);
            }
            
            // Test vpx_codec_get_cx_data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            int max_iterations = (Data[4] % 10) + 1;
            for (int i = 0; i < max_iterations; i++) {
                pkt = vpx_codec_get_cx_data(&enc_ctx, &iter);
                if (!pkt) break;
            }
            
            // Test vpx_codec_control_ on encoder
            int enc_ctrl_id = Data[5] % 256;
            vpx_codec_control_(&enc_ctx, enc_ctrl_id, NULL);
            
            vpx_codec_destroy(&enc_ctx);
        }
    }

    // Clean up decoder
    vpx_codec_destroy(&dec_ctx);

    return 0;
}