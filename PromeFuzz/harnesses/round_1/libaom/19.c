// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

/* Declare the encoder interface function since it's not in the included headers */
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static const char *option_keys[] = {
    "cpu-used", "end-usage", "lag-in-frames", "error-resilient", 
    "aq-mode", "min-q", "max-q", "undershoot-pct", "overshoot-pct",
    "buf-sz", "buf-initial-sz", "buf-optimal-sz", "dropframe-thresh",
    "resize-mode", "superres-mode", "tile-columns", "tile-rows",
    "enable-cdef", "enable-restoration", "enable-rect-partitions",
    "enable-ab-partitions", "enable-1to4-partitions", "enable-flip-idtx",
    "enable-intrabc", "enable-palette", "enable-cfl", "enable-smooth-intra",
    "enable-filter-intra", "enable-dual-filter", "enable-tx64",
    "enable-dist-wtd-comp", "enable-masked-comp", "enable-obmc",
    "enable-warp-motion", "enable-global-motion", "enable-interintra-comp",
    "enable-interinter-wedge", "enable-interintra-wedge", "enable-reduced-tx-set",
    "enable-reduced-ref-set", "enable-ref-frame-mvs", "enable-trellis-quant",
    "enable-paeth-intra", "enable-smooth-interintra", "enable-intra-edge-filter",
    "enable-intra-angle-delta", "monochrome", "full-still-picture-hdr",
    "save-as-annexb", "large-scale-tile", "sframe-dist", "sframe-mode",
    "kf-min-dist", "kf-max-dist", "fwd-kf-enabled", "use-fixed-qp-offsets"
};

static const char *option_values[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "vbr", "cbr", "cq", "q",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
    "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
    "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",
    "61", "62", "63", "64", "65", "66", "67", "68", "69", "70",
    "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",
    "81", "82", "83", "84", "85", "86", "87", "88", "89", "90",
    "91", "92", "93", "94", "95", "96", "97", "98", "99", "100",
    "true", "false", "yes", "no", "on", "off",
    "none", "fixed", "qthresh", "random",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
    "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
    "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",
    "61", "62", "63", "64", "65", "66", "67", "68", "69", "70",
    "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",
    "81", "82", "83", "84", "85", "86", "87", "88", "89", "90",
    "91", "92", "93", "94", "95", "96", "97", "98", "99", "100",
    "default", "normal", "high", "veryhigh", "good", "best",
    "realtime", "allintra", "good_quality", "rt", "intra_only"
};

static aom_image_t *create_test_image(unsigned int width, unsigned int height, 
                                       aom_img_fmt_t fmt, unsigned int bit_depth,
                                       const uint8_t *data, size_t data_size) {
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 16);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    size_t plane_size = width * height;
    if (fmt == AOM_IMG_FMT_I420 || fmt == AOM_IMG_FMT_YV12) {
        plane_size = width * height + 2 * ((width + 1) / 2) * ((height + 1) / 2);
    } else if (fmt == AOM_IMG_FMT_I422) {
        plane_size = width * height + 2 * ((width + 1) / 2) * height;
    } else if (fmt == AOM_IMG_FMT_I444) {
        plane_size = 3 * width * height;
    }
    
    if (data && data_size >= plane_size) {
        size_t offset = 0;
        for (int plane = 0; plane < 3; plane++) {
            if (img->planes[plane]) {
                size_t plane_bytes = img->stride[plane] * ((height + (1 << img->y_chroma_shift) - 1) >> img->y_chroma_shift);
                size_t copy_size = plane_bytes < data_size - offset ? plane_bytes : data_size - offset;
                if (copy_size > 0) {
                    memcpy(img->planes[plane], data + offset, copy_size);
                    offset += copy_size;
                }
            }
        }
    }
    
    return img;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = Data[0] % 3;
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) return 0;
    
    cfg.g_w = 64 + (Data[1] % 192);
    cfg.g_h = 64 + (Data[2] % 192);
    cfg.g_limit = 100;
    cfg.g_threads = 1 + (Data[3] % 8);
    cfg.rc_target_bitrate = 100 + (Data[1] % 9900);
    cfg.rc_min_quantizer = Data[2] % 64;
    cfg.rc_max_quantizer = 4 + (Data[3] % 60);
    if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
        unsigned int temp = cfg.rc_min_quantizer;
        cfg.rc_min_quantizer = cfg.rc_max_quantizer;
        cfg.rc_max_quantizer = temp;
    }
    
    res = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }
    
    size_t data_offset = 4;
    size_t options_to_set = (Size - data_offset) / 3;
    if (options_to_set > 20) options_to_set = 20;
    
    for (size_t i = 0; i < options_to_set && data_offset + 2 < Size; i++) {
        size_t key_idx = Data[data_offset] % (sizeof(option_keys) / sizeof(option_keys[0]));
        size_t val_idx = Data[data_offset + 1] % (sizeof(option_values) / sizeof(option_values[0]));
        aom_codec_set_option(&ctx, option_keys[key_idx], option_values[val_idx]);
        data_offset += 3;
    }
    
    aom_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.rc_target_bitrate = 50 + (Data[0] % 1950);
    new_cfg.g_threads = 1 + (Data[1] % 16);
    aom_codec_enc_config_set(&ctx, &new_cfg);
    
    unsigned char buffer[4096];
    aom_fixed_buf_t buf = {buffer, sizeof(buffer)};
    unsigned int pad_before = Data[0] % 32;
    unsigned int pad_after = Data[1] % 32;
    aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
    
    aom_img_fmt_t fmts[] = {AOM_IMG_FMT_I420, AOM_IMG_FMT_I422, AOM_IMG_FMT_I444};
    aom_img_fmt_t fmt = fmts[Data[0] % 3];
    unsigned int bit_depth = 8 + 2 * (Data[1] % 2);
    
    aom_image_t *img = create_test_image(cfg.g_w, cfg.g_h, fmt, bit_depth, 
                                         Data + data_offset, Size - data_offset);
    
    if (img) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        for (int i = 0; i < 3; i++) {
            res = aom_codec_encode(&ctx, img, pts, duration, flags);
            pts += duration;
            flags = (i == 1) ? AOM_EFLAG_FORCE_KF : 0;
        }
        
        res = aom_codec_encode(&ctx, NULL, pts, duration, flags);
        
        const aom_codec_cx_pkt_t *pkt = NULL;
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                break;
            }
        }
        
        aom_img_free(img);
    }
    
    cleanup_encoder(&ctx);
    return 0;
}