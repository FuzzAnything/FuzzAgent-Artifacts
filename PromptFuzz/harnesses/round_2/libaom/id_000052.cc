#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 165, nr_unique_branch: 117
//<Quality> {"density":15,"unique_branches":{"av1_select_sb_size":[[1023,31,1023,49,0,0,4,1]],"encoder_set_option":[[4466,14,4467,42,0,0,4,1],[4469,14,4470,48,0,0,4,1],[4472,14,4473,48,0,0,4,1],[4475,14,4476,42,0,0,4,1],[4478,14,4479,48,0,0,4,1],[4481,14,4482,42,0,0,4,1],[4484,14,4485,42,0,0,4,1],[4487,14,4488,42,0,0,4,1],[4490,14,4491,42,0,0,4,1],[4493,14,4494,42,0,0,4,1],[4496,14,4497,42,0,0,4,1],[4499,14,4501,48,0,0,4,1],[4504,14,4505,48,0,0,4,1],[4507,14,4508,48,0,0,4,1],[4510,14,4511,42,0,0,4,1],[4513,14,4514,42,0,0,4,1],[4516,14,4517,42,0,0,4,1],[4519,14,4520,42,0,0,4,1],[4522,14,4523,48,0,0,4,1],[4525,14,4526,42,0,0,4,1],[4528,14,4529,48,0,0,4,1],[4531,14,4533,35,0,0,4,1],[4536,14,4538,48,0,0,4,1],[4540,14,4542,29,0,0,4,1],[4544,14,4545,42,0,0,4,1],[4547,14,4548,48,0,0,4,1],[4550,14,4551,42,0,0,4,1],[4553,14,4554,42,0,0,4,1],[4556,14,4557,48,0,0,4,1],[4566,14,4567,42,0,0,4,1],[4569,14,4571,48,0,0,4,1],[4573,14,4574,48,0,0,4,1],[4576,14,4578,48,0,0,4,1],[4580,14,4581,48,0,0,4,1],[4583,14,4584,48,0,0,4,1],[4586,14,4588,48,0,0,4,1],[4591,14,4592,48,0,0,4,1],[4594,14,4595,42,0,0,4,1],[4597,14,4598,48,0,0,4,1],[4600,14,4601,42,0,0,4,1],[4603,14,4604,48,0,0,4,1],[4606,14,4607,48,0,0,4,1],[4609,14,4610,48,0,0,4,1],[4613,14,4614,48,0,0,4,1],[4616,14,4617,48,0,0,4,1],[4619,14,4620,48,0,0,4,1],[4622,14,4624,48,0,0,4,1],[4626,14,4628,48,0,0,4,1],[4630,14,4631,48,0,0,4,1],[4633,14,4635,48,0,0,4,1],[4637,14,4639,48,0,0,4,1],[4641,14,4642,48,0,0,4,1],[4644,14,4645,48,0,0,4,1],[4647,14,4648,48,0,0,4,1],[4650,14,4651,48,0,0,4,1],[4653,14,4654,48,0,0,4,1],[4656,14,4657,48,0,0,4,1],[4659,14,4661,48,0,0,4,1],[4663,14,4664,48,0,0,4,1],[4666,14,4667,42,0,0,4,1],[4669,14,4670,42,0,0,4,1],[4672,14,4673,42,0,0,4,1],[4675,14,4676,48,0,0,4,1],[4678,14,4680,29,0,0,4,1],[4683,14,4684,48,0,0,4,1],[4686,14,4687,48,0,0,4,1],[4689,14,4690,48,0,0,4,1],[4692,14,4694,48,0,0,4,1],[4697,14,4698,42,0,0,4,1],[4700,14,4702,48,0,0,4,1],[4705,14,4706,42,0,0,4,1],[4708,14,4709,42,0,0,4,1],[4711,14,4712,48,0,0,4,1],[4714,14,4715,48,0,0,4,1],[4717,14,4718,48,0,0,4,1],[4720,14,4721,48,0,0,4,1],[4725,12,4726,46,0,0,4,1],[4729,14,4730,48,0,0,4,1],[4732,14,4733,48,0,0,4,1],[4737,12,4738,46,0,0,4,1],[4750,14,4752,48,0,0,4,1],[4754,14,4756,48,0,0,4,1],[4758,14,4759,42,0,0,4,1],[4761,14,4762,42,0,0,4,1],[4764,14,4765,42,0,0,4,1],[4768,14,4769,42,0,0,4,1],[4772,14,4773,48,0,0,4,1],[4775,14,4776,48,0,0,4,1],[4778,14,4780,48,0,0,4,1],[4782,14,4783,42,0,0,4,1],[4785,14,4786,48,0,0,4,1],[4788,14,4789,42,0,0,4,1],[4792,14,4793,42,0,0,4,1],[4796,14,4797,48,0,0,4,1]],"set_good_speed_feature_framesize_dependent":[[740,7,740,22,0,0,4,0]],"av1_rc_update_framerate":[[35,25,35,26,4,0,4,0],[35,38,35,39,4,0,4,0]],"av1_rc_get_default_min_gf_interval":[[439,7,439,28,0,0,4,1],[35,23,35,34,3,0,4,1]],"init_seq_coding_tools":[[595,38,595,66,0,0,4,0],[597,7,597,25,0,0,4,0],[597,29,597,62,0,0,4,0],[602,7,602,38,0,0,4,1],[605,7,605,37,0,0,4,0],[611,7,611,45,0,0,4,1]],"av1_noise_estimate_init":[[45,7,45,26,0,0,4,0]],"arg_match_helper":[[45,7,45,22,0,0,4,0],[45,26,45,67,0,0,4,1]],"set_bitstream_level_tier":[[481,14,482,33,0,0,4,1],[484,14,485,33,0,0,4,0],[484,14,485,33,0,0,4,1],[487,14,488,33,0,0,4,1],[490,14,491,33,0,0,4,0]],"set_encoder_config":[[1375,45,1375,79,0,0,4,1],[1581,7,1581,28,0,0,4,0],[34,23,34,34,10,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_img_remove_metadata","aom_uleb_encode","aom_codec_enc_config_set","aom_codec_error","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_wrap","aom_img_remove_metadata","aom_uleb_encode","aom_codec_enc_config_set","aom_codec_error","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for meaningful fuzzing
    
    // Open input as FILE* for potential reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get encoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    
    // Set up encoder configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0; // Good Quality
    aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, usage);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Modify config with fuzzed data
    cfg.g_w = (data[0] % 128 + 32) * 16; // Reasonable width (512-2560)
    cfg.g_h = (data[1] % 128 + 32) * 16; // Reasonable height (512-2560)
    cfg.g_threads = 1 + (data[2] % 4);   // 1-4 threads
    cfg.rc_target_bitrate = 100 + (data[3] * 10); // 100-2760 kbps
    cfg.g_limit = 1; // Encode one frame
    
    // Create codec context
    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Initialize encoder
    err = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&codec);
        (void)error_str; // Prevent unused warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder options from fuzzed data
    char option_name[32];
    char option_value[32];
    
    // Set quality option
    snprintf(option_name, sizeof(option_name), "end-usage");
    snprintf(option_value, sizeof(option_value), "%s", 
             (data[4] & 1) ? "vbr" : "cq");
    aom_codec_set_option(&codec, option_name, option_value);
    
    // Set cpu-used option
    snprintf(option_name, sizeof(option_name), "cpu-used");
    snprintf(option_value, sizeof(option_value), "%d", data[5] % 9);
    aom_codec_set_option(&codec, option_name, option_value);
    
    // Create test image from fuzzed data
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Use subset of data for image (avoid buffer overrun)
    size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2; // I420 size
    if (img_data_size > size / 2) {
        img_data_size = size / 2;
    }
    
    unsigned char *img_data = (unsigned char *)malloc(img_data_size);
    if (!img_data) {
        aom_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image data from input
    size_t copy_size = img_data_size < (size - 10) ? img_data_size : (size - 10);
    if (copy_size > 0) {
        memcpy(img_data, data + 10, copy_size);
    }
    
    // Wrap image
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int stride_align = 32;
    aom_image_t *wrapped_img = aom_img_wrap(&img, fmt, cfg.g_w, cfg.g_h, 
                                           stride_align, img_data);
    if (!wrapped_img) {
        free(img_data);
        aom_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Remove any metadata (even though image is newly created)
    aom_img_remove_metadata(wrapped_img);
    
    // Test ULEB encoding with fuzzed data
    uint64_t uleb_value = 0;
    for (int i = 0; i < 8 && (i + 20) < size; i++) {
        uleb_value |= (uint64_t)data[i + 20] << (i * 8);
    }
    
    uint8_t uleb_buffer[16];
    size_t uleb_encoded_size = 0;
    int uleb_ret = aom_uleb_encode(uleb_value, sizeof(uleb_buffer), 
                                   uleb_buffer, &uleb_encoded_size);
    (void)uleb_ret; // Ignore return for fuzzing
    
    // Update encoder configuration with modified values
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    err = aom_codec_enc_config_set(&codec, &cfg);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&codec);
        (void)error_str; // Prevent unused warning
    }
    
    // Clean up resources
    aom_img_free(wrapped_img); // Note: this won't free img_data due to img_data_owner=0
    free(img_data);
    aom_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}