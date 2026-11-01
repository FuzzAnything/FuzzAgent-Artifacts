// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: aom_codec_peek_stream_info
    // This function does not require a decoder context, just an interface.
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        // is_annexb must be initialized. Try both true and false based on input.
        si.is_annexb = (Data[0] & 0x01) ? 1 : 0;
        
        // Use the fuzz data as the bitstream
        aom_codec_err_t ret = aom_codec_peek_stream_info(iface, Data, Size, &si);
        (void)ret; // Ignore return for fuzzing stability, just ensure no crash
    }

    // Test 2: aom_codec_dec_init_ver and subsequent decode/get_stream_info
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    // Use fuzz data to configure threads, width, height, etc.
    if (Size >= 4) {
        cfg.threads = Data[0] % 16 + 1;
        cfg.w = (Data[1] * 256 + Data[2]) % 4096;
        cfg.h = (Data[3] * 256 + (Size > 4 ? Data[4] : 0)) % 4096;
        cfg.allow_lowbitdepth = (Data[Size > 5 ? 5 : 0] & 0x01);
    } else {
        cfg.threads = 1;
        cfg.w = 0;
        cfg.h = 0;
        cfg.allow_lowbitdepth = 0;
    }

    aom_codec_flags_t flags = 0;
    // Randomize flags if possible
    if (Size > 6) {
        flags = (aom_codec_flags_t)(Data[6]);
    }

    int ver = AOM_DECODER_ABI_VERSION;

    // Initialize decoder
    aom_codec_err_t init_ret = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, ver);
    
    if (init_ret == AOM_CODEC_OK) {
        // Test 3: aom_codec_decode
        // Pass the fuzz data as encoded data
        aom_codec_err_t decode_ret = aom_codec_decode(&ctx, Data, Size, NULL);
        (void)decode_ret;

        // Test 4: aom_codec_get_stream_info
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        aom_codec_err_t info_ret = aom_codec_get_stream_info(&ctx, &si);
        (void)info_ret;

        // Attempt to get frames to ensure state progression
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&ctx, NULL)) != NULL) {
            // Process frame if needed, here just freeing is enough for fuzzing
            // aom_img_free(img); // Usually handled by codec or application
            // For libaom, aom_img_free is not typically called on the returned pointer 
            // unless the application took ownership, but standard usage is just to read.
            // The codec manages the frame buffer lifecycle.
            (void)img;
        }
    }

    cleanup_decoder(&ctx);

    return 0;
}