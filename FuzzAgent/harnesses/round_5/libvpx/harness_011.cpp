#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_frame_buffer.h"

namespace harness_011 {
/*
 * Fuzzing harness for libvpx VP8/VP9 decoder initialization and callback configuration
 * Target APIs: vpx_codec_dec_init_ver (Primary Target - 0% coverage, 16 undiscovered branches),
 *              vpx_codec_set_frame_buffer_functions (Supporting Target - 50% line coverage, 7 undiscovered branches),
 *              vpx_codec_register_put_slice_cb (Supporting Target - 50% line coverage, 6 undiscovered branches),
 *              vpx_codec_register_put_frame_cb (Supporting Target - 50% line coverage, 6 undiscovered branches),
 *              vpx_codec_vp8_dx (Required Helper: Codec interface),
 *              vpx_codec_vp9_dx (Required Helper: Codec interface),
 *              vpx_codec_destroy (Required Helper: Cleanup)
 * 
 * Focus: Testing decoder initialization lifecycle and callback configuration mechanisms
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Uses vpx_codec_dec_init (convenience macro) for basic decoder operations
 * - harness_003.cpp: Tests external frame buffers and callbacks but uses vpx_codec_dec_init macro
 * - harness_011.cpp: Uses vpx_codec_dec_init_ver directly (primary uncovered API), focuses on initialization 
 *   parameter validation and callback setup before any decoding occurs
 * 
 * Strategy: Test various initialization parameter combinations and callback registrations
 * to exercise the 16 undiscovered branches in vpx_codec_dec_init_ver
 */



// External frame buffer structure for callback management
struct ExternalFrameBuffer {
    uint8_t *data;
    size_t size;
    int in_use;
};

// Frame buffer list for managing external frame buffers
class ExternalFrameBufferList {
public:
    ExternalFrameBufferList() : num_buffers_(0), buffers_(nullptr) {}
    
    ~ExternalFrameBufferList() {
        if (buffers_) {
            for (int i = 0; i < num_buffers_; ++i) {
                if (buffers_[i].data) {
                    delete[] buffers_[i].data;
                }
            }
            delete[] buffers_;
        }
    }
    
    // Create buffer list with specified number of buffers
    bool CreateBufferList(int num_buffers) {
        if (num_buffers <= 0) return false;
        
        num_buffers_ = num_buffers;
        buffers_ = new ExternalFrameBuffer[num_buffers_];
        if (!buffers_) return false;
        
        for (int i = 0; i < num_buffers_; ++i) {
            buffers_[i].data = nullptr;
            buffers_[i].size = 0;
            buffers_[i].in_use = 0;
        }
        return true;
    }
    
    // Get a free frame buffer (callback implementation)
    int GetFreeFrameBuffer(size_t min_size, vpx_codec_frame_buffer_t *fb) {
        if (!fb) return -1;
        
        // Find a free buffer
        for (int i = 0; i < num_buffers_; ++i) {
            if (!buffers_[i].in_use) {
                // Check if we need to allocate or reallocate
                if (buffers_[i].size < min_size) {
                    if (buffers_[i].data) {
                        delete[] buffers_[i].data;
                    }
                    buffers_[i].data = new uint8_t[min_size];
                    if (!buffers_[i].data) return -1;
                    memset(buffers_[i].data, 0, min_size);
                    buffers_[i].size = min_size;
                }
                
                buffers_[i].in_use = 1;
                fb->data = buffers_[i].data;
                fb->size = buffers_[i].size;
                fb->priv = &buffers_[i];
                return 0;
            }
        }
        return -1;  // No free buffers
    }
    
    // Release a frame buffer (callback implementation)
    int ReleaseFrameBuffer(vpx_codec_frame_buffer_t *fb) {
        if (!fb || !fb->priv) return -1;
        
        ExternalFrameBuffer *buffer = static_cast<ExternalFrameBuffer*>(fb->priv);
        buffer->in_use = 0;
        return 0;
    }
    
    // Get callback function wrapper
    static int GetFrameBufferCallback(void *priv, size_t min_size, 
                                      vpx_codec_frame_buffer_t *fb) {
        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
        if (!list) return -1;
        return list->GetFreeFrameBuffer(min_size, fb);
    }
    
    // Release callback function wrapper
    static int ReleaseFrameBufferCallback(void *priv, 
                                          vpx_codec_frame_buffer_t *fb) {
        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
        if (!list) return -1;
        return list->ReleaseFrameBuffer(fb);
    }
    
private:
    int num_buffers_;
    ExternalFrameBuffer *buffers_;
};

// Callback function for put_frame
static void put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    // Callback invoked when frame is available - no action needed for fuzzing
}

// Callback function for put_slice
static void put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
    // Callback invoked when slice is available - no action needed for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for configuration parameters
    const size_t MIN_SIZE = 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    // Thread count: 0-64 range (including 0 to test edge cases)
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    
    // Decoder flags - test various combinations to exercise different code paths
    vpx_codec_flags_t flags = 0;
    bool use_postproc = fdp.ConsumeBool();
    bool use_error_concealment = fdp.ConsumeBool();
    bool use_frame_threading = fdp.ConsumeBool();
    bool use_input_fragments = fdp.ConsumeBool();
    bool test_external_buffers = fdp.ConsumeBool();
    
    if (use_postproc) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    if (use_error_concealment) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    if (use_frame_threading) {
        flags |= VPX_CODEC_USE_FRAME_THREADING;
    }
    if (use_input_fragments) {
        flags |= VPX_CODEC_USE_INPUT_FRAGMENTS;
    }
    
    // Decoder configuration - test edge cases including NULL configuration
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    vpx_codec_dec_cfg_t *cfg_ptr = &cfg;
    
    // Sometimes test with NULL configuration (allowed by API)
    bool use_null_config = fdp.ConsumeBool();
    if (use_null_config) {
        cfg_ptr = nullptr;
    }
    
    // Choose which decoder interface to use (VP8 or VP9)
    // vpx_codec_dec_init_ver should work with both
    bool use_vp9 = fdp.ConsumeBool();
    vpx_codec_iface_t* decoder_iface = nullptr;
    
    if (use_vp9) {
        decoder_iface = vpx_codec_vp9_dx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
    }
    
    if (!decoder_iface) {
        return 0;
    }
    
    // Initialize decoder context using vpx_codec_dec_init_ver (primary target)
    // This is the key difference from existing harnesses which use vpx_codec_dec_init macro
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&codec, decoder_iface, cfg_ptr, 
                                                flags, VPX_DECODER_ABI_VERSION);
    
    // Handle initialization failures - test different error handling paths
    if (err != VPX_CODEC_OK) {
        // Test reinitialization with different parameters
        // Try without problematic flags
        flags &= ~(VPX_CODEC_USE_POSTPROC | VPX_CODEC_USE_ERROR_CONCEALMENT | 
                  VPX_CODEC_USE_FRAME_THREADING | VPX_CODEC_USE_INPUT_FRAGMENTS);
        
        // Use default thread count if we were using 0
        if (threads == 0) {
            cfg.threads = 1;
        }
        
        err = vpx_codec_dec_init_ver(&codec, decoder_iface, &cfg, 
                                    flags, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;  // Can't initialize decoder
        }
    }
    
    // Test callback registration APIs (supporting targets)
    bool register_put_frame = fdp.ConsumeBool();
    bool register_put_slice = fdp.ConsumeBool();
    
    if (register_put_frame) {
        // Test put frame callback registration
        vpx_codec_register_put_frame_cb(&codec, put_frame_callback, nullptr);
        // Error is ignored - callback may not be supported by all codecs
    }
    
    if (register_put_slice) {
        // Test put slice callback registration
        vpx_codec_register_put_slice_cb(&codec, put_slice_callback, nullptr);
        // Error is ignored - callback may not be supported by all codecs
    }
    
    // Test external frame buffer functions (only for VP9)
    std::unique_ptr<ExternalFrameBufferList> fb_list;
    if (test_external_buffers && use_vp9) {
        fb_list.reset(new ExternalFrameBufferList());
        
        // Determine number of buffers according to VP9 requirements
        // VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS
        int num_buffers = fdp.ConsumeIntegralInRange<int>(
            VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS,
            VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS + 4);
        
        if (fb_list->CreateBufferList(num_buffers)) {
            // Set frame buffer functions
            vpx_codec_set_frame_buffer_functions(
                &codec,
                ExternalFrameBufferList::GetFrameBufferCallback,
                ExternalFrameBufferList::ReleaseFrameBufferCallback,
                fb_list.get()
            );
            // Error is ignored - function may fail with certain configurations
        }
    }
    
    // Clean up decoder context
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_011
