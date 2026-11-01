// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static int fuzz_encode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL) return 0;
    
    // Create a minimal image from fuzz data
    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, 32, 32, 1) != 0) return 0;
    
    // Fill image with fuzz data (safely)
    size_t plane_size = img.planes[0][0] * img.stride[0];
    size_t total_size = plane_size * 3; // Y, U, V
    size_t copy_size = (size < total_size) ? size : total_size;
    memcpy(img.planes[0], data, copy_size);
    
    // Encode the frame
    aom_codec_encode(ctx, &img, 0, 1, 0);
    
    aom_img_free(&img);
    return 1;
}

int LLVMFuzzerTestOneInput_292(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz some configuration parameters
    cfg.g_w = (Data[0] % 32) + 16; // Width between 16 and 47
    cfg.g_h = (Data[1] % 32) + 16; // Height between 16 and 47
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) + 100;
    cfg.g_threads = (Data[6] % 4) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_set_option
    char key[32], val[32];
    if (Size > 20) {
        size_t k_len = Data[10] % 16;
        size_t v_len = Data[11] % 16;
        if (k_len > 0 && v_len > 0 && (k_len + v_len) < 20) {
            memcpy(key, Data + 12, k_len);
            key[k_len] = '\0';
            memcpy(val, Data + 12 + k_len, v_len);
            val[v_len] = '\0';
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Fuzz aom_codec_set_cx_data_buf
    unsigned int pad_before = Data[12] % 100;
    unsigned int pad_after = Data[13] % 100;
    size_t buf_size = (Data[14] * 256 + Data[15]) % 4096 + 64;
    
    void *custom_buf = malloc(buf_size);
    if (custom_buf) {
        aom_fixed_buf_t buf;
        buf.buf = custom_buf;
        buf.sz = buf_size;
        aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
        
        // Encode a frame to potentially use the buffer
        fuzz_encode_frame(&ctx, Data, Size);
        
        // Iterate through packets
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Just consume the packet to avoid leaks
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Do nothing, just iterate
            }
        }
        
        free(custom_buf);
    }

    // Try with NULL buffer to restore default
    aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers != NULL) {
        if (headers->buf != NULL) {
            free(headers->buf);
        }
        free(headers);
    }

    // Flush the encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    // Drain remaining packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume packets
    }

    aom_codec_destroy(&ctx);
    return 0;
}