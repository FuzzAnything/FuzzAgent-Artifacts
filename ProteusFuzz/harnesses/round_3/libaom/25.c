// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

static int fuzzer_test_codec_control(const uint8_t *Data, size_t Size, int control_id, int arg_val) {
    aom_codec_ctx_t ctx;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;
    int value_to_pass = arg_val;

    // Initialize encoder config
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config to allow initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_GET_NUM_OPERATING_POINTS requires the encoder to have processed at least one frame
    // or be in a state where sequence parameters are fully initialized.
    if (control_id == AV1E_GET_NUM_OPERATING_POINTS) {
        // Create a dummy frame to ensure sequence parameters are set
        aom_image_t img;
        aom_codec_err_t frame_ret;
        
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        
        // aom_codec_encode signature: (ctx, img, pts, duration, flags)
        frame_ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
        aom_img_free(&img);
        
        if (frame_ret != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        // Now try to get the operating points
        ret = aom_codec_control(&ctx, control_id, &value_to_pass);
    } else {
        // For SET operations, pass the value directly
        ret = aom_codec_control(&ctx, control_id, value_to_pass);
    }
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    int control_selector = Data[0];
    int arg_val = 0;
    if (Size > 1) {
        arg_val = Data[1];
    }

    // Map input to known control IDs using the actual enum values from aomcx.h
    int control_id = 0;
    switch (control_selector % 6) {
        case 0:
            control_id = AV1E_SET_ENABLE_TX_SIZE_SEARCH;
            break;
        case 1:
            control_id = AV1E_SET_ENABLE_GLOBAL_MOTION;
            break;
        case 2:
            control_id = AV1E_GET_NUM_OPERATING_POINTS;
            break;
        case 3:
            control_id = AV1E_SET_FILM_GRAIN_TEST_VECTOR;
            break;
        case 4:
            control_id = AV1E_SET_ENABLE_DIFF_WTD_COMP;
            break;
        case 5:
            control_id = AV1E_SET_INTRA_DEFAULT_TX_ONLY;
            break;
        default:
            return 0;
    }

    fuzzer_test_codec_control(Data, Size, control_id, arg_val);

    return 0;
}