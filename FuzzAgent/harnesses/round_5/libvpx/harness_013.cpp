#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_frame_buffer.h"

namespace harness_013 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation targeting
 * complete encoder lifecycle with configuration validation to address
 * 333 blocked branches in validate_config function (highest blocker in project).
 * 
 * Target APIs: vpx_codec_enc_config_default → vpx_codec_enc_config_set → 
 *              vpx_codec_enc_init_ver/vpx_codec_enc_init_multi_ver → 
 *              vpx_codec_encode → vpx_codec_get_cx_data → vpx_codec_destroy
 * Additional target APIs: vpx_codec_set_frame_buffer_functions, 
 *              vpx_codec_control_, vpx_codec_get_global_headers, 
 *              vpx_codec_get_preview_frame
 * 
 * Focus: Complete encoder lifecycle with configuration modification and validation
 * Semantic differentiation from existing harnesses:
 * - harness_010.cpp: Focuses on specific validation path combinations and sequences
 * - harness_012.cpp: Comprehensive parameter space exploration targeting all 323 blocked branches
 * - harness_013.cpp: Complete encoder lifecycle testing with configuration modification,
 *   using vpx_codec_enc_config_set for dynamic reconfiguration, and testing advanced
 *   features like frame buffer functions, control parameters, and preview frames
 * 
 * Strategy: Test the full encoder lifecycle: default config → modify config → set config → 
 * initialize encoder → encode frames → get encoded data → destroy, with dynamic
 * reconfiguration and advanced feature testing between encoding operations.
 */



// Define constants from libvpx internal headers
#define MAX_LAG_BUFFERS 25  // From vp9/encoder/vp9_lookahead.h
#define MAX_ARF_LAYERS 6    // From vp9/encoder/vp9_firstpass.h
#define MAX_NUM_THREADS 64  // From vp9/encoder/vp9_encoder.h
#define VP9_MAXIMUM_REF_BUFFERS 8  // From vp9/encoder/vp9_encoder.h
#define VPX_MAXIMUM_WORK_BUFFERS 4 // From vp9/vp9_dx_iface.c

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
    void ReleaseFrameBuffer(void *priv, vpx_codec_frame_buffer_t *fb) {
        if (!priv || !fb) return;
        
        ExternalFrameBuffer *buffer = static_cast<ExternalFrameBuffer*>(priv);
        buffer->in_use = 0;
    }
    
private:
    int num_buffers_;
    ExternalFrameBuffer *buffers_;
};

// Callback function wrappers
static int GetFrameBufferCallback(void *user_priv, size_t min_size,
                                  vpx_codec_frame_buffer_t *fb) {
    ExternalFrameBufferList *buffer_list = 
        static_cast<ExternalFrameBufferList*>(user_priv);
    return buffer_list->GetFreeFrameBuffer(min_size, fb);
}

static int ReleaseFrameBufferCallback(void *user_priv, 
                                     vpx_codec_frame_buffer_t *fb) {
    ExternalFrameBufferList *buffer_list = 
        static_cast<ExternalFrameBufferList*>(user_priv);
    buffer_list->ReleaseFrameBuffer(fb->priv, fb);
    return 0;  // Success
}

// Mock FIRSTPASS_STATS structure for two-pass stats testing
typedef struct {
    double frame;
    double weight;
    double intra_error;
    double coded_error;
    double sr_coded_error;
    double frame_noise_energy;
    double pcnt_inter;
    double pcnt_motion;
    double pcnt_second_ref;
    double pcnt_neutral;
    double pcnt_intra_low;
    double pcnt_intra_high;
    double intra_skip_pct;
    double intra_smooth_pct;
    double inactive_zone_rows;
    double inactive_zone_cols;
    double MVr;
    double mvr_abs;
    double MVc;
    double mvc_abs;
    double MVrv;
    double MVcv;
    double mv_in_out_count;
    double duration;
    double count;
    double new_mv_count;
    int64_t spatial_layer_id;
} FIRSTPASS_STATS;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for complex configuration structures
    const size_t MIN_SIZE = 2048;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Use VP9 encoder for configuration validation testing
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    if (config_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test multiple encoder lifecycle scenarios
    unsigned int num_scenarios = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    for (unsigned int scenario_idx = 0; 
         scenario_idx < num_scenarios && fdp.remaining_bytes() > 1024; 
         scenario_idx++) {
        
        // Reset to default config for each scenario
        config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        if (config_err != VPX_CODEC_OK) {
            continue;
        }
        
        // --- Phase 1: Configuration Setup ---
        
        // Consume configuration parameters from fuzzer input
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_NUM_THREADS);
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(1, 4096);
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(1, 4096);
        cfg.g_timebase.num = fdp.ConsumeIntegralInRange<int>(1, 1000);
        cfg.g_timebase.den = fdp.ConsumeIntegralInRange<int>(1, 1000);
        
        // Test different pass modes (triggers validation paths)
        unsigned int pass_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (pass_mode) {
            case 0: cfg.g_pass = VPX_RC_ONE_PASS; break;
            case 1: cfg.g_pass = VPX_RC_FIRST_PASS; break;
            case 2: cfg.g_pass = VPX_RC_LAST_PASS; break;
        }
        
        // Rate control configuration
        unsigned int rc_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        switch (rc_mode) {
            case 0: cfg.rc_end_usage = VPX_VBR; break;
            case 1: cfg.rc_end_usage = VPX_CBR; break;
            case 2: cfg.rc_end_usage = VPX_CQ; break;
            case 3: cfg.rc_end_usage = VPX_Q; break;
        }
        
        cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(1, 10000);
        cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
        cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
        
        // Keyframe configuration
        cfg.kf_mode = fdp.ConsumeBool() ? VPX_KF_AUTO : VPX_KF_FIXED;
        cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
        cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
        
        // --- Phase 2: Encoder Initialization ---
        
        vpx_codec_ctx_t codec;
        vpx_codec_err_t init_result;
        
        // Choose initialization method
        bool use_multi_init = fdp.ConsumeBool();
        vpx_codec_flags_t flags = 0;
        
        // Set flags based on fuzzer input
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_PSNR;
        }
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_HIGHBITDEPTH;
        }
        
        if (use_multi_init) {
            // Test vpx_codec_enc_init_multi_ver
            unsigned int num_encoders = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
            vpx_codec_ctx_t *encoders = new vpx_codec_ctx_t[num_encoders];
            
            if (encoders) {
                // Create down-sampling factor array
                vpx_rational_t dsf[num_encoders];
                for (unsigned int i = 0; i < num_encoders; i++) {
                    dsf[i].num = fdp.ConsumeIntegralInRange<int>(1, 4);
                    dsf[i].den = fdp.ConsumeIntegralInRange<int>(1, 4);
                }
                
                init_result = vpx_codec_enc_init_multi_ver(encoders, encoder_iface, &cfg, 
                                                          num_encoders, flags, dsf,
                                                          VPX_ENCODER_ABI_VERSION);
                
                // Clean up if initialization succeeded
                if (init_result == VPX_CODEC_OK) {
                    for (unsigned int i = 0; i < num_encoders; i++) {
                        vpx_codec_destroy(&encoders[i]);
                    }
                }
                
                delete[] encoders;
            }
        } else {
            // Test vpx_codec_enc_init_ver
            init_result = vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, 
                                                VPX_ENCODER_ABI_VERSION);
            
            if (init_result == VPX_CODEC_OK) {
                // --- Phase 3: Dynamic Reconfiguration ---
                
                // Test vpx_codec_enc_config_set with modified configuration
                vpx_codec_enc_cfg_t modified_cfg = cfg;
                modified_cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(1, 20000);
                modified_cfg.g_pass = fdp.ConsumeBool() ? VPX_RC_ONE_PASS : VPX_RC_FIRST_PASS;
                
                vpx_codec_err_t set_result = vpx_codec_enc_config_set(&codec, &modified_cfg);
                // Note: set_result may fail for invalid configurations - that's part of testing
                
                // --- Phase 4: Advanced Feature Testing ---
                
                // Test vpx_codec_set_frame_buffer_functions
                ExternalFrameBufferList buffer_list;
                int num_buffers = VP9_MAXIMUM_REF_BUFFERS + VPX_MAXIMUM_WORK_BUFFERS;
                if (buffer_list.CreateBufferList(num_buffers)) {
                    vpx_codec_err_t fb_result = vpx_codec_set_frame_buffer_functions(
                        &codec, GetFrameBufferCallback, ReleaseFrameBufferCallback, &buffer_list);
                    // Note: This may fail if encoder doesn't support external frame buffers
                }
                
                // Test vpx_codec_control_ with various control parameters
                if (fdp.ConsumeBool()) {
                    int ctrl_value = fdp.ConsumeIntegral<int>();
                    vpx_codec_err_t ctrl_result = vpx_codec_control(&codec, VP8E_SET_CPUUSED, ctrl_value);
                }
                
                if (fdp.ConsumeBool()) {
                    int ctrl_value = fdp.ConsumeIntegral<int>();
                    vpx_codec_err_t ctrl_result = vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, ctrl_value);
                }
                
                if (fdp.ConsumeBool()) {
                    int ctrl_value = fdp.ConsumeIntegral<int>();
                    vpx_codec_err_t ctrl_result = vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, ctrl_value);
                }
                
                if (fdp.ConsumeBool()) {
                    int ctrl_value = fdp.ConsumeIntegral<int>();
                    vpx_codec_err_t ctrl_result = vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, ctrl_value);
                }
                
                // Test vpx_codec_get_global_headers
                vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
                // Note: global_headers may be NULL if not supported - that's expected
                
                // --- Phase 5: Encoding Test ---
                
                // Create a simple test image
                vpx_image_t* img = nullptr;
                if (fdp.remaining_bytes() > 100) {
                    vpx_img_fmt_t img_fmt;
                    
                    // Choose image format based on whether high bit depth flag is set
                    if (flags & VPX_CODEC_USE_HIGHBITDEPTH) {
                        // Use 16-bit formats when high bit depth flag is set
                        unsigned int fmt = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
                        switch (fmt) {
                            case 0: img_fmt = VPX_IMG_FMT_I42016; break;
                            case 1: img_fmt = VPX_IMG_FMT_I42216; break;
                            case 2: img_fmt = VPX_IMG_FMT_I44416; break;
                            case 3: img_fmt = VPX_IMG_FMT_I44016; break;
                            default: img_fmt = VPX_IMG_FMT_I42016; break;
                        }
                    } else {
                        // Use 8-bit formats when high bit depth flag is not set
                        unsigned int fmt = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
                        switch (fmt) {
                            case 0: img_fmt = VPX_IMG_FMT_I420; break;
                            case 1: img_fmt = VPX_IMG_FMT_I422; break;
                            case 2: img_fmt = VPX_IMG_FMT_I444; break;
                            case 3: img_fmt = VPX_IMG_FMT_I440; break;
                            default: img_fmt = VPX_IMG_FMT_I420; break;
                        }
                    }
                    
                    unsigned int align = 1 << fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
                    img = vpx_img_alloc(nullptr, img_fmt, cfg.g_w, cfg.g_h, align);
                    if (img && fdp.remaining_bytes() > 100) {
                        // Fill image with fuzzer data
                        size_t fill_size = fdp.ConsumeIntegralInRange<size_t>(0, 
                            std::min((size_t)cfg.g_w * cfg.g_h * 3, fdp.remaining_bytes()));
                        std::vector<uint8_t> fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
                        
                        if (fill_size > 0) {
                            // Handle fill differently based on bit depth
                            if (flags & VPX_CODEC_USE_HIGHBITDEPTH) {
                                // For 16-bit formats, fill with 16-bit data
                                if (img->img_data && img->planes[0]) {
                                    size_t plane_size = img->stride[0] * img->h / 2; // 16-bit elements
                                    size_t fill_limit = std::min(fill_size / 2, plane_size);
                                    uint16_t* plane_ptr = reinterpret_cast<uint16_t*>(img->planes[0]);
                                    for (size_t i = 0; i < fill_limit; i++) {
                                        uint16_t value = (fill_data[i*2] << 8) | fill_data[i*2 + 1];
                                        plane_ptr[i] = value;
                                    }
                                }
                            } else {
                                // For 8-bit formats, fill with 8-bit data
                                if (img->img_data && img->planes[0]) {
                                    size_t plane_size = img->stride[0] * img->h;
                                    size_t fill_limit = std::min(fill_size, plane_size);
                                    for (size_t i = 0; i < fill_limit; i++) {
                                        img->planes[0][i] = fill_data[i];
                                    }
                                }
                            }
                        }
                        // Test encoding
                        vpx_codec_err_t encode_result = vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        if (encode_result == VPX_CODEC_OK) {
                            // Get encoded data
                            const vpx_codec_cx_pkt_t *pkt = nullptr;
                            vpx_codec_iter_t iter = nullptr;
                            
                            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                                // Process packets - just consume them for coverage
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Frame packet
                                } else if (pkt->kind == VPX_CODEC_STATS_PKT) {
                                    // Stats packet
                                }
                            }
                            
                            // Test vpx_codec_get_preview_frame
                            const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec);
                            // Note: preview_frame may be NULL if not available
                        }
                        
                        // Test flush encoding (end-of-stream)
                        encode_result = vpx_codec_encode(&codec, nullptr, 0, 1, 0, VPX_DL_REALTIME);
                        
                        if (encode_result == VPX_CODEC_OK) {
                            // Get any remaining data
                            const vpx_codec_cx_pkt_t *pkt = nullptr;
                            vpx_codec_iter_t iter = nullptr;
                            
                            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                                // Just consume packets
                            }
                        }
                    }
                    
                    if (img) {
                        vpx_img_free(img);
                    }
                }
                
                // --- Phase 6: Cleanup ---
                vpx_codec_destroy(&codec);
            }
        }
        
        // Consume remaining bytes for this scenario to advance fuzzer state
        if (fdp.remaining_bytes() > 100) {
            fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 100));
        }
    }
    
    return 0;
}

} // namespace harness_013
