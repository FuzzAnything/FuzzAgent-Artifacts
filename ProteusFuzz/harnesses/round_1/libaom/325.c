// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

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

int LLVMFuzzerTestOneInput_325(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the first byte to decide between encoder and decoder paths
    int use_encoder = Data[0] & 1;

    if (use_encoder) {
        // Encoder path
        aom_codec_ctx_t ctx;
        aom_codec_enc_cfg_t cfg;
        memset(&ctx, 0, sizeof(ctx));
        memset(&cfg, 0, sizeof(cfg));

        const aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz some configuration parameters with bounds checking
        if (Size > 1) cfg.g_w = (Data[1] % 128) + 16;
        if (Size > 2) cfg.g_h = (Data[2] % 128) + 16;
        if (Size > 3) cfg.g_timebase.num = (Data[3] % 100) + 1;
        if (Size > 4) cfg.g_timebase.den = (Data[4] % 100) + 1;
        if (Size > 5) cfg.rc_target_bitrate = (Data[5] % 1000) + 100;
        if (Size > 6) cfg.g_threads = (Data[6] % 8) + 1;
        if (Size > 7) cfg.g_profile = Data[7] % 4;

        if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Create a dummy image frame
        aom_image_t img;
        if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
            cleanup_encoder(&ctx);
            return 0;
        }
        
        // Fill image data with fuzz input
        size_t img_size = (size_t)img.stride[0] * (size_t)img.h;
        size_t copy_size = (Size > img_size) ? img_size : Size;
        if (copy_size > 0 && Size > 1) {
            memcpy(img.planes[0], Data + 1, copy_size);
        }

        // Try to set superblock size if supported
        if (Size > 8) {
            aom_superblock_size_t sb_size = (aom_superblock_size_t)(Data[8] % 3);
            aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
        }

        // Attempt to encode
        aom_codec_encode(&ctx, &img, 0, 1, 0);

        // Try to get quantizer info (simulating target API calls)
        int quantizer;
        aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer);

        int64_t quantizer64;
        aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);

        int cdef_strength;
        aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength);

        // Cleanup
        aom_img_free(&img);
        cleanup_encoder(&ctx);
    } else {
        // Decoder path
        aom_codec_ctx_t ctx;
        aom_codec_dec_cfg_t cfg;
        memset(&ctx, 0, sizeof(ctx));
        memset(&cfg, 0, sizeof(cfg));

        const aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Try to get decoder-specific info
        int quantizer;
        aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);

        int sb_size;
        aom_codec_control(&ctx, AOMD_GET_SB_SIZE, &sb_size);

        int base_q_idx;
        aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);

        // Cleanup
        cleanup_decoder(&ctx);
    }

    return 0;
}