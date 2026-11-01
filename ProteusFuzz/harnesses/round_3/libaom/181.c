// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure basic settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control commands based on available data
    if (Size > 2) {
        int cmd_val = (int)(Data[2] % 100);
        if (aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, cmd_val) != AOM_CODEC_OK) {
            // Ignore error
        }

        if (Size > 3) {
            int qm_val = (int)(Data[3] % 256);
            if (aom_codec_control(&ctx, AV1E_SET_QM_V, qm_val) != AOM_CODEC_OK) {
                // Ignore error
            }
        }

        if (Size > 4) {
            int cs_x = (int)(Data[4] % 2);
            if (aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, cs_x) != AOM_CODEC_OK) {
                // Ignore error
            }
        }

        if (Size > 5) {
            int tx_set = (int)(Data[5] % 2);
            if (aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, tx_set) != AOM_CODEC_OK) {
                // Ignore error
            }
        }

        if (Size > 6) {
            int dist_8x8 = (int)(Data[6] % 2);
            // AV1E_SET_ENABLE_DIST_8X8 is not defined in the current headers.
            // Using the suggested alternative AV1E_SET_ENABLE_RECT_TX to ensure compilation
            // and test similar functionality related to transform types.
            if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, dist_8x8) != AOM_CODEC_OK) {
                // Ignore error
            }
        }

        if (Size > 7) {
            int min_cr = (int)(Data[7] % 256);
            if (aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr) != AOM_CODEC_OK) {
                // Ignore error
            }
        }
    }

    cleanup_codec(&ctx);

    return 0;
}