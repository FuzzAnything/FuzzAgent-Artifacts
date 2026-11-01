// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int get_ctrl_id_from_data(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    uint32_t val = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
    
    // Map to known control IDs if possible, otherwise return raw value
    // We will try to use valid ones for better coverage
    switch (val % 10) {
        case 0: return VP9E_SET_DISABLE_LOOPFILTER;
        case 1: return VP9E_SET_TILE_COLUMNS;
        case 2: return VP9E_SET_TILE_ROWS;
        case 3: return VP9E_SET_SVC;
        case 4: return VP8D_SET_DECRYPTOR;
        default: return VP9E_SET_DISABLE_LOOPFILTER;
    }
}

static int get_ctrl_arg_from_data(const uint8_t *data, size_t size, int ctrl_id) {
    if (size < 4) return 0;
    uint32_t val = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
    
    // For some controls, we need specific types of arguments
    // VP9E_SET_DISABLE_LOOPFILTER expects an int (0 or 1)
    if (ctrl_id == VP9E_SET_DISABLE_LOOPFILTER) {
        return val % 2;
    }
    // VP9E_SET_TILE_COLUMNS expects int
    if (ctrl_id == VP9E_SET_TILE_COLUMNS) {
        return val % 64; // Reasonable max
    }
    // VP9E_SET_TILE_ROWS expects int
    if (ctrl_id == VP9E_SET_TILE_ROWS) {
        return val % 32; // Reasonable max
    }
    // VP9E_SET_SVC expects int (0 or 1)
    if (ctrl_id == VP9E_SET_SVC) {
        return val % 2;
    }
    // VP8D_SET_DECRYPTOR expects vpx_decrypt_init_t pointer
    // We handle this specially in the main function
    return val;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    vpx_codec_ctx_t ctx = {0};
    vpx_codec_err_t res;
    int ctrl_id;
    
    // Initialize context minimally
    ctx.name = "fuzz_test";
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;
    
    ctrl_id = get_ctrl_id_from_data(Data, Size);
    
    // Special handling for VP8D_SET_DECRYPTOR which needs a struct pointer
    if (ctrl_id == VP8D_SET_DECRYPTOR) {
        vpx_decrypt_init decrypt_init = {0};
        decrypt_init.decrypt_cb = NULL;
        decrypt_init.decrypt_state = NULL;
        
        res = vpx_codec_control_(&ctx, ctrl_id, &decrypt_init);
        (void)res; // Suppress unused warning
        return 0;
    }
    
    // For other controls, pass integer arguments
    int arg_val = get_ctrl_arg_from_data(Data, Size, ctrl_id);
    
    res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
    (void)res; // Suppress unused warning
    
    return 0;
}