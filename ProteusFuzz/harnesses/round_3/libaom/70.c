// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/* 
 * Fuzz driver for libaom encoder APIs.
 * Focuses on:
 * - aom_codec_control_typechecked_AV1E_SET_REDUCED_REFERENCE_SET
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_1TO4_PARTITIONS
 * - aom_codec_control_typechecked_AOME_SET_ROI_MAP
 * - aom_codec_control_typechecked_AV1E_GET_TARGET_SEQ_LEVEL_IDX
 * - aom_codec_encode
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height, int bit_depth) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_bit_depth = bit_depth;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static aom_image_t *create_test_image(int width, int height, int bit_depth) {
    aom_image_t *img = malloc(sizeof(aom_image_t));
    if (!img) return NULL;

    int plane_size = width * height * ((bit_depth > 8) ? 2 : 1);
    int stride = width * ((bit_depth > 8) ? 2 : 1);

    memset(img, 0, sizeof(aom_image_t));
    img->fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
    img->w = width;
    img->h = height;
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->monochrome = 0;
    img->range = AOM_CR_STUDIO_RANGE;

    // Allocate planes
    img->planes[0] = malloc(plane_size);
    img->planes[1] = malloc(plane_size / 4);
    img->planes[2] = malloc(plane_size / 4);
    img->stride[0] = stride;
    img->stride[1] = stride / 2;
    img->stride[2] = stride / 2;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Fill with dummy data
    for (int i = 0; i < plane_size; i++) img->planes[0][i] = i % 256;
    for (int i = 0; i < plane_size / 4; i++) img->planes[1][i] = 128;
    for (int i = 0; i < plane_size / 4; i++) img->planes[2][i] = 128;

    return img;
}

static void free_test_image(aom_image_t *img) {
    if (!img) return;
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
    free(img);
}

static void fuzz_roi_map(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 10) return;

    aom_roi_map_t roi_map;
    memset(&roi_map, 0, sizeof(roi_map));

    // Parse some data to configure ROI map
    roi_map.enabled = data[0] % 2;
    if (!roi_map.enabled) return;

    unsigned int rows = (data[1] % 10) + 1;
    unsigned int cols = (data[2] % 10) + 1;
    
    // Limit to reasonable sizes to avoid OOM
    if (rows > 100 || cols > 100) return;

    unsigned char *map_data = malloc(rows * cols);
    if (!map_data) return;

    for (unsigned int i = 0; i < rows * cols; i++) {
        // Ensure index is within bounds
        size_t idx = 3 + (i % (size - 3));
        if (idx >= size) idx = size - 1;
        map_data[i] = data[idx] % 8;
    }

    roi_map.roi_map = map_data;
    roi_map.rows = rows;
    roi_map.cols = cols;
    
    // Safe indexing for ROI map parameters
    size_t idx_q = 3 + (size % (size - 3));
    if (idx_q >= size) idx_q = size - 1;
    roi_map.delta_q[0] = (int)(data[idx_q]) - 128;

    size_t idx_lf = 4 + (size % (size - 3));
    if (idx_lf >= size) idx_lf = size - 1;
    roi_map.delta_lf[0] = (int)(data[idx_lf]) - 128;

    size_t idx_skip = 5 + (size % (size - 3));
    if (idx_skip >= size) idx_skip = size - 1;
    roi_map.skip[0] = data[idx_skip] % 2;

    size_t idx_ref = 6 + (size % (size - 3));
    if (idx_ref >= size) idx_ref = size - 1;
    roi_map.ref_frame[0] = data[idx_ref] % 8;

    size_t idx_dqp = 7 + (size % (size - 3));
    if (idx_dqp >= size) idx_dqp = size - 1;
    roi_map.delta_qp_enabled = data[idx_dqp] % 2;

    size_t idx_ref_en = 8 + (size % (size - 3));
    if (idx_ref_en >= size) idx_ref_en = size - 1;
    roi_map.reference_enabled = data[idx_ref_en] % 2;

    size_t idx_rdmult = 9 + (size % (size - 3));
    if (idx_rdmult >= size) idx_rdmult = size - 1;
    roi_map.rdmult_delta_qp = (int)data[idx_rdmult];

    aom_codec_control(ctx, AOME_SET_ROI_MAP, &roi_map);

    free(map_data);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Extract parameters from input
    int width = (Data[0] % 64) + 16;
    int height = (Data[1] % 64) + 16;
    int bit_depth = (Data[2] % 4) + 8; // 8, 10, 12, or 16

    if (!init_encoder(&ctx, &cfg, width, height, bit_depth)) {
        return 0;
    }

    // Test control functions
    // 1. AV1E_SET_REDUCED_REFERENCE_SET
    int reduced_ref = Data[3] % 2;
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, &reduced_ref);

    // 2. AV1E_SET_ENABLE_CFL_INTRA
    int cfl_intra = (Data[3] >> 1) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, &cfl_intra);

    // 3. AV1E_SET_ENABLE_1TO4_PARTITIONS
    int enable_1to4 = (Data[3] >> 2) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, &enable_1to4);

    // 4. AOME_SET_ROI_MAP
    fuzz_roi_map(&ctx, Data, Size);

    // 5. AV1E_GET_TARGET_SEQ_LEVEL_IDX
    int level_idx;
    aom_codec_control(&ctx, AV1E_GET_TARGET_SEQ_LEVEL_IDX, &level_idx);

    // Create test image and encode
    aom_image_t *img = create_test_image(width, height, bit_depth);
    if (!img) {
        cleanup_encoder(&ctx);
        return 0;
    }

    aom_codec_pts_t pts = 0;
    // Ensure we have at least 5 bytes to access Data[4]
    unsigned long duration = (Data[4] % 100) + 1;
    aom_enc_frame_flags_t flags = 0;

    // Encode a few frames
    for (int i = 0; i < 3; i++) {
        aom_codec_err_t res = aom_codec_encode(&ctx, img, pts, duration, flags);
        if (res != AOM_CODEC_OK) {
            // Continue even on error to explore more states
        }

        // Collect output
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet if needed
            // aom_codec_pkt_free is not a standard API; packets are managed internally
            // or should be handled via aom_codec_get_frame for decoders.
            // For encoders, we just iterate to clear the buffer.
        }

        pts += duration;
    }

    // Flush encoder
    while (1) {
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;

        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Consume packet
        }
        
        // Check if encoder is done
        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) break;
    }

    free_test_image(img);
    cleanup_encoder(&ctx);

    return 0;
}