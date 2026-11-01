// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height safely
    if (size >= 8) {
        uint32_t w = *(uint32_t *)data;
        uint32_t h = *(uint32_t *)(data + 4);
        if (w > 0 && w <= 8192 && h > 0 && h <= 8192) {
            cfg->g_w = w;
            cfg->g_h = h;
        }
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Initialize encoder context
    if (!init_encoder_context(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Extract control parameters from fuzz data safely
    int arg_val = 0;
    int arg_val2 = 0;

    // Ensure we have at least 2 bytes before accessing Data[1]
    arg_val = (int)(Data[1]);
    if (Size >= 3) {
        arg_val2 = (int)(Data[2]);
    }

    // Map to specific AV1 control IDs based on input byte
    int selected_ctrl_id = 0;
    int choice = Data[0] % 6;
    
    switch (choice) {
        case 0:
            selected_ctrl_id = AV1E_SET_MIN_PARTITION_SIZE;
            arg_val = (arg_val % 32); // Partition size usually small
            break;
        case 1:
            selected_ctrl_id = AV1E_SET_ENABLE_1TO4_PARTITIONS;
            arg_val = (arg_val % 2); // Boolean
            break;
        case 2:
            selected_ctrl_id = AV1E_SET_ENABLE_AB_PARTITIONS;
            arg_val = (arg_val % 2); // Boolean
            break;
        case 3:
            selected_ctrl_id = AV1E_SET_MAX_PARTITION_SIZE;
            arg_val = (arg_val % 32);
            break;
        case 4:
            selected_ctrl_id = AV1E_SET_ENABLE_RECT_PARTITIONS;
            arg_val = (arg_val % 2); // Boolean
            break;
        case 5:
            // Fallback to a generic valid control or skip
            selected_ctrl_id = AOME_SET_CPUUSED;
            arg_val = (arg_val % 8);
            break;
    }

    // Ensure we don't pass 0 as control ID
    if (selected_ctrl_id == 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Invoke aom_codec_control
    aom_codec_err_t result = aom_codec_control(&ctx, selected_ctrl_id, arg_val);

    // Try to invoke with a second argument if applicable (some controls take 2)
    // Only pass second arg if we have valid data for it
    if (Size >= 3) {
        aom_codec_control(&ctx, selected_ctrl_id, arg_val, arg_val2);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}