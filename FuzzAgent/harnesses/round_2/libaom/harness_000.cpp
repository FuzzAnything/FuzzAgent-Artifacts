#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

namespace harness_000 {
/*
 * Fuzzing harness for libaom AV1 codec library
 * Targets both decoder and encoder APIs for maximum coverage
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Not enough data for meaningful fuzzing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: decode, 1: encode, 2: both
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(16, 4096);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(16, 4096);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
    uint8_t use_highbitdepth = fdp.ConsumeBool();
    uint8_t tile_mode = fdp.ConsumeBool();
    uint8_t is_annexb = fdp.ConsumeBool();
    
    // Get remaining data for codec operations
    std::vector<uint8_t> codec_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (codec_data.empty()) {
        return 0;
    }

    int ret = 0;

    // DECODER PATH
    if (operation == 0 || operation == 2) {
        aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
        if (!decoder_iface) {
            // Continue to encoder path if decoder init fails
        } else {
            aom_codec_ctx_t decoder;
            aom_codec_dec_cfg_t decoder_cfg = {static_cast<unsigned int>(threads), 0, 0,
                                              static_cast<unsigned int>(!use_highbitdepth)};
            
            if (aom_codec_dec_init(&decoder, decoder_iface, &decoder_cfg, 0) == AOM_CODEC_OK) {
                // Set decoder controls based on fuzzed parameters
                if (tile_mode) {
                    AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1_SET_TILE_MODE, 1);
                }
                if (is_annexb) {
                    AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_IS_ANNEXB, 1);
                }
                
                // Try to parse stream info
                aom_codec_stream_info_t stream_info;
                stream_info.is_annexb = is_annexb;
                aom_codec_peek_stream_info(decoder_iface, codec_data.data(),
                                          codec_data.size(), &stream_info);
                
                // Attempt to decode the data
                aom_codec_decode(&decoder, codec_data.data(), codec_data.size(), nullptr);
                
                // Try to get decoded frames
                aom_codec_iter_t iter = nullptr;
                aom_image_t* img = nullptr;
                while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
                    // Frame available, but we don't need to do anything with it for fuzzing
                }
                
                aom_codec_destroy(&decoder);
            }
        }
    }

    // ENCODER PATH  
    if (operation == 1 || operation == 2) {
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        if (!encoder_iface) {
            return ret;
        }
        
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t encoder_cfg;
        
        // Initialize encoder configuration
        if (aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0) != AOM_CODEC_OK) {
            return ret;
        }
        
        // Set basic encoder parameters from fuzzed data
        encoder_cfg.g_w = width;
        encoder_cfg.g_h = height;
        encoder_cfg.g_threads = threads;
        encoder_cfg.g_pass = AOM_RC_ONE_PASS;
        encoder_cfg.g_lag_in_frames = 0;
        encoder_cfg.g_error_resilient = 0;
        
        if (aom_codec_enc_init(&encoder, encoder_iface, &encoder_cfg, 0) == AOM_CODEC_OK) {
            // Create a simple test image for encoding
            aom_image_t img;
            aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
            
            // Fill image with some data (could be more sophisticated)
            for (int plane = 0; plane < 3; ++plane) {
                uint8_t* plane_buf = img.planes[plane];
                size_t plane_size = static_cast<size_t>(img.stride[plane]) *
                                   (plane == 0 ? height : height / 2);
                
                for (size_t i = 0; i < plane_size && i < codec_data.size(); ++i) {
                    plane_buf[i] = codec_data[i % codec_data.size()];
                }
            }
            
            // Try to encode the image
            aom_codec_encode(&encoder, &img, 0, 1, 0);
            
            // Get encoded data
            aom_codec_iter_t iter = nullptr;
            const aom_codec_cx_pkt_t* pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                // Packet available, but we don't need to do anything with it for fuzzing
            }
            
            // Clean up
            aom_img_free(&img);
            aom_codec_destroy(&encoder);
        }
    }

    return 0;
}

} // namespace harness_000
