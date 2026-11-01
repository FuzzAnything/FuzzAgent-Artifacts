// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"

/*
 * Fuzz driver for libaom codec control functions.
 * Targets:
 * - AV1D_GET_FRAME_SIZE
 * - AV1D_GET_DISPLAY_SIZE
 * - AV1D_GET_IMG_FORMAT
 * - AV1D_SET_OUTPUT_ALL_LAYERS
 * - AV1E_SET_ENABLE_FILTER_INTRA
 * - AV1D_SET_OPERATING_POINT
 */

/* Helper to create dummy data for decoder input */
static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    if (size > 0 && data) {
        fwrite(data, 1, size, f);
    } else {
        // Write minimal valid AV1 bitstream header if possible, or just zeros
        unsigned char dummy[] = {0x42, 0x81, 0x01, 0x00}; 
        fwrite(dummy, 1, sizeof(dummy), f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize decoder context */
    aom_codec_ctx_t ctx_dec;
    aom_codec_iface_t *iface_dec = aom_codec_av1_dx();
    if (!iface_dec) return 0;

    aom_codec_dec_cfg_t cfg_dec;
    memset(&cfg_dec, 0, sizeof(cfg_dec));
    cfg_dec.threads = 1;
    cfg_dec.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx_dec, iface_dec, &cfg_dec, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Initialize encoder context */
    aom_codec_ctx_t ctx_enc;
    aom_codec_iface_t *iface_enc = aom_codec_av1_cx();
    if (!iface_enc) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    aom_codec_enc_cfg_t cfg_enc;
    aom_codec_enc_config_default(iface_enc, &cfg_enc, 0);
    cfg_enc.g_w = 128;
    cfg_enc.g_h = 128;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx_enc, iface_enc, &cfg_enc, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    /* Create dummy file for decoder input if needed */
    if (create_dummy_file("./dummy_file", Data, Size) != 0) {
        aom_codec_destroy(&ctx_dec);
        aom_codec_destroy(&ctx_enc);
        return 0;
    }

    /* Fuzz Decoder Controls */
    {
        int width, height;
        int display_width, display_height;
        aom_img_fmt_t fmt;
        int all_layers = 0;
        int operating_point = 0;

        /* AV1D_GET_FRAME_SIZE */
        if (aom_codec_control(&ctx_dec, AV1D_GET_FRAME_SIZE, &width, &height) == AOM_CODEC_OK) {
            (void)width; (void)height;
        }

        /* AV1D_GET_DISPLAY_SIZE */
        if (aom_codec_control(&ctx_dec, AV1D_GET_DISPLAY_SIZE, &display_width, &display_height) == AOM_CODEC_OK) {
            (void)display_width; (void)display_height;
        }

        /* AV1D_GET_IMG_FORMAT */
        if (aom_codec_control(&ctx_dec, AV1D_GET_IMG_FORMAT, &fmt) == AOM_CODEC_OK) {
            (void)fmt;
        }

        /* AV1D_SET_OUTPUT_ALL_LAYERS */
        if (Size > 0) {
            all_layers = (Data[0] & 1);
            aom_codec_control(&ctx_dec, AV1D_SET_OUTPUT_ALL_LAYERS, all_layers);
        }

        /* AV1D_SET_OPERATING_POINT */
        if (Size > 1) {
            operating_point = Data[1] % 8;
            aom_codec_control(&ctx_dec, AV1D_SET_OPERATING_POINT, operating_point);
        }
    }

    /* Fuzz Encoder Controls */
    {
        int enable_filter_intra = 0;
        if (Size > 2) {
            enable_filter_intra = (Data[2] & 1);
            aom_codec_control(&ctx_enc, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);
        }
    }

    /* Cleanup */
    aom_codec_destroy(&ctx_dec);
    aom_codec_destroy(&ctx_enc);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}