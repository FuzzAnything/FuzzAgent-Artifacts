// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int initialize_codec(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                            const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (size >= 2) {
        cfg->g_w = (data[0] % 128) + 16;
        cfg->g_h = (data[1] % 128) + 16;
    } else {
        cfg->g_w = 128;
        cfg->g_h = 128;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (initialize_codec(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    int ret = 0;
    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            {
                int val = (Size > 1) ? (Data[1] % 2) : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
            }
            break;
        case 1:
            {
                int val = (Size > 1) ? (Data[1] % 2) : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
            }
            break;
        case 2:
            {
                int val = (Size > 1) ? (Data[1] % 2) : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val);
            }
            break;
        case 3:
            {
                int val = (Size > 1) ? (Data[1] % 2) : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
            }
            break;
        case 4:
            {
                aom_roi_map_t roi_map;
                memset(&roi_map, 0, sizeof(roi_map));
                roi_map.enabled = (Size > 1) ? (Data[1] % 2) : 0;
                roi_map.rows = (Size > 2) ? (Data[2] % 64) + 1 : 1;
                roi_map.cols = (Size > 3) ? (Data[3] % 64) + 1 : 1;
                roi_map.delta_qp_enabled = 0;
                roi_map.reference_enabled = 0;
                roi_map.rdmult_delta_qp = 0;

                size_t map_size = roi_map.rows * roi_map.cols;
                if (map_size > 0) {
                    roi_map.roi_map = (unsigned char *)malloc(map_size);
                    if (roi_map.roi_map) {
                        if (Size > 4) {
                            size_t copy_len = (Size - 4) < map_size ? (Size - 4) : map_size;
                            memcpy(roi_map.roi_map, Data + 4, copy_len);
                            if (copy_len < map_size) {
                                memset(roi_map.roi_map + copy_len, 0, map_size - copy_len);
                            }
                        } else {
                            memset(roi_map.roi_map, 0, map_size);
                        }
                    }
                }

                ret = aom_codec_control(&ctx, AOME_SET_ROI_MAP, &roi_map);

                if (roi_map.roi_map) {
                    free(roi_map.roi_map);
                }
            }
            break;
        case 5:
            {
                int val = (Size > 1) ? (Data[1] % 2) : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
            }
            break;
        default:
            break;
    }

    aom_codec_destroy(&ctx);

    return 0;
}