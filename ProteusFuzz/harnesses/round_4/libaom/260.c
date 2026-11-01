// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *init_encoder_context(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    ctx->iface = iface;
    ctx->config.enc = cfg;
    
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to diversify configuration
    if (Size > 0) cfg.g_w = Data[0] ? Data[0] : 16;
    if (Size > 1) cfg.g_h = Data[1] ? Data[1] : 16;
    if (Size > 2) cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] ? Data[3] : 1;
    if (Size > 4) cfg.rc_target_bitrate = Data[4] * 100;
    if (Size > 5) cfg.g_threads = Data[5] % 8 + 1;

    aom_codec_ctx_t *ctx = init_encoder_context(iface, &cfg);
    if (!ctx) return 0;

    // Prepare a dummy frame
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(frame.planes[0], 0, frame.stride[0] * cfg.g_h);
    memset(frame.planes[1], 0, frame.stride[1] * (cfg.g_h / 2));
    memset(frame.planes[2], 0, frame.stride[2] * (cfg.g_h / 2));
    frame.d_w = cfg.g_w;
    frame.d_h = cfg.g_h;

    // Fuzz various control functions with input data
    if (Size >= 1) {
        int denoise_block_size = Data[0] % 4 + 1;
        aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);
    }

    if (Size >= 2) {
        int enable_qm = (Data[1] % 2) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_QM, enable_qm);
    }

    if (Size >= 3) {
        int noise_sensitivity = Data[2] % 10;
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }

    if (Size >= 4) {
        int deltaq_mode = Data[3] % 4;
        aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);
    }

    if (Size >= 5) {
        int enable_auto_alt_ref = (Data[4] % 2) ? 1 : 0;
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    }

    if (Size >= 6) {
        int deltaq_strength = Data[5] % 10;
        aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);
    }

    // Encode a frame to exercise the codec (Fixed: removed extra argument)
    aom_codec_cx_pkt_t pkt;
    aom_codec_encode(ctx, &frame, 0, 1, 0);
    aom_codec_get_cx_data(ctx, &pkt);

    // Cleanup
    aom_img_free(&frame);
    cleanup_encoder_context(ctx);

    // Write dummy file if data is large enough
    if (Size > 10) {
        write_dummy_file(Data, Size);
    }

    return 0;
}