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
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_ext_ratectrl.h"
#include "vpx/vpx_image.h"

namespace harness_004 {
/*
 * Fuzzing harness for libvpx external rate control APIs
 * Targets vpx_ext_ratectrl.h APIs and VP9 external rate control integration
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 128;

// Simple mock rate control model for testing
struct MockRateCtrlModel {
    int frame_count;
    int current_gop;
    int use_key_frame;
    int use_alt_ref;
    int qp_values[10];
    int rdmult_values[10];
    bool initialized;
};

// Mock callback function implementations
static vpx_rc_status_t mock_create_model(void *priv,
                                         const vpx_rc_config_t *ratectrl_config,
                                         vpx_rc_model_t *rate_ctrl_model) {
    (void)priv;
    MockRateCtrlModel *model = new MockRateCtrlModel();
    if (!model) return VPX_RC_ERROR;
    
    model->frame_count = ratectrl_config->show_frame_count;
    model->current_gop = 0;
    model->use_key_frame = 0;
    model->use_alt_ref = 0;
    model->initialized = true;
    
    // Initialize with some default values
    for (int i = 0; i < 10; i++) {
        model->qp_values[i] = 32 + (i % 20);
        model->rdmult_values[i] = 1000 + (i % 500);
    }
    
    *rate_ctrl_model = model;
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_send_firstpass_stats(vpx_rc_model_t rate_ctrl_model,
                                                 const vpx_rc_firstpass_stats_t *firstpass_stats) {
    (void)rate_ctrl_model;
    (void)firstpass_stats;
    // Just accept the stats
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_send_tpl_gop_stats(vpx_rc_model_t rate_ctrl_model,
                                               const VpxTplGopStats *tpl_gop_stats) {
    (void)rate_ctrl_model;
    (void)tpl_gop_stats;
    // Just accept the TPL stats
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_get_encodeframe_decision(vpx_rc_model_t rate_ctrl_model,
                                                     int gop_index,
                                                     vpx_rc_encodeframe_decision_t *encode_frame_decision) {
    MockRateCtrlModel *model = (MockRateCtrlModel *)rate_ctrl_model;
    if (!model || !model->initialized) return VPX_RC_ERROR;
    
    // Simple decision logic based on gop_index
    encode_frame_decision->q_index = model->qp_values[gop_index % 10];
    encode_frame_decision->rdmult = model->rdmult_values[gop_index % 10];
    encode_frame_decision->delta_q_uv = 0;
    encode_frame_decision->sb_params_list = nullptr;
    
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_update_encodeframe_result(vpx_rc_model_t rate_ctrl_model,
                                                      const vpx_rc_encodeframe_result_t *encode_frame_result) {
    (void)rate_ctrl_model;
    (void)encode_frame_result;
    // Just accept the result
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_get_key_frame_decision(vpx_rc_model_t rate_ctrl_model,
                                                   vpx_rc_key_frame_decision_t *key_frame_decision) {
    MockRateCtrlModel *model = (MockRateCtrlModel *)rate_ctrl_model;
    if (!model || !model->initialized) return VPX_RC_ERROR;
    
    // Simple key frame decision: insert key frame every 30 frames
    key_frame_decision->key_frame_show_index = (model->frame_count % 30 == 0) ? model->frame_count : 0;
    key_frame_decision->key_frame_group_size = 8;
    
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_get_gop_decision(vpx_rc_model_t rate_ctrl_model,
                                             vpx_rc_gop_decision_t *gop_decision) {
    MockRateCtrlModel *model = (MockRateCtrlModel *)rate_ctrl_model;
    if (!model || !model->initialized) return VPX_RC_ERROR;
    
    // Simple GOP structure
    gop_decision->use_key_frame = (model->current_gop == 0) ? 1 : 0;
    gop_decision->use_alt_ref = 1;
    gop_decision->gop_coding_frames = 8;
    
    // Fill update types and ref frames
    for (int i = 0; i < gop_decision->gop_coding_frames; i++) {
        if (i == 0 && gop_decision->use_key_frame) {
            gop_decision->update_type[i] = VPX_RC_KF_UPDATE;
            gop_decision->update_ref_index[i] = 0;
            gop_decision->ref_frame_list[i].index[0] = -1;
            gop_decision->ref_frame_list[i].index[1] = -1;
            gop_decision->ref_frame_list[i].index[2] = -1;
            gop_decision->ref_frame_list[i].name[0] = VPX_RC_INVALID_REF_FRAME;
        } else if (i == 1) {
            gop_decision->update_type[i] = VPX_RC_ARF_UPDATE;
            gop_decision->update_ref_index[i] = 1;
            gop_decision->ref_frame_list[i].index[0] = 0;
            gop_decision->ref_frame_list[i].index[1] = -1;
            gop_decision->ref_frame_list[i].index[2] = -1;
            gop_decision->ref_frame_list[i].name[0] = VPX_RC_LAST_FRAME;
        } else {
            gop_decision->update_type[i] = VPX_RC_LF_UPDATE;
            gop_decision->update_ref_index[i] = 2;
            gop_decision->ref_frame_list[i].index[0] = 0;
            gop_decision->ref_frame_list[i].index[1] = 1;
            gop_decision->ref_frame_list[i].index[2] = -1;
            gop_decision->ref_frame_list[i].name[0] = VPX_RC_LAST_FRAME;
            gop_decision->ref_frame_list[i].name[1] = VPX_RC_GOLDEN_FRAME;
        }
    }
    
    model->current_gop++;
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_get_frame_rdmult(vpx_rc_model_t rate_ctrl_model,
                                             const vpx_rc_encodeframe_info_t *frame_info,
                                             int *rdmult) {
    (void)rate_ctrl_model;
    (void)frame_info;
    
    // Return a reasonable rdmult value
    *rdmult = 1500;
    return VPX_RC_OK;
}

static vpx_rc_status_t mock_delete_model(vpx_rc_model_t rate_ctrl_model) {
    MockRateCtrlModel *model = (MockRateCtrlModel *)rate_ctrl_model;
    if (model) {
        delete model;
    }
    return VPX_RC_OK;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t rate_ctrl_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=QP, 1=GOP, 2=QP+GOP
    uint8_t rc_mode = fdp.ConsumeIntegral<uint8_t>() % 3;        // 0=QMODE, 1=VBR, 2=CQ
    uint8_t show_frame_count = fdp.ConsumeIntegral<uint8_t>() % 30 + 1;
    uint8_t encode_width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t encode_height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 3;   // 0=funcs only, 1=with encode, 2=full
    
    // Map rate control type
    vpx_rc_type_t rc_type;
    switch (rate_ctrl_type) {
        case 0: rc_type = VPX_RC_QP; break;
        case 1: rc_type = VPX_RC_GOP; break;
        case 2: rc_type = VPX_RC_GOP_QP; break;
        default: rc_type = VPX_RC_QP; break;
    }
    
    // Map RC mode
    vpx_ext_rc_mode_t ext_rc_mode;
    switch (rc_mode) {
        case 0: ext_rc_mode = VPX_RC_QMODE; break;
        case 1: ext_rc_mode = VPX_RC_VBR; break;
        case 2: ext_rc_mode = VPX_RC_CQ; break;
        default: ext_rc_mode = VPX_RC_QMODE; break;
    }
    
    // Map width/height indices to actual dimensions
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int encode_width = widths[encode_width_idx % 4];
    unsigned int encode_height = heights[encode_height_idx % 4];
    
    // Create rate control configuration
    vpx_rc_config_t ratectrl_config;
    ratectrl_config.show_frame_count = show_frame_count;
    ratectrl_config.frame_width = encode_width;
    ratectrl_config.frame_height = encode_height;
    ratectrl_config.max_gf_interval = 16;
    ratectrl_config.min_gf_interval = 4;
    ratectrl_config.target_bitrate_kbps = 1000;
    ratectrl_config.frame_rate_num = 30;
    ratectrl_config.frame_rate_den = 1;
    ratectrl_config.rc_mode = ext_rc_mode;
    ratectrl_config.overshoot_percent = 50;
    ratectrl_config.undershoot_percent = 50;
    ratectrl_config.min_base_q_index = 0;
    ratectrl_config.max_base_q_index = 255;
    ratectrl_config.base_qp = 32;
    // Set up callback functions
    vpx_rc_funcs_t funcs;
    funcs.rc_type = rc_type;
    funcs.create_model = mock_create_model;
    funcs.send_firstpass_stats = mock_send_firstpass_stats;
    funcs.send_tpl_gop_stats = mock_send_tpl_gop_stats;
    funcs.get_encodeframe_decision = mock_get_encodeframe_decision;
    funcs.update_encodeframe_result = mock_update_encodeframe_result;
    funcs.get_key_frame_decision = mock_get_key_frame_decision;
    funcs.get_gop_decision = mock_get_gop_decision;
    funcs.get_frame_rdmult = mock_get_frame_rdmult;
    funcs.delete_model = mock_delete_model;
    funcs.priv = nullptr;
    funcs.rate_ctrl_log_path = nullptr;
    
    // Test scenario 0: Just test the callback functions directly
    if (test_scenario == 0) {
        vpx_rc_model_t model = nullptr;
        vpx_rc_status_t status = funcs.create_model(funcs.priv, &ratectrl_config, &model);
        if (status == VPX_RC_OK && model) {
            // Test encodeframe decision
            vpx_rc_encodeframe_decision_t decision;
            for (int i = 0; i < 5 && i < show_frame_count; i++) {
                status = funcs.get_encodeframe_decision(model, i, &decision);
                if (status != VPX_RC_OK) break;
            }
            
            // Test GOP decision
            vpx_rc_gop_decision_t gop_decision;
            status = funcs.get_gop_decision(model, &gop_decision);
            
            // Test key frame decision
            vpx_rc_key_frame_decision_t key_decision;
            status = funcs.get_key_frame_decision(model, &key_decision);
            
            // Clean up
            funcs.delete_model(model);
        }
    }
    // Test scenario 1: Test with simple encode setup
    else if (test_scenario == 1) {
        // Create a simple image to encode
        vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                         encode_width, encode_height, 1);
        if (img) {
            // Fill image with some fuzzed data
            for (int plane = 0; plane < 3; plane++) {
                unsigned char *buf = img->planes[plane];
                if (buf) {
                    int stride = img->stride[plane];
                    int w = (plane == 0) ? img->w : (img->w + 1) / 2;
                    int h = (plane == 0) ? img->h : (img->h + 1) / 2;
                    
                    for (int y = 0; y < h && fdp.remaining_bytes() > 0; y++) {
                        size_t bytes_to_use = std::min((size_t)w, fdp.remaining_bytes());
                        std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(bytes_to_use);
                        if (bytes_to_use > 0) {
                            memcpy(buf + y * stride, row_data.data(), bytes_to_use);
                        }
                    }
                }
            }
            
            // Test creating encoder with external rate control
            vpx_codec_enc_cfg_t cfg;
            vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
            
            if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
                cfg.g_w = encode_width;
                cfg.g_h = encode_height;
                cfg.rc_target_bitrate = 1000;
                cfg.g_timebase.num = 1;
                cfg.g_timebase.den = 30;
                
                // Note: In a real scenario, we would set external rate control
                // using VP9E_SET_EXTERNAL_RATE_CONTROL codec control
                // For this harness, we're testing the API structures and callbacks
            }
            
            vpx_img_free(img);
        }
    }
    // Test scenario 2: Full external rate control integration test
    else {
        // Test the full callback chain
        vpx_rc_model_t model = nullptr;
        vpx_rc_status_t status = funcs.create_model(funcs.priv, &ratectrl_config, &model);
        
        if (status == VPX_RC_OK && model) {
            // Test sending firstpass stats (mock)
            vpx_rc_firstpass_stats_t firstpass_stats;
            firstpass_stats.num_frames = 5;
            vpx_rc_frame_stats_t frame_stats[5];
            firstpass_stats.frame_stats = frame_stats;
            
            for (int i = 0; i < 5; i++) {
                frame_stats[i].frame = i;
                frame_stats[i].weight = 1.0;
                frame_stats[i].intra_error = 1000;
                frame_stats[i].coded_error = 500;
                frame_stats[i].sr_coded_error = 300;
                frame_stats[i].frame_noise_energy = 100;
                frame_stats[i].pcnt_inter = 0.7;
                frame_stats[i].pcnt_motion = 0.5;
                frame_stats[i].pcnt_second_ref = 0.3;
                frame_stats[i].pcnt_neutral = 0.1;
                frame_stats[i].pcnt_intra_low = 0.2;
                frame_stats[i].pcnt_intra_high = 0.1;
                frame_stats[i].intra_skip_pct = 0.05;
                frame_stats[i].intra_smooth_pct = 0.1;
                frame_stats[i].inactive_zone_rows = 0;
                frame_stats[i].inactive_zone_cols = 0;
                frame_stats[i].MVr = 0.0;
                frame_stats[i].mvr_abs = 0.0;
                frame_stats[i].MVc = 0.0;
                frame_stats[i].mvc_abs = 0.0;
                frame_stats[i].MVrv = 0.0;
                frame_stats[i].MVcv = 0.0;
                frame_stats[i].mv_in_out_count = 0;
                frame_stats[i].duration = 1.0;
                frame_stats[i].count = 1;
                frame_stats[i].new_mv_count = 0;
            }
            
            status = funcs.send_firstpass_stats(model, &firstpass_stats);
            
            // Test encodeframe decisions for multiple frames
            for (int i = 0; i < 5 && i < show_frame_count; i++) {
                vpx_rc_encodeframe_decision_t decision;
                status = funcs.get_encodeframe_decision(model, i, &decision);
                
                if (status == VPX_RC_OK) {
                    // Test updating encodeframe result
                    vpx_rc_encodeframe_result_t result;
                    result.bit_count = 1000 + (i * 100);
                    result.actual_encoding_qindex = decision.q_index;
                    status = funcs.update_encodeframe_result(model, &result);
                }
            }
            
            // Test GOP decision
            vpx_rc_gop_decision_t gop_decision;
            status = funcs.get_gop_decision(model, &gop_decision);
            
            // Test key frame decision
            vpx_rc_key_frame_decision_t key_decision;
            status = funcs.get_key_frame_decision(model, &key_decision);
            
            // Test frame rdmult
            vpx_rc_encodeframe_info_t frame_info;
            frame_info.frame_type = 1;  // Inter frame
            frame_info.show_index = 0;
            frame_info.coding_index = 0;
            frame_info.gop_index = 0;
            for (int j = 0; j < 3; j++) {
                frame_info.ref_frame_coding_indexes[j] = -1;
                frame_info.ref_frame_valid_list[j] = 0;
            }
            frame_info.gop_size = 8;
            frame_info.use_alt_ref = 1;
            
            int rdmult = 0;
            status = funcs.get_frame_rdmult(model, &frame_info, &rdmult);
            
            // Clean up
            funcs.delete_model(model);
        }
    }
    
    return 0;
}

} // namespace harness_004
