// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface functions that are defined in the library
// but might not be declared in the headers we have
extern const vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern const vpx_codec_iface_t *vpx_codec_vp9_dx(void);

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Dummy callback function for put_frame registration
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get decoder interfaces
    vpx_codec_iface_t *iface_vp8 = (vpx_codec_iface_t *)vpx_codec_vp8_dx();
    vpx_codec_iface_t *iface_vp9 = (vpx_codec_iface_t *)vpx_codec_vp9_dx();
    
    // Choose codec interface based on first byte
    vpx_codec_iface_t *iface = NULL;
    if ((Data[0] & 1) == 0 && iface_vp8) {
        iface = iface_vp8;
    } else if (iface_vp9) {
        iface = iface_vp9;
    }
    
    if (!iface) return 0;
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Use fuzzed data for configuration parameters if enough data available
    if (Size >= 5) {
        cfg.threads = Data[1] % 8 + 1;  // 1-8 threads
        cfg.w = (Data[2] << 8) | Data[3];
        cfg.h = (Data[4] << 8) | Data[5];
        if (cfg.w > 4096) cfg.w = 4096;
        if (cfg.h > 4096) cfg.h = 4096;
    } else {
        cfg.threads = 1;
        cfg.w = 0;
        cfg.h = 0;
    }
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;  // Failed to initialize, skip this input
    }
    
    // Register put_frame callback with probability based on fuzzed data
    if (Size >= 6 && (Data[6] & 0x80)) {
        vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    }
    
    // Prepare data for decoding (skip first few bytes used for configuration)
    size_t data_offset = Size < 10 ? Size : 10;
    const uint8_t *decode_data = Data + data_offset;
    size_t decode_size = Size - data_offset;
    
    // Decode the data
    if (decode_size > 0) {
        vpx_codec_decode(&ctx, decode_data, (unsigned int)decode_size, NULL, 0);
        
        // Try to get stream info
        vpx_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&ctx, &si);
        
        // Try to get decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Access some image properties to explore more states
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
        
        // Try some control operations based on fuzzed data
        if (Size >= 7) {
            int ctrl_id = Data[7] % 256;  // Use modulo to limit to reasonable range
            
            // Map to actual control IDs (these are examples, actual IDs may vary)
            // We'll use some common control IDs from vpx_decoder.h
            switch (ctrl_id) {
                case 0:
                    // Try VP8D_GET_LAST_REF_UPDATES if defined
                    #ifdef VP8D_GET_LAST_REF_UPDATES
                    vpx_codec_control_(&ctx, VP8D_GET_LAST_REF_UPDATES, NULL);
                    #endif
                    break;
                case 1:
                    // Try VP8D_GET_FRAME_CORRUPTED if defined
                    #ifdef VP8D_GET_FRAME_CORRUPTED
                    vpx_codec_control_(&ctx, VP8D_GET_FRAME_CORRUPTED, NULL);
                    #endif
                    break;
                case 2: {
                    int value = Data[8];
                    // Try VP9_INVERT_TILE_DECODE_ORDER if defined
                    #ifdef VP9_INVERT_TILE_DECODE_ORDER
                    vpx_codec_control_(&ctx, VP9_INVERT_TILE_DECODE_ORDER, value);
                    #endif
                    break;
                }
                case 3: {
                    int value = Data[9] & 1;
                    // Try VP8D_SET_DECRYPTOR if defined
                    #ifdef VP8D_SET_DECRYPTOR
                    vpx_codec_control_(&ctx, VP8D_SET_DECRYPTOR, value);
                    #endif
                    break;
                }
                default:
                    // Try with the raw ctrl_id
                    vpx_codec_control_(&ctx, ctrl_id, NULL);
                    break;
            }
        }
    } else {
        // Try to flush decoder
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    }
    
    // Cleanup
    vpx_codec_destroy(&ctx);
    
    return 0;
}