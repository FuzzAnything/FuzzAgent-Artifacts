// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzz data to set width/height safely
    unsigned int w = 16;
    unsigned int h = 16;
    if (size >= 8) {
        w = *(const uint32_t *)(data);
        h = *(const uint32_t *)(data + 4);
    }
    // Clamp to reasonable limits
    if (w < 16) w = 16;
    if (h < 16) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;
    
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    return 1;
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    struct aom_codec_dec_cfg cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed invalid members: allow_multiref, allow_screencontenttools
    // These are encoder-specific or handled differently in newer versions

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t enc_ctx;
    aom_codec_ctx_t dec_ctx;
    int enc_init = 0;
    int dec_init = 0;

    if (Size < 2) return 0;

    // Use first byte to decide encoder vs decoder
    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (mode == 0) {
        if (init_encoder(&enc_ctx, payload, payload_size)) {
            enc_init = 1;
        }
    } else {
        if (init_decoder(&dec_ctx, payload, payload_size)) {
            dec_init = 1;
        }
    }

    // Fuzz encoder controls if encoder is initialized
    if (enc_init) {
        int val;
        if (payload_size >= 4) {
            val = *(const int32_t *)payload;
        } else {
            val = 0;
        }

        // AV1E_SET_CHROMA_SAMPLE_POSITION
        aom_codec_control(&enc_ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
        
        // AV1E_SET_QM_U
        aom_codec_control(&enc_ctx, AV1E_SET_QM_U, val);
        
        // AV1E_SET_SKIP_POSTPROC_FILTERING
        aom_codec_control(&enc_ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
        
        // AV1E_SET_TILE_ROWS
        aom_codec_control(&enc_ctx, AV1E_SET_TILE_ROWS, val);
        
        // AV1E_SET_ROW_MT
        aom_codec_control(&enc_ctx, AV1E_SET_ROW_MT, val);

        aom_codec_destroy(&enc_ctx);
    }

    // Fuzz decoder controls if decoder is initialized
    if (dec_init) {
        int val;
        if (payload_size >= 4) {
            val = *(const int32_t *)payload;
        } else {
            val = 0;
        }

        // AV1D_SET_ROW_MT
        aom_codec_control(&dec_ctx, AV1D_SET_ROW_MT, val);

        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}