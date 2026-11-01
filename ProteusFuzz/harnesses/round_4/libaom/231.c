// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

// Define control IDs if not present in headers, based on common libaom AV1 decoder controls
#ifndef AOM_CODEC_CTRL_ID_AV1D_GET_DISPLAY_SIZE
#define AOM_CODEC_CTRL_ID_AV1D_GET_DISPLAY_SIZE 1001
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1D_SET_OUTPUT_ALL_LAYERS
#define AOM_CODEC_CTRL_ID_AV1D_SET_OUTPUT_ALL_LAYERS 1002
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1D_SET_SKIP_FILM_GRAIN
#define AOM_CODEC_CTRL_ID_AV1D_SET_SKIP_FILM_GRAIN 1003
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_HEADER_INFO
#define AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_HEADER_INFO 1004
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1D_SET_OPERATING_POINT
#define AOM_CODEC_CTRL_ID_AV1D_SET_OPERATING_POINT 1005
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_SIZE
#define AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_SIZE 1006
#endif

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (ctx == NULL) return 0;
    // Invoke the control function
    aom_codec_control(ctx, control_id, arg);
    return 0;
}

int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize the codec context with AV1 decoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Allocate temporary buffers for arguments
    int8_t arg_int8 = 0;
    int32_t arg_int32 = 0;
    struct { int width; int height; } display_size;
    struct { int width; int height; } frame_size;
    
    // Initialize structures
    memset(&display_size, 0, sizeof(display_size));
    memset(&frame_size, 0, sizeof(frame_size));

    // Use the first byte to select the control ID
    int control_id = Data[0] % 6;

    switch (control_id) {
        case 0: // aom_codec_control_typechecked_AV1D_GET_DISPLAY_SIZE
            fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_GET_DISPLAY_SIZE, &display_size);
            break;
        case 1: // aom_codec_control_typechecked_AV1D_SET_OUTPUT_ALL_LAYERS
            if (Size > 1) {
                arg_int8 = Data[1] % 2;
                fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_SET_OUTPUT_ALL_LAYERS, &arg_int8);
            }
            break;
        case 2: // aom_codec_control_typechecked_AV1D_SET_SKIP_FILM_GRAIN
            if (Size > 1) {
                arg_int8 = (Data[1] % 2) ? 1 : 0;
                fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_SET_SKIP_FILM_GRAIN, &arg_int8);
            }
            break;
        case 3: // aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO
            {
                struct {
                    unsigned int show_frame;
                    unsigned int frame_type;
                    unsigned int frame_width;
                    unsigned int frame_height;
                } header_info;
                memset(&header_info, 0, sizeof(header_info));
                fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_HEADER_INFO, &header_info);
            }
            break;
        case 4: // aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
            if (Size > 1) {
                arg_int32 = Data[1] % 32; // AV1 has up to 32 operating points
                fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_SET_OPERATING_POINT, &arg_int32);
            }
            break;
        case 5: // aom_codec_control_typechecked_AV1D_GET_FRAME_SIZE
            fuzz_codec_control(&ctx, AOM_CODEC_CTRL_ID_AV1D_GET_FRAME_SIZE, &frame_size);
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}