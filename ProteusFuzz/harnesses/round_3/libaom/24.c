// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

static int parse_fuzz_data(const uint8_t *data, size_t size, int *cmd, const uint8_t **arg, size_t *arg_size) {
    if (size < 4) return 0;

    int cmd_idx = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    cmd_idx %= 6; 

    *cmd = cmd_idx;
    *arg_size = 0;

    if (size > 4) {
        *arg = data + 4;
        *arg_size = size - 4;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    int cmd;
    const uint8_t *arg;
    size_t arg_size;

    if (!iface) return 0;

    /* Initialize default config */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* Removed intra_only and error_resilient as they are not members of aom_codec_dec_cfg_t */

    /* Initialize the codec context */
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    /* Parse fuzz input to determine which command to run */
    if (!parse_fuzz_data(Data, Size, &cmd, &arg, &arg_size)) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    switch (cmd) {
        case 0:
            /* AV1D_GET_FRAME_SIZE */
            {
                aom_image_t img;
                memset(&img, 0, sizeof(img));
                res = aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, &img);
                (void)res;
            }
            break;

        case 1:
            /* AV1D_GET_DISPLAY_SIZE */
            {
                aom_image_t img;
                memset(&img, 0, sizeof(img));
                res = aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, &img);
                (void)res;
            }
            break;

        case 2:
            /* AV1D_SET_OUTPUT_ALL_LAYERS */
            {
                int val = 0;
                if (arg_size >= sizeof(int)) {
                    memcpy(&val, arg, sizeof(int));
                }
                res = aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, &val);
                (void)res;
            }
            break;

        case 3:
            /* AV1D_GET_IMG_FORMAT */
            {
                aom_img_fmt_t fmt = AOM_IMG_FMT_NONE;
                res = aom_codec_control(&ctx, AV1D_GET_IMG_FORMAT, &fmt);
                (void)res;
            }
            break;

        case 4:
            /* AV1D_SET_SKIP_FILM_GRAIN */
            {
                int val = 0;
                if (arg_size >= sizeof(int)) {
                    memcpy(&val, arg, sizeof(int));
                }
                res = aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, &val);
                (void)res;
            }
            break;

        case 5:
            /* AV1D_SET_OPERATING_POINT */
            {
                int val = 0;
                if (arg_size >= sizeof(int)) {
                    memcpy(&val, arg, sizeof(int));
                }
                res = aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, &val);
                (void)res;
            }
            break;

        default:
            break;
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}