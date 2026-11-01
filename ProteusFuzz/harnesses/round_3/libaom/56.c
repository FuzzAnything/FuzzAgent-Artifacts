// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

// Helper to safely read a uint32_t from the fuzz buffer
static uint32_t read_u32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) > size) {
        *offset = size;
        return 0;
    }
    uint32_t val = 0;
    memcpy(&val, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return val;
}

// Helper to safely read a uint8_t from the fuzz buffer
static uint8_t read_u8(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        *offset = size;
        return 0;
    }
    uint8_t val = data[*offset];
    *offset += 1;
    return val;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    // Initialize configuration with defaults
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Parse some basic parameters from fuzz data to vary the config
    size_t offset = 0;
    unsigned int width = read_u32(Data, Size, &offset);
    unsigned int height = read_u32(Data, Size, &offset);
    unsigned int threads = read_u8(Data, Size, &offset);

    // Sanity checks to avoid invalid allocations
    if (width == 0) width = 64;
    if (height == 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (threads == 0) threads = 1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.g_limit = 1;
    cfg.g_profile = 0;
    cfg.g_usage = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control parameters from fuzz data
    offset = 0;
    uint32_t ctrl_val_1 = read_u32(Data, Size, &offset);
    uint32_t ctrl_val_2 = read_u32(Data, Size, &offset);
    uint32_t ctrl_val_3 = read_u32(Data, Size, &offset);
    uint32_t ctrl_val_4 = read_u32(Data, Size, &offset);
    uint32_t ctrl_val_5 = read_u32(Data, Size, &offset);
    uint32_t ctrl_val_6 = read_u32(Data, Size, &offset);

    // 1. aom_codec_control_typechecked_AV1E_SET_REDUCED_REFERENCE_SET
    // Expected: int value (0 or 1)
    int reduced_ref_set = (ctrl_val_1 % 2);
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref_set);

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA
    // Expected: int value (0 or 1)
    int enable_cfl = (ctrl_val_2 % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl);

    // 3. aom_codec_control_typechecked_AOME_SET_SCALEMODE
    // Expected: aom_scaling_mode_t struct
    aom_scaling_mode_t scale_mode;
    scale_mode.h_scaling_mode = (AOM_SCALING_MODE)(ctrl_val_3 % 9); // 0-8
    scale_mode.v_scaling_mode = (AOM_SCALING_MODE)(ctrl_val_4 % 9); // 0-8
    aom_codec_control(&ctx, AOME_SET_SCALEMODE, &scale_mode);

    // 4. aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE
    // Expected: int value (0 or 1 typically for limited/full)
    int color_range = (ctrl_val_5 % 2);
    aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, color_range);

    // 5. aom_codec_enc_config_set
    // Reconfigure with current cfg (which we modified earlier)
    aom_codec_enc_config_set(&ctx, &cfg);

    // 6. aom_codec_control_typechecked_AV1E_SET_TILE_ROWS
    // Expected: int value (number of rows)
    int tile_rows = ctrl_val_6 % 8; // Limit to reasonable value
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}