// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *data) {
    if (!ctx) return 0;
    
    // Attempt to invoke the control function
    // Since the specific function signatures are unknown/empty in the prompt,
    // we rely on the generic aom_codec_control mechanism which dispatches
    // based on the control ID.
    // We cast data to void* as expected by aom_codec_control.
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, data);
    
    // Handle potential errors if needed, but for fuzzing we mostly want to avoid crashes.
    // We return 1 to indicate success in processing this input path.
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from the input buffer
    uint32_t width = Data[0] | (Data[1] << 8);
    uint32_t height = Data[2] | (Data[3] << 8);
    
    // Sanity check dimensions to avoid allocation failures or excessive memory
    if (width == 0 || height == 0 || width > 8192 || height > 8192) {
        width = 64;
        height = 64;
    }

    // Initialize Encoder Context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare data for AV1E_SET_ENABLE_PALETTE
    int enable_palette = (Size > 4) ? Data[4] : 0;
    
    // Prepare data for AV1E_SET_ENABLE_CFL_INTRA
    int enable_cfl_intra = (Size > 5) ? Data[5] : 0;
    
    // Prepare data for AV1E_SET_INTRA_DEFAULT_TX_ONLY
    int intra_default_tx_only = (Size > 6) ? Data[6] : 0;
    
    // Prepare data for AV1E_SET_ENABLE_GLOBAL_MOTION
    int enable_global_motion = (Size > 7) ? Data[7] : 0;

    // Prepare data for AOME_SET_ACTIVEMAP and AV1E_GET_ACTIVEMAP
    aom_active_map_t active_map;
    active_map.rows = (Size > 8) ? Data[8] : 1;
    active_map.cols = (Size > 9) ? Data[9] : 1;
    
    // Limit active map size to prevent OOM
    if (active_map.rows > 1024 || active_map.cols > 1024) {
        active_map.rows = 1;
        active_map.cols = 1;
    }
    
    active_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (!active_map.active_map) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Fill active map with fuzz data if available
    size_t map_size = active_map.rows * active_map.cols;
    size_t data_offset = 10;
    if (Size > data_offset) {
        size_t copy_len = (Size - data_offset) < map_size ? (Size - data_offset) : map_size;
        memcpy(active_map.active_map, Data + data_offset, copy_len);
        if (copy_len < map_size) {
            memset(active_map.active_map + copy_len, 0, map_size - copy_len);
        }
    } else {
        memset(active_map.active_map, 0, map_size);
    }

    // Invoke control functions
    // Note: Since the exact function signatures for the typechecked variants are not provided,
    // we use the standard aom_codec_control with the standard control IDs.
    // The typechecked variants are likely wrappers or internal implementations.
    
    // 1. AV1E_SET_ENABLE_PALETTE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, &enable_palette);
    
    // 2. AOME_SET_ACTIVEMAP
    aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map);
    
    // 3. AV1E_GET_ACTIVEMAP
    aom_active_map_t get_map;
    get_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (get_map.active_map) {
        get_map.rows = active_map.rows;
        get_map.cols = active_map.cols;
        aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &get_map);
        free(get_map.active_map);
    }
    
    // 4. AV1E_SET_ENABLE_CFL_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, &enable_cfl_intra);
    
    // 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &intra_default_tx_only);
    
    // 6. AV1E_SET_ENABLE_GLOBAL_MOTION
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, &enable_global_motion);

    // Cleanup
    free(active_map.active_map);
    aom_codec_destroy(&ctx);

    return 0;
}