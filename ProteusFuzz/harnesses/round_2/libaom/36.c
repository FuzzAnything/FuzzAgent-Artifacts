// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

// Helper to initialize a valid image for fuzzing
static int init_test_image(aom_image_t *img, unsigned int w, unsigned int h, unsigned int bit_depth) {
    if (!img || w == 0 || h == 0) return 0;
    
    img->fmt = AOM_IMG_FMT_I420;
    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;
    img->bit_depth = bit_depth;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_UNKNOWN;
    img->range = AOM_CR_STUDIO_RANGE;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->bps = 0;

    // Allocate planes
    size_t y_size = w * h;
    size_t uv_size = (w / 2) * (h / 2);
    
    img->planes[0] = (unsigned char *)malloc(y_size);
    img->planes[1] = (unsigned char *)malloc(uv_size);
    img->planes[2] = (unsigned char *)malloc(uv_size);
    img->stride[0] = w;
    img->stride[1] = w / 2;
    img->stride[2] = w / 2;
    img->sz = y_size + 2 * uv_size;
    img->img_data = NULL;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        return 0;
    }

    // Initialize with random-like data based on bit depth
    uint8_t max_val = (bit_depth > 8) ? 255 : (1 << bit_depth) - 1;
    memset(img->planes[0], max_val / 2, y_size);
    memset(img->planes[1], max_val / 2, uv_size);
    memset(img->planes[2], max_val / 2, uv_size);

    return 1;
}

static void cleanup_image(aom_image_t *img) {
    if (!img) return;
    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Minimum size required to safely access all indices used below (0 to 18)
    if (Size < 19) return 0;

    // Parse input for dimensions and bit depth
    unsigned int width = (Data[0] & 0xFF) | ((Data[1] & 0xFF) << 8);
    unsigned int height = (Data[2] & 0xFF) | ((Data[3] & 0xFF) << 8);
    unsigned int bit_depth = (Data[4] & 0xFF);
    
    // Clamp values to reasonable limits
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (bit_depth == 0) bit_depth = 8;
    if (bit_depth > 12) bit_depth = 12;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    if (!init_test_image(&img, width, height, bit_depth)) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz parameters from input
    uint64_t pts = (uint64_t)Data[5] | ((uint64_t)Data[6] << 8) | ((uint64_t)Data[7] << 16) | ((uint64_t)Data[8] << 24);
    unsigned long duration = (unsigned long)Data[9] | ((unsigned long)Data[10] << 8) | ((unsigned long)Data[11] << 16) | ((unsigned long)Data[12] << 24);
    if (duration == 0) duration = 1;
    
    // Ensure duration fits in uint32_t as per spec
    if (duration > UINT32_MAX) duration = UINT32_MAX;

    aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)(Data[13] | ((Data[14] & 0xFF) << 8));
    
    // Control commands to fuzz (using typechecked macros if available, or raw control)
    // We attempt to set various AV1 encoder options
    
    // 1. AV1E_SET_ENABLE_INTRABC
    int enable_intrabc = (Data[15] & 0xFF) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    // 2. AV1E_SET_REDUCED_REFERENCE_SET
    int reduced_ref = (Data[16] & 0xFF) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);

    // 3. AV1E_SET_CHROMA_SAMPLE_POSITION
    int csp = Data[17] % 3; // 0, 1, or 2
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, csp);

    // 4. AV1E_GET_TARGET_SEQ_LEVEL_IDX (Read-only, just call it)
    int level_idx;
    aom_codec_control(&ctx, AV1E_GET_TARGET_SEQ_LEVEL_IDX, &level_idx);

    // 5. AV1E_SET_ENABLE_CFL_INTRA
    int enable_cfl = (Data[18] & 0xFF) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl);

    // 6. aom_codec_encode
    // Encode one frame
    ret = aom_codec_encode(&ctx, &img, (aom_codec_pts_t)pts, duration, flags);
    
    // Check for errors but continue to flush
    if (ret != AOM_CODEC_OK) {
        // Might be invalid params, try flushing anyway
    }

    // Flush the encoder
    ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    // Drain output packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter))) {
        // Consume packet, do nothing
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Optionally free frame data if needed, but usually handled by codec
        }
    }

    // Cleanup
    cleanup_image(&img);
    aom_codec_destroy(&ctx);

    return 0;
}