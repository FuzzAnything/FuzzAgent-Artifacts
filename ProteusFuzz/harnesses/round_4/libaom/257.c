// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

// Helper to parse integers from fuzz data
static int32_t parse_int32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int32_t) > size) return 0;
    int32_t val;
    memcpy(&val, data + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return val;
}

static uint32_t parse_uint32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) > size) return 0;
    uint32_t val;
    memcpy(&val, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return val;
}

static int64_t parse_int64(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int64_t) > size) return 0;
    int64_t val;
    memcpy(&val, data + *offset, sizeof(int64_t));
    *offset += sizeof(int64_t);
    return val;
}

int LLVMFuzzerTestOneInput_257(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    size_t offset = 0;
    
    // Parse basic parameters
    int32_t width = parse_int32(Data, Size, &offset);
    int32_t height = parse_int32(Data, Size, &offset);
    int32_t bit_depth = parse_int32(Data, Size, &offset);
    int32_t fps_num = parse_int32(Data, Size, &offset);
    int32_t fps_den = parse_int32(Data, Size, &offset);
    
    // Clamp values to reasonable ranges to avoid allocation failures or undefined behavior
    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;
    if (bit_depth <= 0 || bit_depth > 12) bit_depth = 8;
    if (fps_num <= 0) fps_num = 30;
    if (fps_den <= 0) fps_den = 1;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000; // Low bitrate for speed
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_usage = AOM_CQ;
    cfg.rc_end_usage = AOM_CQ;
    cfg.g_pass = AOM_RC_ONE_PASS;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create an image buffer
    aom_image_t img;
    aom_img_fmt_t fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
    if (aom_img_alloc(&img, fmt, width, height, 1) != AOM_RC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data if enough data remains
    size_t img_size = img.sz;
    if (offset + img_size <= Size) {
        memcpy(img.planes[0], Data + offset, img_size);
        offset += img_size;
    } else if (offset < Size) {
        // Fill with remaining data pattern
        size_t remaining = Size - offset;
        for (size_t i = 0; i < img_size; i++) {
            img.planes[0][i] = Data[(offset + i) % remaining];
        }
        offset = Size;
    }

    // Fuzz aom_codec_encode
    // We try to encode a few frames or flush
    int64_t pts = 0;
    unsigned long duration = (fps_den > 0) ? (fps_den / fps_num) : 1;
    if (duration == 0) duration = 1;
    
    // Ensure duration fits in uint32_t as per spec
    if (duration > UINT32_MAX) duration = UINT32_MAX;

    aom_enc_frame_flags_t flags = 0;
    if (offset < Size) {
        flags = (aom_enc_frame_flags_t)parse_int32(Data, Size, &offset);
    }

    // Encode one frame
    aom_codec_encode(&ctx, &img, pts, duration, flags);

    // Flush the encoder
    aom_codec_encode(&ctx, NULL, 0, 0, 0);

    // Try to invoke control functions with fuzzed parameters
    // Note: The specific typechecked functions mentioned in the prompt have empty signatures
    // in the provided text, so we use the standard aom_codec_control with known constants
    // or attempt to pass raw data if the macro resolves to a standard control.
    
    // AV1E_SET_ENABLE_QM (0x00000001) - typically takes int32_t
    int32_t qm_val = (offset < Size) ? parse_int32(Data, Size, &offset) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, qm_val);

    // AV1E_SET_MV_COST_UPD_FREQ (0x00000002) - typically takes int32_t
    int32_t mv_cost_val = (offset < Size) ? parse_int32(Data, Size, &offset) : 0;
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, mv_cost_val);

    // AV1E_SET_DV_COST_UPD_FREQ (0x00000003) - typically takes int32_t
    int32_t dv_cost_val = (offset < Size) ? parse_int32(Data, Size, &offset) : 0;
    aom_codec_control(&ctx, AV1E_SET_DV_COST_UPD_FREQ, dv_cost_val);

    // AV1E_SET_RATE_DISTRIBUTION_INFO - complex struct, skip or pass dummy
    // If the typechecked wrapper exists, it might expect a specific struct.
    // Since signature is empty, we skip direct invocation to avoid compilation errors
    // unless the macro is defined in headers. We assume standard control for now.
    
    // AV1E_SET_TIER_MASK (0x00000004) - typically takes int32_t
    int32_t tier_val = (offset < Size) ? parse_int32(Data, Size, &offset) : 0;
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_val);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}