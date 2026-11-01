// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) return 0;

    if (aom_codec_dec_init(ctx, decoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (size > 0) {
        aom_image_t *frame = NULL;
        aom_codec_iter_t iter = NULL;
        aom_codec_err_t res = aom_codec_decode(ctx, data, size, NULL);
        
        if (res == AOM_CODEC_OK) {
            while ((frame = aom_codec_get_frame(ctx, &iter)) != NULL) {
                aom_img_free(frame);
            }
        }
    }
    
    aom_codec_destroy(ctx);
    return 0;
}

static int fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) return 0;

    if (aom_codec_enc_init(ctx, encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    
    // Fill image with fuzzed data safely
    size_t plane_size = 64 * 64;
    size_t offset = 0;
    for (int p = 0; p < 3; p++) {
        size_t bytes_to_copy = (p == 0) ? plane_size : (plane_size / 4);
        if (offset + bytes_to_copy <= size) {
            memcpy(img.planes[p], data + offset, bytes_to_copy);
            offset += bytes_to_copy;
        } else {
            memset(img.planes[p], 0, bytes_to_copy);
        }
    }
    img.planes[3] = NULL;

    // Test target API functions
    // AV1E_SET_ENABLE_RECT_PARTITIONS
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, 1);
    // AV1E_SET_SINGLE_TILE_DECODING
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, 1);
    
    // Try to set tile mode if data is sufficient
    if (size >= 8) {
        uint32_t tile_cols = *(uint32_t*)(data);
        uint32_t tile_rows = *(uint32_t*)(data + 4);
        aom_tile_info tile_info;
        memset(&tile_info, 0, sizeof(tile_info));
        tile_info.tile_columns = (tile_cols % 32) + 1;
        tile_info.tile_rows = (tile_rows % 32) + 1;
        for (int i = 0; i < AOM_MAX_TILE_COLS; i++) tile_info.tile_widths[i] = 1;
        for (int i = 0; i < AOM_MAX_TILE_ROWS; i++) tile_info.tile_heights[i] = 1;
        tile_info.num_tile_groups = 1;
        aom_codec_control(ctx, AV1_SET_TILE_MODE, &tile_info);
    }

    // AV1_INVERT_TILE_DECODE_ORDER
    aom_codec_control(ctx, AV1_INVERT_TILE_DECODE_ORDER, 1);

    // AV1D_GET_FRAME_SIZE (Decoder control, might fail on encoder but tests error handling)
    int frame_width, frame_height;
    aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, &frame_width);
    aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, &frame_height);

    // AOMD_GET_TILE_INFO
    aom_tile_info tile_info_out;
    memset(&tile_info_out, 0, sizeof(tile_info_out));
    aom_codec_control(ctx, AOMD_GET_TILE_INFO, &tile_info_out);

    // Use 0 for flags instead of AOM_EFLAG_NONE if not defined
    // Fixed: Removed the 6th argument (deadline) as the signature expects 5 arguments
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_codec_destroy(ctx);
    aom_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Alternate between decoder and encoder fuzzing based on input
    if (Data[0] & 1) {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));
        fuzz_decoder(&ctx, Data + 1, Size > 1 ? Size - 1 : 0);
    } else {
        aom_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));
        fuzz_encoder(&ctx, Data + 1, Size > 1 ? Size - 1 : 0);
    }

    return 0;
}