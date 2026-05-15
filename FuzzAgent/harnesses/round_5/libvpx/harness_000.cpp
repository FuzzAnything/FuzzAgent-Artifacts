#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

namespace harness_000 {
/*
 * Fuzzing harness for libvpx video decoder
 * Target APIs: vpx_codec_dec_init, vpx_codec_decode, vpx_codec_get_frame,
 *              vpx_codec_peek_stream_info, vpx_codec_destroy
 */



// IVF header constants
#define IVF_FRAME_HDR_SZ (4 + 8)  // 4 byte size + 8 byte timestamp
#define IVF_FILE_HDR_SZ 32

// Simple mem_get_le32 implementation (from vpx_ports/mem_ops.h)
static inline uint32_t mem_get_le32(const void *vmem) {
    const uint8_t *mem = (const uint8_t *)vmem;
    return ((uint32_t)mem[3] << 24) | (mem[2] << 16) | (mem[1] << 8) | mem[0];
}

// Interface macros
#define VPXD_INTERFACE(name) VPXD_INTERFACE_(name)
#define VPXD_INTERFACE_(name) vpx_codec_##name##_dx()
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for IVF header and at least some data
    if (size < IVF_FILE_HDR_SZ + 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    // Thread count: 1-64 range
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
    
    // Flags for decoder initialization
    vpx_codec_flags_t flags = 0;
    bool use_postproc = fdp.ConsumeBool();
    bool use_error_concealment = fdp.ConsumeBool();
    bool use_frame_threading = fdp.ConsumeBool();
    
    if (use_postproc) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    if (use_error_concealment) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    if (use_frame_threading) {
        flags |= VPX_CODEC_USE_FRAME_THREADING;
    }
    
    // Decoder configuration
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    
    // Choose which decoder interface to use (VP8 or VP9)
    bool use_vp9 = fdp.ConsumeBool();
    vpx_codec_iface_t* decoder_iface = nullptr;
    
    if (use_vp9) {
        decoder_iface = vpx_codec_vp9_dx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
    }
    
    // Initialize decoder context
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_dec_init(&codec, decoder_iface, &cfg, flags);
    
    // Handle initialization failures
    if (err == VPX_CODEC_INCAPABLE) {
        // Try without postproc if library is configured without it
        flags &= ~VPX_CODEC_USE_POSTPROC;
        if (vpx_codec_dec_init(&codec, decoder_iface, &cfg, flags) != VPX_CODEC_OK) {
            return 0;
        }
    } else if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Get remaining data for frame processing
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    const uint8_t* frame_data = remaining_data.data();
    size_t frame_size = remaining_data.size();
    
    // Skip IVF file header if present (simulate IVF format)
    if (frame_size > IVF_FILE_HDR_SZ) {
        frame_data += IVF_FILE_HDR_SZ;
        frame_size -= IVF_FILE_HDR_SZ;
    }
    
    // Process frames while we have data
    int frame_cnt = 0;
    while (frame_size > IVF_FRAME_HDR_SZ + 16) {  // Need minimum frame data
        // Simulate IVF frame header: 4 bytes for frame size
        size_t frame_payload_size = mem_get_le32(frame_data);
        
        // Skip frame header
        frame_data += IVF_FRAME_HDR_SZ;
        frame_size -= IVF_FRAME_HDR_SZ;
        
        // Clamp frame size to remaining data
        frame_payload_size = std::min(frame_size, frame_payload_size);
        
        if (frame_payload_size == 0) {
            break;
        }
        
        // Peek stream info from the compressed data
        vpx_codec_stream_info_t stream_info;
        stream_info.sz = sizeof(stream_info);
        vpx_codec_peek_stream_info(decoder_iface, frame_data, 
                                   static_cast<unsigned int>(frame_payload_size), 
                                   &stream_info);
        
        // Decode the frame
        err = vpx_codec_decode(&codec, frame_data, 
                               static_cast<unsigned int>(frame_payload_size), 
                               nullptr, 0);
        
        // Get decoded frames (multiple frames possible from one decode call)
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *img = nullptr;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
            // We could process the image here, but for fuzzing we just consume it
            frame_cnt++;
        }
        
        // Move to next frame
        frame_data += frame_payload_size;
        frame_size -= frame_payload_size;
    }
    
    // Test control API if we have frames processed
    if (frame_cnt > 0) {
        // Test some control operations based on fuzzer input
        bool test_control = fdp.ConsumeBool();
        if (test_control && use_vp9 && threads > 1) {
            // Test VP9-specific control
            int enable = fdp.ConsumeBool() ? 1 : 0;
            vpx_codec_control(&codec, VP9D_SET_LOOP_FILTER_OPT, enable);
        }
        
        // Test postproc control if enabled
        if (test_control && (flags & VPX_CODEC_USE_POSTPROC)) {
            vp8_postproc_cfg_t pp = { 0, 0, 0 };
            if (frame_cnt % 4 == 0) {
                pp.post_proc_flag = VP8_DEBLOCK | VP8_DEMACROBLOCK;
                pp.deblocking_level = 4;
            }
            vpx_codec_control(&codec, VP8_SET_POSTPROC, &pp);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_000
