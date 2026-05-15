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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 71.5, nr_unique_branch: 158
//<Quality> {"density":11,"unique_branches":{"arg_match_helper":[[45,7,45,22,0,0,4,0],[45,26,45,67,0,0,4,1]],"av1_set_speed_features_qindex_dependent":[[2929,16,2929,25,0,0,4,1],[34,23,34,34,6,0,4,0]],"populate_unified_cost_update_freq":[[34,23,34,34,2,0,4,0],[34,23,34,34,3,0,4,0]],"av1_set_cost_upd_freq":[[1638,5,1638,33,0,0,4,0],[1639,5,1639,30,0,0,4,1],[1653,5,1653,33,0,0,4,0],[1654,5,1654,30,0,0,4,1]],"encode_frame_internal":[[2458,8,2459,17,0,0,4,0],[2460,8,2460,74,0,0,4,0],[2463,17,2463,33,0,0,4,0],[2463,17,2463,33,0,0,4,1],[2468,19,2468,31,0,0,4,0],[2468,19,2468,31,0,0,4,1],[2471,30,2471,36,0,0,4,0],[2471,30,2471,36,0,0,4,1],[2474,13,2474,16,0,0,4,0],[2474,13,2474,16,0,0,4,1],[2476,20,2476,21,0,0,4,0],[2476,20,2476,21,0,0,4,1],[2502,13,2502,77,0,0,4,1],[2507,13,2507,37,0,0,4,0],[2511,15,2511,21,0,0,4,0],[2511,15,2511,21,0,0,4,1]],"validate_config":[[667,11,667,28,52,0,4,1],[667,32,667,49,52,0,4,1]],"av1_rd_pick_intra_sby_mode":[[1592,9,1593,29,0,0,4,0],[1730,9,1730,59,0,0,4,0]],"skip_trellis_opt_based_on_satd":[[66,4,66,11,3,0,4,0]],"encoder_set_option":[[4365,14,4366,42,0,0,4,1],[4368,14,4369,42,0,0,4,1],[4371,14,4372,42,0,0,4,1],[4374,14,4375,42,0,0,4,1],[4377,14,4379,48,0,0,4,1],[4382,14,4383,42,0,0,4,1],[4385,14,4386,42,0,0,4,1],[4388,14,4389,42,0,0,4,1],[4391,14,4392,42,0,0,4,1],[4399,14,4400,42,0,0,4,1],[4407,14,4408,42,0,0,4,1],[4410,14,4411,48,0,0,4,1],[4413,14,4414,42,0,0,4,1],[4416,14,4417,42,0,0,4,1],[4419,14,4420,42,0,0,4,1],[4431,12,4432,46,0,0,4,1],[4435,14,4437,48,0,0,4,1],[4440,14,4442,48,0,0,4,1],[4446,14,4447,42,0,0,4,1],[4449,14,4450,42,0,0,4,0],[4449,14,4450,42,0,0,4,1],[4452,14,4453,48,0,0,4,1],[4456,14,4457,48,0,0,4,1],[4460,14,4461,48,0,0,4,1],[4463,14,4464,42,0,0,4,1],[4466,14,4467,42,0,0,4,1],[4469,14,4470,48,0,0,4,1],[4472,14,4473,48,0,0,4,1],[4475,14,4476,42,0,0,4,1],[4478,14,4479,48,0,0,4,1],[4481,14,4482,42,0,0,4,1],[4484,14,4485,42,0,0,4,1],[4487,14,4488,42,0,0,4,1],[4490,14,4491,42,0,0,4,1],[4493,14,4494,42,0,0,4,1],[4496,14,4497,42,0,0,4,1],[4499,14,4501,48,0,0,4,1],[4504,14,4505,48,0,0,4,1],[4507,14,4508,48,0,0,4,1],[4510,14,4511,42,0,0,4,1],[4513,14,4514,42,0,0,4,1],[4516,14,4517,42,0,0,4,1],[4519,14,4520,42,0,0,4,1],[4522,14,4523,48,0,0,4,1],[4525,14,4526,42,0,0,4,1],[4528,14,4529,48,0,0,4,1],[4531,14,4533,35,0,0,4,1],[4536,14,4538,48,0,0,4,1],[4540,14,4542,29,0,0,4,1],[4544,14,4545,42,0,0,4,1],[4547,14,4548,48,0,0,4,1],[4550,14,4551,42,0,0,4,1],[4553,14,4554,42,0,0,4,1],[4556,14,4557,48,0,0,4,1],[4566,14,4567,42,0,0,4,1],[4569,14,4571,48,0,0,4,1],[4573,14,4574,48,0,0,4,1],[4576,14,4578,48,0,0,4,1],[4580,14,4581,48,0,0,4,1],[4583,14,4584,48,0,0,4,1],[4586,14,4588,48,0,0,4,1],[4591,14,4592,48,0,0,4,1],[4594,14,4595,42,0,0,4,1],[4597,14,4598,48,0,0,4,1],[4600,14,4601,42,0,0,4,1],[4603,14,4604,48,0,0,4,1],[4606,14,4607,48,0,0,4,1],[4609,14,4610,48,0,0,4,1],[4613,14,4614,48,0,0,4,1],[4616,14,4617,48,0,0,4,1],[4619,14,4620,48,0,0,4,1],[4622,14,4624,48,0,0,4,1],[4626,14,4628,48,0,0,4,1],[4630,14,4631,48,0,0,4,1],[4633,14,4635,48,0,0,4,1],[4637,14,4639,48,0,0,4,1],[4641,14,4642,48,0,0,4,1],[4644,14,4645,48,0,0,4,1],[4647,14,4648,48,0,0,4,1],[4650,14,4651,48,0,0,4,1],[4653,14,4654,48,0,0,4,1],[4656,14,4657,48,0,0,4,1],[4659,14,4661,48,0,0,4,1],[4663,14,4664,48,0,0,4,1],[4666,14,4667,42,0,0,4,1],[4669,14,4670,42,0,0,4,1],[4672,14,4673,42,0,0,4,1],[4675,14,4676,48,0,0,4,1],[4678,14,4680,29,0,0,4,1],[4683,14,4684,48,0,0,4,1],[4686,14,4687,48,0,0,4,1],[4689,14,4690,48,0,0,4,1],[4692,14,4694,48,0,0,4,1],[4697,14,4698,42,0,0,4,1],[4700,14,4702,48,0,0,4,1],[4705,14,4706,42,0,0,4,1],[4708,14,4709,42,0,0,4,1],[4711,14,4712,48,0,0,4,1],[4714,14,4715,48,0,0,4,1],[4717,14,4718,48,0,0,4,1],[4720,14,4721,48,0,0,4,1],[4725,12,4726,46,0,0,4,1],[4729,14,4730,48,0,0,4,1],[4732,14,4733,48,0,0,4,1],[4737,12,4738,46,0,0,4,1],[4750,14,4752,48,0,0,4,1],[4754,14,4756,48,0,0,4,1],[4758,14,4759,42,0,0,4,1],[4761,14,4762,42,0,0,4,1],[4764,14,4765,42,0,0,4,1],[4768,14,4769,42,0,0,4,1],[4772,14,4773,48,0,0,4,1],[4775,14,4776,48,0,0,4,1],[4778,14,4780,48,0,0,4,1],[4782,14,4783,42,0,0,4,1],[4785,14,4786,48,0,0,4,1],[4788,14,4789,42,0,0,4,1],[4792,14,4793,42,0,0,4,1],[4796,14,4797,48,0,0,4,1],[4811,7,4811,30,0,0,4,0]],"get_cdef_filter_strengths":[[53,5,53,31,0,0,4,1],[59,5,59,31,0,0,4,0]],"update_extra_cfg":[[1758,7,1758,26,0,0,4,1]],"skip_cost_update":[[1560,7,1560,40,0,0,4,1],[1561,7,1561,41,0,0,4,1],[1564,7,1564,40,0,0,4,1],[1566,7,1566,47,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_encode","aom_codec_error","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_get_cx_data","aom_codec_error","aom_codec_encode","aom_codec_encode","aom_codec_error","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode input data and then re-encode it:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<124) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_15, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_16, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_17, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Variables to track resources
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_codec_iter_t iter = nullptr;
    std::vector<aom_image_t*> decoded_images;
    std::vector<uint8_t> file_buffer;
    aom_codec_err_t res = AOM_CODEC_OK;
    
    // Read the entire input file into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        file_buffer.resize(file_size);
        fread(file_buffer.data(), 1, file_size, in_file);
    }
    assert_file_closed(&in_file);;
    
    if (file_buffer.empty()) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);
	return 0;
    }

    res = aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Use error message to satisfy the API requirement
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    res = aom_codec_decode(&decoder, file_buffer.data(), file_buffer.size(), nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg;
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frames
    aom_image_t *img;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create a copy of the image
        aom_image_t *img_copy = aom_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, fuzz_uint32_t_2);
        if (img_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; plane++) {
                if (img->planes[plane]) {
                    int width = aom_img_plane_width(img, fuzz_int32_t_3);
                    int height = aom_img_plane_height(img, plane);
                    for (int y = 0; y < height; y++) {
                        memcpy(img_copy->planes[plane] + y * img_copy->stride[plane],
                               img->planes[plane] + y * img->stride[plane],
                               width);
                    }
                }
            }
            decoded_images.push_back(img_copy);
        }
    }

    // Initialize encoder if we have decoded frames
    if (!decoded_images.empty()) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            aom_codec_enc_cfg_t cfg;
            
            // Get default encoder configuration
            res = aom_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_4);
            if (res == AOM_CODEC_OK) {
                // Configure encoder based on first decoded image
                aom_image_t *first_img = decoded_images[0];
                cfg.g_w = first_img->d_w;
                cfg.g_h = first_img->d_h;
                cfg.g_timebase.num = 1;
                cfg.g_timebase.den = 30;
                cfg.rc_target_bitrate = 1000;
                cfg.g_pass = AOM_RC_ONE_PASS;
                cfg.g_lag_in_frames = 0;
                
                // Initialize encoder
                res = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_5, AOM_ENCODER_ABI_VERSION);
                if (res == AOM_CODEC_OK) {
                    // Set encoder options
                    aom_codec_set_option(&encoder, fuzz_str_6, fuzz_str_7);
                    aom_codec_set_option(&encoder, fuzz_str_8, fuzz_str_9);
                    aom_codec_set_option(&encoder, fuzz_str_10, fuzz_str_11);
                    
                    // Use aom_codec_control to set various parameters
                    // Set CPU used (control ID examples - these are hypothetical)
                    // Note: Actual control IDs would need to be defined
                    int ctrl_value = 8;
                    aom_codec_control(&encoder, AOME_SET_CPUUSED, &ctrl_value);
                    
                    // Encode each decoded frame
                    for (aom_image_t *img_to_encode : decoded_images) {
                        res = aom_codec_encode(&encoder, img_to_encode, fuzz_int64_t_12, fuzz_uint64_t_13, fuzz_int64_t_14);
                        if (res != AOM_CODEC_OK) {
                            const char *error_msg = aom_codec_error(&encoder);
                            (void)error_msg;
                            break;
                        }
                        
                        // Get encoded data (optional, for completeness)
                        const aom_codec_cx_pkt_t *pkt = nullptr;
                        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                            // Process encoded packet if needed
                            (void)pkt;
                        }
                    }
                    
                    // Flush encoder
                    res = aom_codec_encode(&encoder, nullptr, fuzz_int64_t_15, fuzz_uint64_t_16, fuzz_int64_t_17);
                    if (res != AOM_CODEC_OK) {
                        const char *error_msg = aom_codec_error(&encoder);
                        (void)error_msg;
                    }
                }
            }
        }
    }

    // Cleanup
    for (aom_image_t *img : decoded_images) {
        if (img) {
            aom_img_free(img);
        }
    }
    
    if (encoder.iface) {
        aom_codec_destroy(&encoder);
    }
    
    if (decoder.iface) {
        aom_codec_destroy(&decoder);
    }

    assert_file_closed(&in_file);
	return 0;
}