// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_control(aom_codec_ctx_t *ctx, int id, void *arg) {
    if (!ctx || !arg) return 0;
    return aom_codec_control(ctx, id, arg);
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to configure encoder settings
    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    cfg.g_timebase.den = Data[3] ? Data[3] : 1;
    cfg.rc_target_bitrate = Size > 4 ? (int)(Data[4] * 1000) : 1000;
    cfg.g_error_resilient = 0;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy image for encoding to ensure context is valid
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image with dummy data
    for (int i = 0; i < img.h * img.stride[0]; i++) {
        img.planes[0][i] = 128;
    }
    // Fill UV planes
    for (int i = 0; i < (img.h / 2) * img.stride[1]; i++) {
        img.planes[1][i] = 128;
        img.planes[2][i] = 128;
    }

    // Encode the frame to initialize internal state
    // aom_codec_encode signature: aom_codec_ctx_t *ctx, const aom_image_t *img,
    // aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags
    err = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Extract control IDs from input data to fuzz different controls
    int control_id = (int)Data[0];
    int value = (int)Data[1];
    int *value_ptr = &value;
    int *get_ptr = NULL;
    
    // Map input byte to specific control IDs
    switch (control_id % 6) {
        case 0:
            // AV1E_SET_CHROMA_SAMPLE_POSITION
            // Values: 0, 1, 2, 3
            value = value % 4;
            fuzz_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, &value);
            break;
        case 1:
            // AV1E_SET_ENABLE_INTERINTRA_COMP
            // Values: 0, 1
            value = value % 2;
            fuzz_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, &value);
            break;
        case 2:
            // AV1E_SET_ENABLE_RECT_PARTITIONS
            // Values: 0, 1
            value = value % 2;
            fuzz_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, &value);
            break;
        case 3:
            // AV1E_GET_BASELINE_GF_INTERVAL
            // This is a GET operation, we need a pointer to store the result
            get_ptr = (int*)malloc(sizeof(int));
            if (get_ptr) {
                fuzz_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, get_ptr);
                free(get_ptr);
            }
            break;
        case 4:
            // AV1E_SET_ENABLE_TX_SIZE_SEARCH
            // Values: 0, 1
            value = value % 2;
            fuzz_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &value);
            break;
        case 5:
            // AV1E_SET_MIN_GF_INTERVAL
            // Values: 0, 1, 2, ...
            value = value % 10;
            fuzz_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, &value);
            break;
        default:
            break;
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}