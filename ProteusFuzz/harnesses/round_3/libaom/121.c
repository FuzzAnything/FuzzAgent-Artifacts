// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    const aom_codec_cx_pkt_t *pkt;
    aom_fixed_buf_t *global_headers;
    aom_codec_err_t res;
    int i;

    if (size < 10) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzzer data to configure encoder settings within safe bounds
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 255) + 1;
    cfg.rc_target_bitrate = (data[3] % 100) + 100;
    cfg.g_profile = data[4] % 4;
    cfg.g_threads = (data[5] % 4) + 1;
    cfg.g_pass = 1; // One pass encoding for simplicity

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Initialize image structure
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (aom_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, 1) == NULL) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzer data
    // img.planes is an array of pointers, not a struct with stride member directly accessible as img.planes[i].stride
    // Correct access: img.planes[i] is a pointer to uint8_t, stride is in img.stride[i]
    for (i = 0; i < 3; i++) {
        size_t plane_size = img.stride[i] * (i == 0 ? img.d_h : (img.d_h + 1) / 2);
        if (plane_size > 0) {
            size_t copy_size = (size < plane_size) ? size : plane_size;
            memcpy(img.planes[i], data, copy_size);
            if (copy_size < plane_size) {
                memset(img.planes[i] + copy_size, 0, plane_size - copy_size);
            }
        }
    }

    // Control: AV1E_SET_ERROR_RESILIENT_MODE
    aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, data[6] % 2);

    // Control: AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, data[7] % 4);

    // Control: AV1E_SET_ENABLE_DIFF_WTD_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, data[8] % 2);

    // Control: AV1E_SET_PARTITION_INFO_PATH
    // Create dummy file as requested
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
    // Attempt to call control. Note: If this macro expects a struct, this might fail at runtime or compile time.
    // Based on typical usage, it might expect a path string or a specific struct. 
    // Without the struct definition, we pass the string. If compilation fails, the macro definition is needed.
    // Assuming it accepts a const char* for the path.
    aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, "./dummy_file");

    // Control: AV1E_SET_FILM_GRAIN_TABLE
    // Skipped due to missing struct definition in provided context.
    // aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, &params);

    // Encode a frame
    // aom_codec_encode signature: (ctx, img, pts, duration, flags) - 5 args
    res = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Get global headers
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // Drain the encoder
    // aom_codec_get_cx_data returns a const pointer, not an error code directly in the assignment
    // The function returns the packet pointer on success, NULL on error.
    // We need to check the return value of the function call.
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            if (pkt->data.frame.buf) {
                free(pkt->data.frame.buf);
            }
        }
        // AOM_CODEC_CX_GLOBAL_HEADER_PKT is not a standard enum value in the common header
        // Global headers are usually handled via aom_codec_get_global_headers
        // So we skip checking for that specific kind in the drain loop if it doesn't exist.
    }

    aom_img_free(&img);
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_encoder(Data, Size);

    return 0;
}