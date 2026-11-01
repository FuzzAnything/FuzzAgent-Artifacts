// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int enable_diff_wtd_comp = 1;

    if (size < 16) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
    int cdef_strengths[16];
    memset(cdef_strengths, 0, sizeof(cdef_strengths));
    res = aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, &enable_diff_wtd_comp);
    (void)res;

    // Prepare a dummy image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img.planes[0]) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Use img.sz to get the correct size for memset instead of calculating manually
    // This prevents buffer overflows if stride calculations are complex
    memset(img.planes[0], 0, img.sz);
    
    aom_codec_control(&ctx, AOM_CODEC_CX_FRAME_PKT, &img);

    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int sb_size = 0;
    int frame_flags = 0;
    int last_quantizer = 0;
    av1_ext_ref_frame_t ext_ref;
    aom_image_t dummy_img;

    if (size < 16) return 0;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Fuzz AOMD_GET_SB_SIZE
    res = aom_codec_control(&ctx, AOMD_GET_SB_SIZE, &sb_size);
    (void)res;

    // Fuzz AOMD_GET_FRAME_FLAGS
    res = aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)res;

    // Fuzz AOMD_GET_LAST_QUANTIZER
    res = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer);
    (void)res;

    // Fuzz AV1D_SET_EXT_REF_PTR
    aom_img_alloc(&dummy_img, AOM_IMG_FMT_I420, 128, 128, 1);
    if (!dummy_img.planes[0]) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Use img.sz for safe memset
    memset(dummy_img.planes[0], 0, dummy_img.sz);

    ext_ref.img = &dummy_img;
    ext_ref.num = 1;

    res = aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
    (void)res;

    aom_img_free(&dummy_img);
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Try encoder fuzzing
    fuzz_encoder(Data, Size);

    // Try decoder fuzzing
    fuzz_decoder(Data, Size);

    return 0;
}