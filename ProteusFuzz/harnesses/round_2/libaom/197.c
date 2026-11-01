// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static int parse_fuzz_data(const uint8_t *Data, size_t Size, aom_image_t *img, aom_codec_pts_t *pts, unsigned long *duration, aom_enc_frame_flags_t *flags, char **option_name, char **option_value) {
    if (Size < 4) return 0;

    Data += 4;
    Size -= 4;

    // Parse option name
    if (Size > 0 && Data[0] != 0) {
        size_t name_len = 0;
        while (name_len < Size && Data[name_len] != 0) name_len++;
        if (name_len > 0) {
            *option_name = malloc(name_len + 1);
            if (*option_name) {
                memcpy(*option_name, Data, name_len);
                (*option_name)[name_len] = '\0';
                Data += name_len + 1;
                Size -= name_len + 1;
            }
        }
    } else {
        *option_name = NULL;
    }

    // Parse option value
    if (Size > 0 && Data[0] != 0) {
        size_t val_len = 0;
        while (val_len < Size && Data[val_len] != 0) val_len++;
        if (val_len > 0) {
            *option_value = malloc(val_len + 1);
            if (*option_value) {
                memcpy(*option_value, Data, val_len);
                (*option_value)[val_len] = '\0';
                Data += val_len + 1;
                Size -= val_len + 1;
            }
        }
    } else {
        *option_value = NULL;
    }

    // Parse image parameters
    if (Size >= 16) {
        img->w = *(const uint32_t *)Data;
        Data += 4;
        img->h = *(const uint32_t *)(Data);
        Data += 4;
        img->bit_depth = *(const uint32_t *)(Data);
        Data += 4;
        img->fmt = (aom_img_fmt_t)(*(const uint32_t *)(Data) % 20);
        Data += 4;

        // Clamp dimensions
        if (img->w > 4096) img->w = 4096;
        if (img->h > 4096) img->h = 4096;
        if (img->bit_depth > 12) img->bit_depth = 12;
        if (img->bit_depth < 8) img->bit_depth = 8;

        // Allocate planes
        if (img->w > 0 && img->h > 0) {
            img->stride[0] = img->w;
            img->stride[1] = (img->w + 1) / 2;
            img->stride[2] = (img->w + 1) / 2;
            size_t plane_size = img->w * img->h;
            size_t chroma_size = ((img->w + 1) / 2) * ((img->h + 1) / 2);

            img->planes[0] = malloc(plane_size);
            img->planes[1] = malloc(chroma_size);
            img->planes[2] = malloc(chroma_size);

            if (img->planes[0] && img->planes[1] && img->planes[2]) {
                memset(img->planes[0], 128, plane_size);
                memset(img->planes[1], 128, chroma_size);
                memset(img->planes[2], 128, chroma_size);
                img->img_data = NULL;
                img->img_data_owner = 0;
            } else {
                free(img->planes[0]); free(img->planes[1]); free(img->planes[2]);
                img->planes[0] = img->planes[1] = img->planes[2] = NULL;
            }
        } else {
            img->planes[0] = img->planes[1] = img->planes[2] = NULL;
        }
    }

    // Parse PTS, Duration, Flags
    if (Size >= 16) {
        *pts = *(const int64_t *)Data;
        Data += 8;
        *duration = *(const unsigned long *)(Data);
        Data += 8;
        if (Size >= 24) {
            *flags = *(const aom_enc_frame_flags_t *)(Data);
            Data += 8;
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img = {0};
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    char *option_name = NULL;
    char *option_value = NULL;
    aom_codec_iface_t *iface = NULL;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (Size > 0) {
        parse_fuzz_data(Data, Size, &img, &pts, &duration, &flags, &option_name, &option_value);
    }

    // Fuzz aom_codec_set_option
    if (option_name && option_value) {
        aom_codec_set_option(&ctx, option_name, option_value);
        free(option_name);
        free(option_value);
    }

    // Fuzz aom_codec_encode
    aom_codec_encode(&ctx, img.planes[0] ? &img : NULL, pts, duration, flags);

    // Retrieve error details
    const char *err_str = aom_codec_error(&ctx);
    (void)err_str;

    const char *err_detail = aom_codec_error_detail(&ctx);
    (void)err_detail;

    // Exercise error string conversion
    aom_codec_err_t err_code = ctx.err;
    const char *err_msg = aom_codec_err_to_string(err_code);
    (void)err_msg;

    // Flush the encoder
    while (aom_codec_encode(&ctx, NULL, 0, 0, 0) == AOM_CODEC_OK) {
        aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Consume packet
            (void)pkt;
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    // Free image planes
    if (img.planes[0]) free(img.planes[0]);
    if (img.planes[1]) free(img.planes[1]);
    if (img.planes[2]) free(img.planes[2]);

    return 0;
}