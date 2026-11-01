// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
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

int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Use first byte to decide encoder or decoder path
    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    aom_codec_ctx_t ctx = {0};
    aom_codec_err_t res;
    int quantizer_val = 0;
    int64_t quantizer_val_64 = 0;
    int luma_cdef = 0;
    int base_q_idx = 0;
    int last_quantizer = 0;

    if (mode == 0) {
        // Encoder path
        const aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz some config values
        cfg.g_w = (payload[0] % 1024) + 16;
        cfg.g_h = (payload[1] % 1024) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_error_resilient = (payload[2] % 3);

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz AOME_GET_LAST_QUANTIZER
        if (aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val) != AOM_CODEC_OK) {
            // Expected to fail if no frame encoded yet, but safe to ignore
        }

        // Fuzz AOME_GET_LAST_QUANTIZER_64
        if (aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64) != AOM_CODEC_OK) {
            // Expected to fail if no frame encoded yet
        }

        // Fuzz AV1E_SET_QUANTIZER_ONE_PASS
        int fuzz_quant = (payload[3] % 64);
        if (aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, fuzz_quant) != AOM_CODEC_OK) {
            // May fail if not in one-pass mode
        }

        // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
        if (aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &luma_cdef) != AOM_CODEC_OK) {
            // May fail depending on state
        }

        // Encode a dummy frame to trigger state changes
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        
        // Fixed: use 'stride' instead of 'd_stride'
        memset(img.planes[0], payload[4] % 256, img.stride[0] * img.h);
        memset(img.planes[1], payload[5] % 256, img.stride[1] * img.h);
        memset(img.planes[2], payload[6] % 256, img.stride[2] * img.h);

        aom_codec_encode(&ctx, &img, 0, 1, 0);

        // Retry controls after encoding
        if (aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val) != AOM_CODEC_OK) {}
        if (aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64) != AOM_CODEC_OK) {}
        if (aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &luma_cdef) != AOM_CODEC_OK) {}

        aom_img_free(&img);
        cleanup_encoder(&ctx);

    } else {
        // Decoder path
        const aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (payload[0] % 8) + 1;
        cfg.allow_lowbitdepth = (payload[1] % 2);

        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz AOMD_GET_BASE_Q_IDX
        if (aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx) != AOM_CODEC_OK) {
            // May fail if no frame decoded
        }

        // Fuzz AOMD_GET_LAST_QUANTIZER
        if (aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer) != AOM_CODEC_OK) {
            // May fail if no frame decoded
        }

        // Attempt to decode some dummy data directly using aom_codec_decode
        // Signature: aom_codec_decode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t data_sz, void *user_priv)
        if (payload_size > 0) {
            aom_codec_decode(&ctx, payload, payload_size, NULL);
        }

        // Retry controls after decode attempt
        if (aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx) != AOM_CODEC_OK) {}
        if (aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer) != AOM_CODEC_OK) {}

        cleanup_decoder(&ctx);
    }

    return 0;
}