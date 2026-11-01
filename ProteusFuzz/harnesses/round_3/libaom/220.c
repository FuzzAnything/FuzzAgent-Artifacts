// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    if (!encoder) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 100;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (aom_codec_enc_init(&ctx, encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Define control IDs based on libaom AV1 encoder controls
    int controls[] = {
        AV1E_ENABLE_EXT_TILE_DEBUG,
        AV1E_SET_ENABLE_RECT_PARTITIONS,
        AV1E_SET_ENABLE_PALETTE,
        AV1_SET_TILE_MODE,
        AV1E_SET_SINGLE_TILE_DECODING,
        AV1E_SET_TILE_COLUMNS
    };
    int num_controls = sizeof(controls) / sizeof(controls[0]);

    for (int i = 0; i < num_controls; i++) {
        if (Size > (size_t)(i + 2)) {
            int value = Data[i + 2] % 2; // Toggle between 0 and 1 for boolean-like controls
            fuzz_codec_control(&ctx, controls[i], value);
        }
    }

    // Attempt to encode a dummy frame
    aom_image_t frame;
    memset(&frame, 0, sizeof(frame));
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    aom_codec_frame_flags_t flags = 0;
    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));

    if (aom_codec_encode(&ctx, &frame, 0, 1, flags) != AOM_CODEC_OK) {
        aom_img_free(&frame);
        aom_codec_destroy(&ctx);
        return 0;
    }

    aom_img_free(&frame);
    aom_codec_destroy(&ctx);

    return 0;
}