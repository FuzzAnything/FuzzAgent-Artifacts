// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Set basic configuration compatible with current libaom API
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    // Use rc_end_usage instead of gc_crq_mode for newer versions, or keep defaults
    cfg->rc_end_usage = AOM_CQ;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    // Extract parameters from fuzz input
    int denoise_block_size = (int)(Data[0] % 32) + 1; // 1 to 32
    int denoise_noise_level = (int)(Data[1] % 100);   // 0 to 99
    int enable_cfl_intra = (int)(Data[2] % 2);        // 0 or 1
    int enable_intrabc = (int)(Data[3] % 2);          // 0 or 1
    int skip_postproc = (int)(Data[4] % 2);           // 0 or 1
    int enable_restoration = (int)(Data[5] % 2);      // 0 or 1

    // Call control functions
    // AV1E_SET_DENOISE_BLOCK_SIZE
    aom_codec_control(&codec, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);
    
    // AV1E_SET_DENOISE_NOISE_LEVEL
    aom_codec_control(&codec, AV1E_SET_DENOISE_NOISE_LEVEL, denoise_noise_level);
    
    // AV1E_SET_ENABLE_CFL_INTRA
    aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);
    
    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    
    // AV1E_SET_SKIP_POSTPROC_FILTERING
    aom_codec_control(&codec, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);
    
    // AV1E_SET_ENABLE_RESTORATION
    aom_codec_control(&codec, AV1E_SET_ENABLE_RESTORATION, enable_restoration);

    // Optional: Try to encode a dummy frame to ensure state is valid
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    
    // Fill with dummy data if available
    if (Size > 0) {
        for (int i = 0; i < 64 * 64 && i < Size; i++) {
            img.planes[0][i] = Data[i % Size];
        }
    }

    // Use correct control ID and argument count
    aom_codec_control(&codec, AOME_SET_CPUUSED, 0);
    aom_codec_control(&codec, AV1E_SET_ENABLE_QM, 0);

    // aom_codec_encode signature: (ctx, img, pts, duration, flags)
    aom_codec_encode(&codec, &img, 0, 1, 0);
    
    aom_img_free(&img);
    cleanup_encoder(&codec);

    return 0;
}