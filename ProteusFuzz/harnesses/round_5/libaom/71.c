// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom.h"
#include "aom_decoder.h"
#include "aom_encoder.h"
#include "aom_image.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Use first 4 bytes to determine test scenario
    uint32_t scenario = *(uint32_t *)Data;
    Data += 4;
    Size -= 4;

    aom_codec_ctx_t ctx = {0};
    aom_codec_err_t err;
    int is_encoder = (scenario & 1);
    int is_decoder = (scenario & 2);

    // Ensure at least one mode is selected
    if (!is_encoder && !is_decoder) {
        is_encoder = 1;
    }

    if (is_encoder) {
        const aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz some config parameters if data is available
        if (Size > 0) cfg.g_w = (Data[0] % 4096) + 16;
        if (Size > 1) cfg.g_h = (Data[1] % 4096) + 16;
        if (Size > 2) cfg.g_threads = (Data[2] % 16) + 1;
        if (Size > 3) cfg.g_timebase.num = (Data[3] % 255) + 1;
        if (Size > 4) cfg.g_timebase.den = (Data[4] % 255) + 1;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        // Prepare a dummy frame for encoding to ensure state is valid
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        memset(img.planes[0], 0, img.stride[0] * cfg.g_h);

        // Fuzz control commands for encoder
        // 1. AOME_SET_SPATIAL_LAYER_ID
        if (Size > 5) {
            int layer_id = Data[5] % 8;
            aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, layer_id);
        }

        // 2. AV1E_SET_SVC_FRAME_DROP_MODE
        if (Size > 6) {
            int drop_mode = Data[6] % 3; // 0, 1, or 2
            aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, drop_mode);
        }

        // 4. AV1E_SET_ENABLE_FILTER_INTRA
        if (Size > 7) {
            int enable = Data[7] % 2;
            aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable);
        }

        // 5. AOME_SET_NUMBER_SPATIAL_LAYERS
        if (Size > 8) {
            int num_layers = (Data[8] % 4) + 1;
            aom_codec_control(&ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, num_layers);
        }

        // 6. AV1E_SET_QM_MAX
        if (Size > 9) {
            int qm_max = Data[9] % 64;
            aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max);
        }

        // Attempt to encode a frame to exercise the state
        // Fixed: aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags
        aom_codec_encode(&ctx, &img, 0, 1, 0);

        aom_img_free(&img);
        cleanup_encoder(&ctx);
    }

    if (is_decoder) {
        const aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;

        err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz control commands for decoder
        // 3. AV1D_SET_OPERATING_POINT
        if (Size > 10) {
            int op = Data[10] % 64;
            aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, op);
        }

        // Decode dummy data if available
        if (Size > 11) {
            size_t decode_size = Size - 11;
            if (decode_size > 0) {
                aom_codec_decode(&ctx, Data + 11, decode_size, NULL);
            }
        }

        cleanup_decoder(&ctx);
    }

    return 0;
}