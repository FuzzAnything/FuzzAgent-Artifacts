// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"

static int create_encoder_context(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_328(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int flag;

    if (Size < 2) {
        return 0;
    }

    /* Initialize codec context */
    memset(&ctx, 0, sizeof(ctx));

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (!create_encoder_context(&ctx, iface)) {
        return 0;
    }

    /* Use input data to drive control parameters */
    int param_idx = Data[0] % 6;
    int value = (int)(Data[1] & 0xFF);

    /* Map input to control constants and values */
    switch (param_idx) {
        case 0:
            /* AV1E_SET_INTER_DCT_ONLY: 0 or 1 */
            flag = (value & 1) ? 1 : 0;
            ret = aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, flag);
            break;
        case 1:
            /* AV1E_SET_NOISE_SENSITIVITY: 0-100 typically */
            ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, value % 101);
            break;
        case 2:
            /* AV1E_SET_DENOISE_NOISE_LEVEL: 0-100 typically */
            ret = aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, value % 101);
            break;
        case 3:
            /* AV1E_SET_ENABLE_DNL_DENOISING: 0 or 1 */
            flag = (value & 1) ? 1 : 0;
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, flag);
            break;
        case 4:
            /* AV1E_SET_ENABLE_INTRABC: 0 or 1 */
            flag = (value & 1) ? 1 : 0;
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, flag);
            break;
        case 5:
            /* AV1E_SET_ENABLE_CHROMA_DELTAQ: 0 or 1 */
            flag = (value & 1) ? 1 : 0;
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, flag);
            break;
        default:
            ret = AOM_CODEC_OK;
            break;
    }

    /* Attempt to encode a dummy frame to exercise the state changes */
    if (ret == AOM_CODEC_OK) {
        aom_image_t img;
        const aom_codec_cx_pkt_t *pkt;

        /* Create a small dummy image */
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        
        /* Fill with some data from input if available */
        if (Size > 64 * 64 * 3 / 2) {
            memcpy(img.planes[0], Data + 2, 64 * 64);
        } else {
            memset(img.planes[0], 0, 64 * 64);
        }

        /* Encode: Fixed signature to 5 arguments */
        ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
        
        /* Drain the encoder */
        while (ret == AOM_CODEC_OK) {
            pkt = aom_codec_get_cx_data(&ctx, NULL);
            if (pkt) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    /* No explicit free needed for packet data in this context */
                }
            } else {
                break;
            }
        }
        
        aom_img_free(&img);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}