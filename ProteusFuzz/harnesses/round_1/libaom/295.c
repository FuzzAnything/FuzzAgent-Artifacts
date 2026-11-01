// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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

// Helper to safely parse unsigned integers from fuzz data
static unsigned int parse_uint(const uint8_t *data, size_t *idx, size_t size, unsigned int max_val) {
    if (*idx + sizeof(unsigned int) > size) return 0;
    unsigned int val = 0;
    memcpy(&val, data + *idx, sizeof(unsigned int));
    *idx += sizeof(unsigned int);
    if (val > max_val) val = max_val;
    return val;
}

// Helper to safely parse signed integers from fuzz data
static int parse_int(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + sizeof(int) > size) return 0;
    int val = 0;
    memcpy(&val, data + *idx, sizeof(int));
    *idx += sizeof(int);
    return val;
}

// Helper to safely parse int64_t from fuzz data
static int64_t parse_int64(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + sizeof(int64_t) > size) return 0;
    int64_t val = 0;
    memcpy(&val, data + *idx, sizeof(int64_t));
    *idx += sizeof(int64_t);
    return val;
}

// Helper to safely parse unsigned long from fuzz data
static unsigned long parse_ulong(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + sizeof(unsigned long) > size) return 0;
    unsigned long val = 0;
    memcpy(&val, data + *idx, sizeof(unsigned long));
    *idx += sizeof(unsigned long);
    // Ensure duration fits within UINT32_MAX as per documentation
    if (val > UINT32_MAX) val = UINT32_MAX;
    return val;
}

int LLVMFuzzerTestOneInput_295(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Initialize configuration from fuzz data
    size_t idx = 0;
    
    // Parse width, height, bit depth
    cfg.g_w = parse_uint(Data, &idx, Size, 4096);
    cfg.g_h = parse_uint(Data, &idx, Size, 4096);
    cfg.g_bit_depth = parse_uint(Data, &idx, Size, 12);
    if (cfg.g_bit_depth < 8) cfg.g_bit_depth = 8;
    
    // Parse other config options
    cfg.g_timebase.num = parse_uint(Data, &idx, Size, 1000);
    cfg.g_timebase.den = parse_uint(Data, &idx, Size, 1000);
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
    
    cfg.rc_target_bitrate = parse_uint(Data, &idx, Size, 10000000);
    cfg.g_error_resilient = parse_uint(Data, &idx, Size, 2);
    cfg.g_lag_in_frames = parse_uint(Data, &idx, Size, 100);
    cfg.g_threads = parse_uint(Data, &idx, Size, 16);
    cfg.g_pass = 0; // Simplify to single pass for fuzzing

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (cfg.g_bit_depth > 8) fmt = AOM_IMG_FMT_I42016;
    
    // aom_img_alloc returns 0 on success, non-zero on error
    if (aom_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, 16) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data if enough data available
    if (idx < Size) {
        size_t plane_size = 0;
        for (int i = 0; i < 3; i++) {
            if (img.planes[i]) {
                plane_size += (size_t)img.stride[i] * img.h;
            }
        }
        size_t copy_size = (Size - idx) < plane_size ? (Size - idx) : plane_size;
        if (copy_size > 0) {
            // Simple scatter write to planes
            for (int i = 0; i < 3 && img.planes[i]; i++) {
                size_t plane_h = (i == 0) ? img.h : (img.h + img.y_chroma_shift) >> img.y_chroma_shift;
                size_t plane_w = (i == 0) ? img.w : (img.w + img.x_chroma_shift) >> img.x_chroma_shift;
                size_t plane_stride = img.stride[i];
                size_t plane_bytes = plane_stride * plane_h;
                
                size_t available = Size - idx;
                size_t to_copy = available < plane_bytes ? available : plane_bytes;
                if (to_copy > 0) {
                    memcpy(img.planes[i], Data + idx, to_copy);
                    idx += to_copy;
                }
            }
        }
    }

    // Fuzz aom_codec_encode
    int64_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    // Loop to encode frames and flush
    int frame_count = 0;
    int max_frames = 5; // Limit frames to avoid infinite loops in fuzzing
    
    while (frame_count < max_frames) {
        // Parse new parameters for this frame if data available
        if (idx < Size) {
            pts = parse_int64(Data, &idx, Size);
            duration = parse_ulong(Data, &idx, Size);
            flags = parse_int(Data, &idx, Size);
            if (duration == 0) duration = 1;
        }

        // Call aom_codec_encode
        aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        
        // Check for errors
        if (res != AOM_CODEC_OK) {
            // If error, try flushing or break
            if (res == AOM_CODEC_INVALID_PARAM || res == AOM_CODEC_INCAPABLE) {
                break;
            }
        }

        // Check for output packets
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet if needed, but for fuzzing we just consume
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Free the frame data if allocated
                if (pkt->data.frame.buf) {
                    free(pkt->data.frame.buf);
                }
            }
        }

        // If we successfully encoded, increment frame count
        if (res == AOM_CODEC_OK) {
            frame_count++;
            pts += duration;
        } else {
            break;
        }
    }

    // Flush the encoder
    while (1) {
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;
        
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            has_data = 1;
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                if (pkt->data.frame.buf) {
                    free(pkt->data.frame.buf);
                }
            }
        }
        if (!has_data) break;
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}