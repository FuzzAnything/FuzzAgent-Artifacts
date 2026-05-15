#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_frame_buffer.h"

namespace harness_003 {
/*
 * Fuzzing harness for libvpx decoder advanced features
 * Target APIs: vpx_codec_set_frame_buffer_functions, vpx_codec_register_put_frame_cb,
 *              vpx_codec_register_put_slice_cb, vpx_codec_dec_init, vpx_codec_decode,
 *              vpx_codec_get_frame, vpx_codec_destroy
 * Focus: Decoder frame buffer management, callback registration APIs, and advanced decoder features
 *        Specifically targets external frame buffer callbacks and decoder callbacks for VP9
 * Note: External frame buffers only work with VP9 according to libvpx documentation
 */



// IVF header constants
#define IVF_FRAME_HDR_SZ (4 + 8)  // 4 byte size + 8 byte timestamp
#define IVF_FILE_HDR_SZ 32

// Simple mem_get_le32 implementation (from vpx_ports/mem_ops.h)
static inline uint32_t mem_get_le32(const void *vmem) {
    const uint8_t *mem = (const uint8_t *)vmem;
    return ((uint32_t)mem[3] << 24) | (mem[2] << 16) | (mem[1] << 8) | mem[0];
}

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
    
    // Test function that returns error (for error path testing)
    int GetErrorFrameBuffer(size_t min_size, vpx_codec_frame_buffer_t *fb) {
        (void)min_size;
        (void)fb;
        return -1;  // Always fail
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
    // Minimum size check: need enough for configuration and basic data
    const size_t MIN_SIZE = 128;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    // Thread count: 1-8 range (reasonable for fuzzing)
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    
    // Decoder flags - test various combinations
    vpx_codec_flags_t flags = 0;
    bool use_error_concealment = fdp.ConsumeBool();
    bool use_frame_threading = fdp.ConsumeBool();
    bool use_external_buffers = fdp.ConsumeBool();  // Test external frame buffers
    
    if (use_error_concealment) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    if (use_frame_threading) {
        flags |= VPX_CODEC_USE_FRAME_THREADING;
    }
    
    // Decoder configuration
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    
    // For external frame buffer testing, we need to use VP9
    // According to documentation, external frame buffers only work with VP9
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        return 0;
    }
    
    // Initialize decoder context
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_dec_init(&codec, decoder_iface, &cfg, flags);
    
    // Handle initialization failures
    if (err == VPX_CODEC_INCAPABLE) {
        // Try without problematic flags
        flags &= ~(VPX_CODEC_USE_ERROR_CONCEALMENT | VPX_CODEC_USE_FRAME_THREADING);
        if (vpx_codec_dec_init(&codec, decoder_iface, &cfg, flags) != VPX_CODEC_OK) {
            return 0;
        }
    } else if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test callback registration APIs
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
    
    // External frame buffer list - must have same lifetime as decoder context
    // Declared here to ensure it's available for callbacks throughout the function
    std::unique_ptr<ExternalFrameBufferList> fb_list;
    
    // Test external frame buffer functions
    if (use_external_buffers) {
        // Create external frame buffer list on heap to extend lifetime
        fb_list.reset(new ExternalFrameBufferList());
        
        // Determine number of buffers (minimum required for VP9)
        // According to documentation: VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS
        int num_buffers = fdp.ConsumeIntegralInRange<int>(
            VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS,
            VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS + 4);
        
        if (fb_list->CreateBufferList(num_buffers)) {
            // Test both normal and error callback functions
            bool use_error_callbacks = fdp.ConsumeBool();
            
            if (use_error_callbacks) {
                // Test error paths by using error callback functions
                vpx_codec_set_frame_buffer_functions(
                    &codec,
                    [](void *priv, size_t min_size, vpx_codec_frame_buffer_t *fb) -> int {
                        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
                        return list ? list->GetErrorFrameBuffer(min_size, fb) : -1;
                    },
                    [](void *priv, vpx_codec_frame_buffer_t *fb) -> int {
                        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
                        return list ? list->ReleaseFrameBuffer(fb) : -1;
                    },
                    fb_list.get());
            } else {
                // Test normal callback functions
                vpx_codec_set_frame_buffer_functions(
                    &codec,
                    [](void *priv, size_t min_size, vpx_codec_frame_buffer_t *fb) -> int {
                        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
                        return list ? list->GetFreeFrameBuffer(min_size, fb) : -1;
                    },
                    [](void *priv, vpx_codec_frame_buffer_t *fb) -> int {
                        ExternalFrameBufferList *list = static_cast<ExternalFrameBufferList*>(priv);
                        return list ? list->ReleaseFrameBuffer(fb) : -1;
                    },
                    fb_list.get());
            }
        }
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
    const int MAX_FRAMES = 10;  // Limit frames to prevent infinite loops
    
    while (frame_size > IVF_FRAME_HDR_SZ + 16 && frame_cnt < MAX_FRAMES) {
        // Simulate IVF frame header: 4 bytes for frame size
        uint32_t frame_data_size = mem_get_le32(frame_data);
        
        // Validate frame size
        if (frame_data_size == 0 || frame_data_size > frame_size - IVF_FRAME_HDR_SZ) {
            break;
        }
        
        // Advance past frame header
        const uint8_t* compressed_frame = frame_data + IVF_FRAME_HDR_SZ;
        
        // Decode the frame
        vpx_codec_decode(&codec, compressed_frame, frame_data_size, nullptr, 0);
        
        // Try to get decoded frame (ignore errors)
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
            // Frame received - could be from callback or direct retrieval
            (void)img;  // Not used in fuzzing
        }
        
        // Advance to next frame
        size_t frame_total_size = IVF_FRAME_HDR_SZ + frame_data_size;
        frame_data += frame_total_size;
        frame_size -= frame_total_size;
        frame_cnt++;
    }
    
    // Test stream info APIs
    vpx_codec_stream_info_t si;
    si.sz = sizeof(vpx_codec_stream_info_t);
    
    if (remaining_data.size() > IVF_FILE_HDR_SZ) {
        // Test peek stream info with first frame data
        const uint8_t* test_data = remaining_data.data() + IVF_FILE_HDR_SZ;
        size_t test_size = std::min<size_t>(remaining_data.size() - IVF_FILE_HDR_SZ, 1024);
        
        vpx_codec_peek_stream_info(decoder_iface, test_data, test_size, &si);
        
        // Also test getting stream info from decoder
        vpx_codec_get_stream_info(&codec, &si);
    }
    
    // Clean up - fb_list will be automatically destroyed after decoder
    // because unique_ptr ensures it stays alive as long as needed
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_003
