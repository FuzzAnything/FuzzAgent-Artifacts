// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/* 
 * Helper to get AV1 encoder interface.
 * aom_codec_av1_cx() is defined in aom/aom_encoder.h but requires aom_encoder.h inclusion.
 * If not found, we might need to check specific version headers, but standard is aom_codec_av1_cx().
 */

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Ensure we have the correct interface for AV1
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static int create_frame(aom_image_t *img, const uint8_t *data, size_t size) {
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, 64, 64, 1) != 0) return -1;
    
    if (size < 64 * 64 * 3 / 2) {
        memset(img->planes[0], 128, 64 * 64);
        memset(img->planes[1], 128, 32 * 32);
        memset(img->planes[2], 128, 32 * 32);
    } else {
        memcpy(img->planes[0], data, 64 * 64);
        memset(img->planes[1], 128, 32 * 32);
        memset(img->planes[2], 128, 32 * 32);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t ret;
    int val;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    if (initialize_encoder(&ctx, &cfg) != 0) return 0;

    if (create_frame(&img, Data, Size) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Feed the frame to the encoder. 
    // Signature: aom_codec_encode(ctx, img, pts, duration, flags)
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        aom_img_free(&img);
        return 0;
    }

    val = (int)(Data[0] % 256);

    // The target functions in the prompt are "typechecked" wrappers which are likely internal.
    // We attempt to call the standard aom_codec_control with the corresponding AV1E_* IDs.
    // These IDs are defined in aom/aom_encoder.h. If they are missing in the specific build,
    // the compiler will error. Assuming a standard libaom build where these exist.
    
    // 1. AV1E_SET_QM_U
    ret = aom_codec_control(&ctx, AV1E_SET_QM_U, val);
    (void)ret;

    // 2. AV1E_SET_QM_MIN
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);
    (void)ret;

    // 3. AV1E_SET_ENABLE_OVERLAY
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val);
    (void)ret;

    // 4. AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    // This might be an internal test flag. We try to call it if the macro exists.
    #ifdef AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    ret = aom_codec_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, val);
    (void)ret;
    #endif

    // 5. AV1E_SET_LOSSLESS
    ret = aom_codec_control(&ctx, AV1E_SET_LOSSLESS, val);
    (void)ret;

    // 6. AV1E_SET_MV_COST_UPD_FREQ
    ret = aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}