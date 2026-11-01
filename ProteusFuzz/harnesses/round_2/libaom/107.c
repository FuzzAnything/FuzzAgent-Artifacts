// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int init_encoder(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 30;
    cfg.g_timebase.den = 1;
    cfg.rc_target_bitrate = 500;
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static uint8_t get_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (Size > index) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Try Decoder API
    if (init_decoder(&ctx)) {
        // AV1_SET_REFERENCE (Decoder)
        av1_ref_frame_t ref_dec;
        memset(&ref_dec, 0, sizeof(ref_dec));
        ref_dec.idx = get_byte(Data, Size, 0) % 16;
        ref_dec.use_external_ref = get_byte(Data, Size, 1) % 2;
        aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_dec);

        // AV1_GET_REFERENCE (Decoder)
        av1_ref_frame_t get_ref;
        memset(&get_ref, 0, sizeof(get_ref));
        get_ref.idx = get_byte(Data, Size, 2) % 16;
        aom_codec_control(&ctx, AV1_GET_REFERENCE, &get_ref);

        // AV1_COPY_REFERENCE (Decoder)
        av1_ref_frame_t copy_ref;
        memset(&copy_ref, 0, sizeof(copy_ref));
        copy_ref.idx = get_byte(Data, Size, 3) % 16;
        aom_codec_control(&ctx, AV1_COPY_REFERENCE, &copy_ref);

        // Feed some data to potentially generate frames for reference
        if (Size > 10) {
            aom_codec_decode(&ctx, Data, Size, NULL);
        }

        cleanup_codec(&ctx);
    }

    // Try Encoder API
    memset(&ctx, 0, sizeof(ctx));
    if (init_encoder(&ctx)) {
        // AV1E_SET_MAX_REFERENCE_FRAMES (Encoder)
        int max_refs = get_byte(Data, Size, 0) % 8 + 1; // 1 to 8
        aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_refs);

        // AV1E_SET_ENABLE_REF_FRAME_MVS (Encoder)
        int enable_mvs = get_byte(Data, Size, 1) % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, &enable_mvs);

        // AV1E_SET_QM_MAX (Encoder)
        int qm_max = get_byte(Data, Size, 2) % 64;
        aom_codec_control(&ctx, AV1E_SET_QM_MAX, &qm_max);

        // Encode a dummy frame to exercise state
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        img.w = 64;
        img.h = 64;
        img.fmt = AOM_IMG_FMT_I420;
        img.bit_depth = 8;
        
        // Allocate memory for image planes to avoid using fuzz data directly as pointers
        unsigned char *y_plane = (unsigned char *)malloc(64 * 64);
        unsigned char *u_plane = (unsigned char *)malloc(32 * 32);
        unsigned char *v_plane = (unsigned char *)malloc(32 * 32);

        if (y_plane && u_plane && v_plane) {
            // Fill with fuzz data if available, otherwise zeros
            size_t y_size = 64 * 64;
            size_t uv_size = 32 * 32;
            
            if (Size >= y_size) {
                memcpy(y_plane, Data, y_size);
            } else {
                memset(y_plane, 0, y_size);
            }
            
            if (Size >= y_size + uv_size) {
                memcpy(u_plane, Data + y_size, uv_size);
            } else {
                memset(u_plane, 0, uv_size);
            }

            if (Size >= y_size + 2 * uv_size) {
                memcpy(v_plane, Data + y_size + uv_size, uv_size);
            } else {
                memset(v_plane, 0, uv_size);
            }

            img.planes[0] = y_plane;
            img.planes[1] = u_plane;
            img.planes[2] = v_plane;
            img.stride[0] = 64;
            img.stride[1] = 32;
            img.stride[2] = 32;

            aom_codec_encode(&ctx, &img, 0, 1, 0);
        }

        free(y_plane);
        free(u_plane);
        free(v_plane);

        cleanup_codec(&ctx);
    }

    return 0;
}