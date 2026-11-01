// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int create_image_from_fuzz_data(const uint8_t *data, size_t size, aom_image_t *img, int width, int height) {
    if (width <= 0 || height <= 0) return 0;
    if (size < (size_t)width * height) return 0;

    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    int bit_depth = 8;
    
    aom_img_alloc(img, fmt, width, height, 16);
    if (!img->planes[0]) return 0;

    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    
    size_t total_needed = y_size + 2 * uv_size;
    if (size < total_needed) {
        memset(img->planes[0], 0, y_size);
        memset(img->planes[1], 0, uv_size);
        memset(img->planes[2], 0, uv_size);
    } else {
        memcpy(img->planes[0], data, y_size);
        memcpy(img->planes[1], data + y_size, uv_size);
        memcpy(img->planes[2], data + y_size + uv_size, uv_size);
    }

    img->bit_depth = bit_depth;
    return 1;
}

int LLVMFuzzerTestOneInput_327(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    uint32_t w = Data[0] | (Data[1] << 8);
    uint32_t h = Data[2] | (Data[3] << 8);
    
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    int frame_count = 0;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (Size > 4) {
        flags = (aom_enc_frame_flags_t)(Data[4] & 0xFF);
        if (Size > 5) duration = (unsigned long)(Data[5] & 0xFF);
        if (duration == 0) duration = 1;
    }

    int max_frames = 10;
    
    while (frame_count < max_frames) {
        if (Size > 4 && create_image_from_fuzz_data(Data, Size, &img, w, h)) {
            aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, flags);
            if (res != AOM_CODEC_OK) {
                // Error is expected for malformed data, continue to flush
            }
            pts += duration;
            frame_count++;
        } else {
            break; 
        }
    }

    // Flush the encoder
    while (1) {
        aom_codec_cx_pkt_t *pkt;
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;

        pkt = aom_codec_get_cx_data(&ctx, NULL);
        if (!pkt) break;
        
        // Check if the packet contains frame data
        // pkt->data is a union, we check the type to access the correct member
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Accessing the frame pointer safely
            if (pkt->data.frame.buf) {
                // Frame data exists, logic handled by library cleanup
            }
        }
        
        // Move to next packet or break if done
        if (!aom_codec_get_cx_data(&ctx, pkt)) break;
    }

    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}