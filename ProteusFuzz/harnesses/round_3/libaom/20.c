// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

// Helper to safely initialize an encoder context
static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) return -1;

    return 0;
}

// Helper to safely initialize a decoder context
static int init_decoder(aom_codec_ctx_t *codec) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg;
    // Use aom_codec_dec_config_default if available, otherwise manually init
    #ifdef AOM_CODEC_DEC_CONFIG_DEFAULT
    if (aom_codec_dec_config_default(&cfg) != AOM_CODEC_OK) return -1;
    #else
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    #endif

    if (aom_codec_dec_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static void cleanup_decoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to decide which fuzzing path to take
    int mode = Data[0] % 10;

    if (mode < 5) {
        // Encoder Path
        aom_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));
        aom_codec_enc_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));

        if (init_encoder(&codec, &cfg) != 0) return 0;

        // Prepare a dummy frame to encode to establish state
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        memset(img.planes[0], 128, img.stride[0] * 64);
        memset(img.planes[1], 128, img.stride[1] * 32);
        memset(img.planes[2], 128, img.stride[2] * 32);

        // Fixed signature: aom_codec_encode(ctx, img, pts, duration, flags)
        aom_codec_encode(&codec, &img, 0, 1, 0);

        // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
        if (Size > 1) {
            int max_refs = Data[1] % 8; // Valid range 0-7
            aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, max_refs);
        }

        // Fuzz AV1E_SET_QM_MAX
        if (Size > 2) {
            int qm_max = Data[2]; // 0-255
            aom_codec_control(&codec, AV1E_SET_QM_MAX, qm_max);
        }

        aom_img_free(&img);
        cleanup_encoder(&codec);

    } else {
        // Decoder Path
        aom_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));

        if (init_decoder(&codec) != 0) return 0;

        // Create a dummy file for potential file-based operations if needed
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }

        // Attempt to decode (will likely fail with random data, but sets up context)
        // Fixed: Use AOME_SET_CPUUSED instead of AOMD_SET_CPUUSED
        aom_codec_control(&codec, AOME_SET_CPUUSED, 0);
        aom_codec_decode(&codec, Data, Size, NULL);

        // Fuzz AOMD_GET_LAST_REF_USED
        int last_ref_used[8];
        memset(last_ref_used, 0, sizeof(last_ref_used));
        aom_codec_control(&codec, AOMD_GET_LAST_REF_USED, last_ref_used);

        // Fuzz AV1_GET_REFERENCE
        av1_ref_frame_t ref_frame;
        memset(&ref_frame, 0, sizeof(ref_frame));
        if (Size > 3) {
            ref_frame.idx = Data[3] % 8;
        }
        ref_frame.use_external_ref = 0;
        // Initialize image for output
        aom_img_alloc(&ref_frame.img, AOM_IMG_FMT_I420, 64, 64, 1);
        aom_codec_control(&codec, AV1_GET_REFERENCE, &ref_frame);
        aom_img_free(&ref_frame.img);

        // Fuzz AV1_COPY_REFERENCE
        av1_ref_frame_t copy_frame;
        memset(&copy_frame, 0, sizeof(copy_frame));
        if (Size > 4) {
            copy_frame.idx = Data[4] % 8;
        }
        copy_frame.use_external_ref = 0;
        aom_img_alloc(&copy_frame.img, AOM_IMG_FMT_I420, 64, 64, 1);
        aom_codec_control(&codec, AV1_COPY_REFERENCE, &copy_frame);
        aom_img_free(&copy_frame.img);

        // Fuzz AV1_SET_REFERENCE
        av1_ref_frame_t set_frame;
        memset(&set_frame, 0, sizeof(set_frame));
        if (Size > 5) {
            set_frame.idx = Data[5] % 8;
        }
        set_frame.use_external_ref = 0;
        aom_img_alloc(&set_frame.img, AOM_IMG_FMT_I420, 64, 64, 1);
        // Fill with dummy data
        memset(set_frame.img.planes[0], 0, set_frame.img.stride[0] * 64);
        aom_codec_control(&codec, AV1_SET_REFERENCE, &set_frame);
        aom_img_free(&set_frame.img);

        cleanup_decoder(&codec);
    }

    return 0;
}