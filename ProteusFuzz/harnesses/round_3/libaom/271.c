// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

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

int LLVMFuzzerTestOneInput_271(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    // Initialize basic config from input data
    int width = (Data[0] % 16) + 16;
    int height = (Data[1] % 16) + 16;
    int bit_depth = (Data[2] % 8) + 8;
    int usage = Data[3] % 2;

    // Try Encoder Path
    iface = aom_codec_av1_cx();
    if (iface) {
        memset(&ctx, 0, sizeof(ctx));
        memset(&cfg, 0, sizeof(cfg));

        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res == AOM_CODEC_OK) {
            cfg.g_w = width;
            cfg.g_h = height;
            cfg.g_bit_depth = bit_depth;
            cfg.g_usage = usage;

            res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Fuzz AV1E_SET_S_FRAME_MODE
                int s_frame_mode = (Data[4] % 2);
                aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, &s_frame_mode);

                // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
                int enable_mvs = (Data[5] % 2);
                aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, &enable_mvs);

                // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
                int enable_filter = (Data[6] % 2);
                aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, &enable_filter);

                // Prepare reference frame structures for decoder-like controls if applicable
                av1_ref_frame_t ref_frame;
                memset(&ref_frame, 0, sizeof(ref_frame));
                ref_frame.idx = (Data[7] % 7); // Valid indices usually 0-6
                ref_frame.use_external_ref = (Data[8] % 2);
                
                // Initialize a dummy image for reference frame operations
                aom_image_t img;
                memset(&img, 0, sizeof(img));
                img.w = width;
                img.h = height;
                img.bit_depth = bit_depth;
                img.fmt = AOM_IMG_FMT_I420;
                
                // Allocate planes manually for fuzzing safety
                int stride = width * (bit_depth / 8);
                int size = stride * height;
                img.planes[0] = malloc(size);
                img.planes[1] = malloc(size / 2);
                img.planes[2] = malloc(size / 2);
                img.stride[0] = stride;
                img.stride[1] = stride / 2;
                img.stride[2] = stride / 2;
                img.sz = size + (size / 2) * 2;
                
                if (img.planes[0] && img.planes[1] && img.planes[2]) {
                    memset(img.planes[0], 0, size);
                    memset(img.planes[1], 0, size / 2);
                    memset(img.planes[2], 0, size / 2);
                    img.img_data = img.planes[0];
                    img.img_data_owner = 1;
                    img.self_allocd = 1;
                    
                    ref_frame.img = img;

                    // Fuzz AV1_SET_REFERENCE
                    aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_frame);

                    // Fuzz AV1_COPY_REFERENCE
                    aom_codec_control(&ctx, AV1_COPY_REFERENCE, &ref_frame);

                    // Fuzz AV1_GET_REFERENCE
                    aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref_frame);
                }

                cleanup_encoder(&ctx);
                
                // Cleanup image
                free(img.planes[0]);
                free(img.planes[1]);
                free(img.planes[2]);
            }
        }
    }

    // Try Decoder Path
    iface = aom_codec_av1_dx();
    if (iface) {
        memset(&ctx, 0, sizeof(ctx));
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        // Corrected fields based on aom_codec_dec_cfg_t structure
        dec_cfg.threads = 1;
        dec_cfg.allow_lowbitdepth = 1;

        res = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (res == AOM_CODEC_OK) {
            // Attempt decoder-specific reference controls
            av1_ref_frame_t ref_frame;
            memset(&ref_frame, 0, sizeof(ref_frame));
            ref_frame.idx = (Data[9] % 7);
            ref_frame.use_external_ref = (Data[10] % 2);
            
            aom_image_t img;
            memset(&img, 0, sizeof(img));
            img.w = width;
            img.h = height;
            img.bit_depth = bit_depth;
            img.fmt = AOM_IMG_FMT_I420;
            
            int stride = width * (bit_depth / 8);
            int size = stride * height;
            img.planes[0] = malloc(size);
            img.planes[1] = malloc(size / 2);
            img.planes[2] = malloc(size / 2);
            img.stride[0] = stride;
            img.stride[1] = stride / 2;
            img.stride[2] = stride / 2;
            img.sz = size + (size / 2) * 2;

            if (img.planes[0] && img.planes[1] && img.planes[2]) {
                memset(img.planes[0], 0, size);
                memset(img.planes[1], 0, size / 2);
                memset(img.planes[2], 0, size / 2);
                img.img_data = img.planes[0];
                img.img_data_owner = 1;
                img.self_allocd = 1;
                
                ref_frame.img = img;

                aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_frame);
                aom_codec_control(&ctx, AV1_COPY_REFERENCE, &ref_frame);
                aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref_frame);

                free(img.planes[0]);
                free(img.planes[1]);
                free(img.planes[2]);
            }

            cleanup_decoder(&ctx);
        }
    }

    return 0;
}