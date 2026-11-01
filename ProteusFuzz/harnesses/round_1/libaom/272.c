// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    // Use data to determine which control to try and what value to pass
    uint8_t control_type = data[0] % 6;
    
    switch (control_type) {
        case 0: // AV1E_SET_INTER_DCT_ONLY
            if (size >= 2) {
                int val = (data[1] != 0);
                aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
            }
            break;
        case 1: // AV1E_SET_REDUCED_TX_TYPE_SET
            if (size >= 2) {
                int val = data[1];
                aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
            }
            break;
        case 2: // AV1E_SET_SVC_LAYER_ID
            if (size >= 5) {
                aom_svc_layer_id_t layer_id;
                layer_id.spatial_layer_id = data[1];
                layer_id.temporal_layer_id = data[2];
                // Ensure values are within reasonable bounds for fuzzing
                if (layer_id.spatial_layer_id > 3) layer_id.spatial_layer_id = 3;
                if (layer_id.temporal_layer_id > 3) layer_id.temporal_layer_id = 3;
                aom_codec_control(ctx, AV1E_SET_SVC_LAYER_ID, &layer_id);
            }
            break;
        case 3: // AV1E_SET_ENABLE_INTERINTER_WEDGE
            if (size >= 2) {
                int val = (data[1] != 0);
                aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
            }
            break;
        case 4: // AV1E_GET_SEQ_LEVEL_IDX
            {
                int *level_idx = malloc(sizeof(int));
                if (level_idx) {
                    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, level_idx);
                    free(level_idx);
                }
            }
            break;
        case 5: // AV1E_SET_RENDER_SIZE
            if (size >= 5) {
                aom_image_t render_size;
                // Initialize image struct
                memset(&render_size, 0, sizeof(render_size));
                render_size.w = (data[1] << 8) | data[2];
                render_size.h = (data[3] << 8) | data[4];
                // Avoid zero or excessively large sizes
                if (render_size.w == 0 || render_size.h == 0) {
                    render_size.w = 16;
                    render_size.h = 16;
                }
                if (render_size.w > 4096) render_size.w = 4096;
                if (render_size.h > 4096) render_size.h = 4096;
                aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, &render_size);
            }
            break;
        default:
            break;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Set some basic config from input data
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}