// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
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

int LLVMFuzzerTestOneInput_276(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    int use_encoder = (Data[0] % 2) == 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_fixed_buf_t *global_headers = NULL;

    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) {
            return 0;
        }

        aom_codec_enc_cfg_t cfg;
        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            return 0;
        }

        // Fuzz some config parameters based on input data
        cfg.g_w = (Data[1] % 256) + 16;
        cfg.g_h = (Data[2] % 256) + 16;
        cfg.g_timebase.num = (Data[3] % 100) + 1;
        cfg.g_timebase.den = (Data[4] % 100) + 1;
        cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

        ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            return 0;
        }

        // Optionally send a dummy frame to trigger configuration finalization
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
        memset(img.planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
        memset(img.planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

        // Use correct control constants
        aom_codec_control(&ctx, AOME_SET_CPUUSED, (Data[6] % 8) + 1);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, (Data[7] % 2));
        aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, (Data[8] % 2));
        aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, (Data[9] % 8) + 1);
        aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, (Data[10] % 8) + 1);
        
        // Try to set loop filter control if available (fuzzed value)
        int loopfilter_level = Data[11] % 64;
        // Note: AV1E_SET_LOOPFILTER_LEVEL might not exist, using a generic control if needed
        // For now, we skip setting it directly if the constant is missing or use a known one
        
        // Try to set film grain test vector (if supported)
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (Data[12] % 2));
        
        // Try to set partition info path (if supported)
        // AV1E_SET_PARTITION_INFO_PATH expects a string path, not an int.
        // We provide a valid string path to avoid null pointer dereference.
        const char *partition_path = "./dummy_file";
        aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, partition_path);
        
        // Try to set film grain table (if supported)
        // The previous crash occurred here because AV1E_SET_FILM_GRAIN_TABLE likely expects
        // a pointer to a structure or string, not an integer. Passing an integer cast to void*
        // resulted in a null/invalid pointer dereference inside the library.
        // We skip this specific control to avoid the crash unless we have the correct struct.
        // aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, (Data[14] % 2)); 
        
        // Try to set matrix coefficients (if supported)
        // Similarly, this might expect a specific enum or struct, but often enums are passed as ints.
        // We keep it but ensure it's a valid range if it's an enum.
        aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, (Data[15] % 8));

        ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
        aom_img_free(&img);

        // Now retrieve global headers
        global_headers = aom_codec_get_global_headers(&ctx);
        if (global_headers) {
            // Free the buffer as per documentation
            if (global_headers->buf) {
                free(global_headers->buf);
            }
            free(global_headers);
            global_headers = NULL;
        }

        cleanup_encoder(&ctx);
    } else {
        // Decoder setup
        iface = aom_codec_av1_dx();
        if (!iface) {
            return 0;
        }

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (Data[1] % 4) + 1;

        ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            return 0;
        }

        // Try to get global headers (though typically for encoder)
        global_headers = aom_codec_get_global_headers(&ctx);
        if (global_headers) {
            if (global_headers->buf) {
                free(global_headers->buf);
            }
            free(global_headers);
            global_headers = NULL;
        }

        cleanup_decoder(&ctx);
    }

    return 0;
}