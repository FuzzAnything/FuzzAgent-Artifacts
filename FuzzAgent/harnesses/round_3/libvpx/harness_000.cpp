#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

namespace harness_000 {
/*
 * Fuzzing harness for libvpx video codec library
 * Targets VP8/VP9 encode-decode pipeline
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 64;

// Helper function to allocate and fill image with fuzzed data
static int fill_image_with_fuzzed_data(vpx_image_t *img, FuzzedDataProvider *fdp) {
    if (img == nullptr || fdp == nullptr) return 0;
    
    // For simplicity, we only handle 8-bit I420 format in this harness
    if (img->bit_depth != 8 || img->fmt != VPX_IMG_FMT_I420) {
        return 0;
    }
    
    size_t total_bytes = 0;
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char *buf = img->planes[plane];
        const int stride = img->stride[plane];
        int w = (plane == 0) ? img->d_w : (img->d_w + 1) / 2;
        int h = (plane == 0) ? img->d_h : (img->d_h + 1) / 2;
        
        for (int y = 0; y < h; ++y) {
            if (fdp->remaining_bytes() < (size_t)w) {
                return total_bytes;
            }
            std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
            memcpy(buf, row_data.data(), w);
            buf += stride;
            total_bytes += w;
        }
    }
    return total_bytes;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t quality_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t thread_count = (fdp.ConsumeIntegral<uint8_t>() % 8) + 1;
    uint8_t use_postproc = fdp.ConsumeBool();
    
    // Map width/height indices to actual dimensions
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int width = widths[width_idx % 4];
    unsigned int height = heights[height_idx % 4];
    
    // Map quality mode to vpx_enc_deadline_t
    vpx_enc_deadline_t quality;
    switch (quality_mode) {
        case 0: quality = VPX_DL_REALTIME; break;
        case 1: quality = VPX_DL_GOOD_QUALITY; break;
        case 2: quality = VPX_DL_BEST_QUALITY; break;
        default: quality = VPX_DL_GOOD_QUALITY; break;
    }
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t *encoder_iface = nullptr;
    const vpx_codec_iface_t *decoder_iface = nullptr;
    
    if (codec_choice == 0) {
        // VP8
        encoder_iface = vpx_codec_vp8_cx();
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        // VP9
        encoder_iface = vpx_codec_vp9_cx();
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!encoder_iface || !decoder_iface) {
        return 0;
    }
    
    // ENCODER SETUP
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoder_cfg;
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Configure encoder with fuzzed parameters
    encoder_cfg.g_w = width;
    encoder_cfg.g_h = height;
    encoder_cfg.g_timebase.num = 1;
    encoder_cfg.g_timebase.den = 30;  // 30 fps
    encoder_cfg.rc_target_bitrate = 200;  // kbps
    encoder_cfg.g_threads = thread_count;
    
    // Initialize encoder
    vpx_codec_flags_t encoder_flags = 0;
    if (vpx_codec_enc_init(&encoder, encoder_iface, &encoder_cfg, encoder_flags) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Allocate image buffer for encoding
    vpx_image_t *raw = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!raw) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image with fuzzed data
    size_t image_data_used = fill_image_with_fuzzed_data(raw, &fdp);
    if (image_data_used == 0) {
        // Not enough data for image, clean up and exit
        vpx_img_free(raw);
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Encode a single frame
    int frame_index = 0;
    int encode_flags = 0;
    
    vpx_codec_err_t encode_result = vpx_codec_encode(&encoder, raw, frame_index, 
                                                    1, encode_flags, quality);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            const uint8_t *frame_data = static_cast<const uint8_t*>(pkt->data.frame.buf);
            size_t frame_size = pkt->data.frame.sz;
            encoded_data.insert(encoded_data.end(), frame_data, frame_data + frame_size);
        }
    }
    
    // Clean up encoder resources
    vpx_img_free(raw);
    vpx_codec_destroy(&encoder);
    
    // If we have encoded data, try to decode it
    if (encoded_data.empty()) {
        return 0;
    }
    
    // DECODER SETUP
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t decoder_cfg = {thread_count, 0, 0};
    
    vpx_codec_flags_t decoder_flags = 0;
    if (use_postproc) {
        decoder_flags |= VPX_CODEC_USE_POSTPROC;
    }
    
    // Initialize decoder
    if (vpx_codec_dec_init(&decoder, decoder_iface, &decoder_cfg, decoder_flags) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Decode the encoded frame
    vpx_codec_err_t decode_result = vpx_codec_decode(&decoder, 
                                                    encoded_data.data(), 
                                                    encoded_data.size(), 
                                                    nullptr, 0);
    
    // Try to get decoded frame
    vpx_codec_iter_t dec_iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &dec_iter);
    
    // Clean up decoder resources
    vpx_codec_destroy(&decoder);
    
    return 0;
}

} // namespace harness_000
