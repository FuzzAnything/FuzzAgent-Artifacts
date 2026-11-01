// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int try_fuzz_decoder(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    const aom_codec_iface_t *iface;
    uint32_t op_point;
    int all_layers;
    aom_codec_iter_t iter = NULL;

    if (Size < 8) return 0;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (Data[0] % 16) + 1;
    cfg.w = (Data[1] * Data[2]) % 4096;
    cfg.h = (Data[3] * Data[4]) % 4096;
    if (cfg.w == 0) cfg.w = 1;
    if (cfg.h == 0) cfg.h = 1;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Try AV1D_SET_OPERATING_POINT
    op_point = (Data[5] << 8) | Data[6];
    ret = aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, op_point);
    (void)ret;

    // Try AV1D_SET_OUTPUT_ALL_LAYERS
    all_layers = Data[7] % 2;
    ret = aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, all_layers);
    (void)ret;

    // Try AV1D_GET_DISPLAY_SIZE
    ret = aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, &cfg);
    (void)ret;

    // Feed some data to trigger decoding logic
    if (Size > 8) {
        ret = aom_codec_decode(&ctx, Data + 8, Size - 8, NULL);
        (void)ret;

        // Attempt to get a frame to ensure state exploration
        aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
        if (frame) {
            (void)frame;
        }
    }

    aom_codec_destroy(&ctx);
    return 0;
}

static int try_fuzz_encoder(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    const aom_codec_iface_t *iface;
    uint32_t svc_mode;
    int spatial_id;
    int num_layers;

    if (Size < 9) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Fixed: Added usage parameter (0)
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[1] * Data[2]) % 4096;
    cfg.g_h = (Data[3] * Data[4]) % 4096;
    if (cfg.g_w == 0) cfg.g_w = 1;
    if (cfg.g_h == 0) cfg.g_h = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Try AOME_SET_NUMBER_SPATIAL_LAYERS
    num_layers = (Data[5] % 4) + 1;
    ret = aom_codec_control(&ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, num_layers);
    (void)ret;

    // Try AOME_SET_SPATIAL_LAYER_ID
    spatial_id = Data[6] % 4;
    ret = aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, spatial_id);
    (void)ret;

    // Try AV1E_SET_SVC_FRAME_DROP_MODE
    svc_mode = (Data[7] << 8) | Data[8];
    ret = aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, svc_mode);
    (void)ret;

    // Encode a dummy frame if data is available
    if (Size > 9) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        
        // Fill with some data from the input buffer
        size_t fill_size = (size_t)cfg.g_w * cfg.g_h;
        if (fill_size > Size - 9) fill_size = Size - 9;
        if (fill_size > 0) {
            memcpy(img.planes[0], Data + 9, fill_size);
        }
        
        // Fixed: Corrected type to aom_enc_frame_flags_t and removed extra argument
        aom_enc_frame_flags_t flags = 0;
        ret = aom_codec_encode(&ctx, &img, 0, 1, flags);
        (void)ret;
        
        aom_img_free(&img);
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Alternate between decoder and encoder fuzzing based on input
    if (Data[0] & 0x01) {
        return try_fuzz_decoder(Data, Size);
    } else {
        return try_fuzz_encoder(Data, Size);
    }
}