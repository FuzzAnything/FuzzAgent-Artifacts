// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg = {0};
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    // Initialize Encoder
    if (init_encoder(&enc_ctx, &cfg) != 0) {
        return 0;
    }

    // Initialize Decoder
    if (init_decoder(&dec_ctx) != 0) {
        cleanup_encoder(&enc_ctx);
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    if (Data[0] < 256) {
        int val = Data[0];
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_FLIP_IDTX, &val);
    }

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    if (Data[1] < 256) {
        int val = Data[1];
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_WARPED_MOTION, &val);
    }

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    if (Size > 2 && Data[2] < 256) {
        int val = Data[2];
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, &val);
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 3 && Data[3] < 256) {
        int val = Data[3];
        aom_codec_control(&enc_ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &val);
    }

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    // The type aom_loopfilter_control_t is internal or not exposed in standard headers in some versions.
    // We skip this specific control if the type is not available to ensure compilation.
    // If the library version supports it via a specific header not listed, it would need that header.
    // For robustness against missing type definitions, we omit this specific call or use a generic int if known.
    // Based on common libaom usage, this often takes a struct. Without the definition, we cannot safely construct it.
    // We will skip this specific control to avoid compilation errors.

    // Fuzz AV1D_SET_EXT_REF_PTR
    // Requires av1_ext_ref_frame_t
    if (Size > 5) {
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));
        
        // Create a dummy image if data is sufficient
        if (Size > 10) {
            aom_image_t img;
            memset(&img, 0, sizeof(img));
            img.w = (Data[5] % 1024) + 16;
            img.h = (Data[6] % 1024) + 16;
            img.fmt = AOM_IMG_FMT_I420;
            img.bit_depth = 8;
            img.d_w = img.w;
            img.d_h = img.h;
            img.r_w = img.w;
            img.r_h = img.h;
            
            // Allocate minimal buffer for planes to avoid NULL pointer issues in some implementations
            size_t plane_size = img.w * img.h;
            unsigned char *buf = malloc(plane_size * 2); // Y + UV approx
            if (buf) {
                img.planes[0] = buf;
                img.stride[0] = img.w;
                img.planes[1] = buf + plane_size;
                img.stride[1] = img.w;
                img.planes[2] = buf + plane_size + (plane_size / 2);
                img.stride[2] = img.w;
                
                ext_ref.img = &img;
                ext_ref.num = (Data[7] % 4) + 1;
                
                aom_codec_control(&dec_ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
                
                free(buf);
            }
        } else {
            ext_ref.num = 0;
            ext_ref.img = NULL;
            aom_codec_control(&dec_ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
        }
    }

    cleanup_encoder(&enc_ctx);
    cleanup_decoder(&dec_ctx);

    return 0;
}