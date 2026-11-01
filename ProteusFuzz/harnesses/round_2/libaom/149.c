// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset, int min, int max) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

static int get_bool_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    return (val != 0) ? 1 : 0;
}

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    aom_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        memset(ctx, 0, sizeof(aom_codec_ctx_t));
    }
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_codec_err_t ret;

    init_encoder_context(&ctx, &cfg);
    if (ctx.err != AOM_CODEC_OK) return 0;

    size_t offset = 0;
    int ctrl_id = 0;

    if (offset + sizeof(int) <= Size) {
        ctrl_id = *(const int *)(Data + offset);
        offset += sizeof(int);
    }

    switch (ctrl_id) {
        case 1: // AV1E_SET_MAX_PARTITION_SIZE
            {
                int val = get_int_from_data(Data, Size, &offset, 4, 128);
                if (val > 0) {
                    ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val);
                    if (ret != AOM_CODEC_OK) {
                        // Ignore error, just continue
                    }
                }
            }
            break;
        case 2: // AV1E_SET_ENABLE_CDEF
            {
                int val = get_bool_from_data(Data, Size, &offset);
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore error
                }
            }
            break;
        case 3: // AV1E_SET_SUPERBLOCK_SIZE
            {
                int val = get_int_from_data(Data, Size, &offset, 0, 2);
                ret = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore error
                }
            }
            break;
        case 4: // AV1E_SET_ENABLE_KEYFRAME_FILTERING
            {
                int val = get_bool_from_data(Data, Size, &offset);
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore error
                }
            }
            break;
        case 5: // AV1E_SET_ENABLE_QM
            {
                int val = get_bool_from_data(Data, Size, &offset);
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore error
                }
            }
            break;
        case 6: // AV1E_SET_FORCE_VIDEO_MODE
            {
                int val = get_bool_from_data(Data, Size, &offset);
                ret = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore error
                }
            }
            break;
        default:
            // Randomly pick a control to test if no specific ID provided
            if (offset + sizeof(int) <= Size) {
                int random_ctrl = *(const int *)(Data + offset);
                offset += sizeof(int);
                int val = 1;
                if (offset + sizeof(int) <= Size) {
                    val = *(const int *)(Data + offset);
                    offset += sizeof(int);
                }
                // Try a few common controls safely
                if (random_ctrl == 0) {
                    ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val > 4 ? val : 4);
                } else if (random_ctrl == 1) {
                    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val % 2);
                } else if (random_ctrl == 2) {
                    ret = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, val % 3);
                } else if (random_ctrl == 3) {
                    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val % 2);
                } else if (random_ctrl == 4) {
                    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val % 2);
                } else if (random_ctrl == 5) {
                    ret = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val % 2);
                }
            }
            break;
    }

    aom_codec_destroy(&ctx);
    return 0;
}