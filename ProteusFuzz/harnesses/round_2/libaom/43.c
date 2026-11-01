// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/*
 * Define control IDs if not present in the included headers.
 * These correspond to the AV1 encoder and decoder control commands.
 */
#ifndef AV1E_SET_ENABLE_FILTER_INTRA
#define AV1E_SET_ENABLE_FILTER_INTRA 2001
#endif
#ifndef AV1E_SET_ENABLE_RECT_TX
#define AV1E_SET_ENABLE_RECT_TX 2002
#endif
#ifndef AV1E_SET_ENABLE_DIFF_WTD_COMP
#define AV1E_SET_ENABLE_DIFF_WTD_COMP 2003
#endif
#ifndef AV1E_SET_ENABLE_AB_PARTITIONS
#define AV1E_SET_ENABLE_AB_PARTITIONS 2004
#endif
#ifndef AV1E_SET_ENABLE_DIST_WTD_COMP
#define AV1E_SET_ENABLE_DIST_WTD_COMP 2005
#endif
#ifndef AV1D_SET_EXT_REF_PTR
#define AV1D_SET_EXT_REF_PTR 1001
#endif

/* Forward declaration for the external reference frame structure if not defined in aom.h */
#ifndef AV1_EXT_REF_FRAME_T_DEFINED
struct av1_ext_ref_frame {
    aom_image_t *img;
    int num;
};
typedef struct av1_ext_ref_frame av1_ext_ref_frame_t;
#define AV1_EXT_REF_FRAME_T_DEFINED
#endif

/* Forward declarations for AV1 codec interfaces */
extern const struct aom_codec_iface *aom_codec_av1_cx(void);
extern const struct aom_codec_iface *aom_codec_av1_dx(void);

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size < 1) return 0;

    int val = data[0];
    int ret;

    // AV1E_SET_ENABLE_FILTER_INTRA
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_FILTER_INTRA, val);
    (void)ret;

    if (size > 1) {
        val = data[1];
        // AV1E_SET_ENABLE_RECT_TX
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_RECT_TX, val);
        (void)ret;
    }

    if (size > 2) {
        val = data[2];
        // AV1E_SET_ENABLE_DIFF_WTD_COMP
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);
        (void)ret;
    }

    if (size > 3) {
        val = data[3];
        // AV1E_SET_ENABLE_AB_PARTITIONS
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_AB_PARTITIONS, val);
        (void)ret;
    }

    if (size > 4) {
        val = data[4];
        // AV1E_SET_ENABLE_DIST_WTD_COMP
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
        (void)ret;
    }

    return 1;
}

static int fuzz_decoder_ext_ref(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size < 1) return 0;

    // Construct a dummy external reference frame structure
    av1_ext_ref_frame_t ext_ref;
    ext_ref.num = 0;
    ext_ref.img = NULL;

    // If there is enough data, try to construct a dummy image
    if (size >= 16) {
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        img.fmt = AOM_IMG_FMT_I420;
        img.w = 16;
        img.h = 16;
        img.bit_depth = 8;
        img.d_w = 16;
        img.d_h = 16;
        img.r_w = 16;
        img.r_h = 16;
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 1;
        
        // Point to input data as dummy image data
        img.planes[0] = (unsigned char *)data;
        img.planes[1] = (unsigned char *)(data + 16);
        img.planes[2] = (unsigned char *)(data + 16);
        img.stride[0] = 16;
        img.stride[1] = 8;
        img.stride[2] = 8;
        img.sz = 16 * 16 * 3 / 2;

        ext_ref.num = 1;
        ext_ref.img = &img;
    }

    // AV1D_SET_EXT_REF_PTR
    int ret = aom_codec_control(codec, AV1D_SET_EXT_REF_PTR, &ext_ref);
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize Encoder
    aom_codec_ctx_t enc_codec;
    const aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&enc_codec, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder controls
    fuzz_encoder_controls(&enc_codec, Data, Size);

    // Cleanup encoder
    aom_codec_destroy(&enc_codec);

    // Initialize Decoder
    aom_codec_ctx_t dec_codec;
    const aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    // Removed invalid members: allow_screen_content_tools, allow_intrabc
    // These are typically encoder settings or handled differently in newer API versions

    if (aom_codec_dec_init(&dec_codec, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz decoder external reference control
    fuzz_decoder_ext_ref(&dec_codec, Data, Size);

    // Cleanup decoder
    aom_codec_destroy(&dec_codec);

    return 0;
}