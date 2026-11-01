// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

int LLVMFuzzerTestOneInput_250(const uint8_t *Data, size_t Size) {
    if (Size < 13) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to vary configuration slightly
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare arguments based on input data
    int val_int = 0;
    aom_svc_layer_id_t layer_id = {0, 0};
    
    // Ensure we have enough data before accessing indices
    val_int = Data[5] % 256;
    layer_id.spatial_layer_id = Data[6] % 4;
    layer_id.temporal_layer_id = Data[7] % 4;

    // Fuzz AV1E_SET_SVC_LAYER_ID
    aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &layer_id);

    // Fuzz AOME_SET_SPATIAL_LAYER_ID
    int spatial_id = Data[8] % 4;
    aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, &spatial_id);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    int freq = Data[9] % 100;
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, &freq);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    int enable = (Data[10] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, &enable);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    int min_size = (Data[11] % 4) + 1; // 1, 2, 3, or 4
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, &min_size);

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    int enable_filter = (Data[12] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, &enable_filter);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}