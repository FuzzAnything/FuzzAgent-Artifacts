// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_dec_cfg_t cfg;
    
    // Extract parameters from fuzz input
    int width = Data[0] ? Data[0] : 16;
    int height = Data[1] ? Data[1] : 16;
    int bit_depth = (Data[2] % 8) + 8; // 8 to 15
    int color_space = Data[3] % 2;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize decoder config with valid fields for aom_codec_dec_cfg
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Prepare dummy image for external reference if needed
    aom_image_t dummy_img;
    memset(&dummy_img, 0, sizeof(dummy_img));
    dummy_img.w = width;
    dummy_img.h = height;
    dummy_img.stride[0] = width;
    dummy_img.stride[1] = (width + 1) / 2;
    dummy_img.stride[2] = (width + 1) / 2;
    dummy_img.planes[0] = malloc(width * height);
    dummy_img.planes[1] = malloc(((width + 1) / 2) * ((height + 1) / 2));
    dummy_img.planes[2] = malloc(((width + 1) / 2) * ((height + 1) / 2));
    
    if (!dummy_img.planes[0] || !dummy_img.planes[1] || !dummy_img.planes[2]) {
        aom_codec_destroy(&ctx);
        free(dummy_img.planes[0]);
        free(dummy_img.planes[1]);
        free(dummy_img.planes[2]);
        return 0;
    }
    memset(dummy_img.planes[0], 0, width * height);
    memset(dummy_img.planes[1], 0, ((width + 1) / 2) * ((height + 1) / 2));
    memset(dummy_img.planes[2], 0, ((width + 1) / 2) * ((height + 1) / 2));

    // Decode one frame to ensure internal state is populated for control calls
    // We pass the raw data as a packet.
    aom_codec_decode(&ctx, Data, Size, NULL);

    // 1. aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES
    int ref_updates[8];
    memset(ref_updates, 0, sizeof(ref_updates));
    aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);

    // 2. aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED
    int corrupted = 0;
    aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);

    // 3. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
    av1_ext_ref_frame_t ext_ref;
    ext_ref.img = &dummy_img;
    ext_ref.num = 1;
    aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);

    // 4. aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT
    int altref_present = 0;
    aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

    // 5. aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED
    int last_ref_used[8];
    memset(last_ref_used, 0, sizeof(last_ref_used));
    aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, last_ref_used);

    // 6. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer);

    // Cleanup
    aom_codec_destroy(&ctx);
    free(dummy_img.planes[0]);
    free(dummy_img.planes[1]);
    free(dummy_img.planes[2]);

    return 0;
}