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
#include <lcms2.h>
}
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 43.333332, nr_unique_branch: 25
//<Quality> {"density":26,"unique_branches":{"IsInkSpace":[[1063,6,1063,18,0,0,4,1],[1076,6,1076,13,0,0,4,0]],"PackDoublesFromFloat":[[3120,35,3120,65,0,0,4,1],[3127,12,3127,22,0,0,4,1],[3130,20,3130,29,0,0,4,0],[3130,20,3130,29,0,0,4,1],[3132,39,3132,45,0,0,4,1],[3136,19,3136,26,0,0,4,1],[3139,19,3139,25,0,0,4,1],[3145,12,3145,22,0,0,4,0],[3145,26,3145,35,0,0,4,1],[731,31,731,44,7,0,4,1]],"UnrollDoublesToFloat":[[1418,32,1418,62,0,0,4,1],[1424,9,1424,15,0,0,4,1],[1432,9,1432,19,0,0,4,1],[1435,15,1435,24,0,0,4,0],[1435,15,1435,24,0,0,4,1],[1437,33,1437,39,0,0,4,1],[1439,13,1439,19,0,0,4,1],[1445,13,1445,19,0,0,4,1],[1450,42,1450,49,0,0,4,1],[1454,9,1454,19,0,0,4,0],[1454,23,1454,32,0,0,4,1],[731,31,731,44,8,0,4,1]],"PixelSize":[[1086,9,1086,23,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsGetProfileInfo","cmsMLUdup","cmsMLUgetUTF8","cmsMLUfree","cmsPipelineFree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsfilelength","cmsWhitePointFromTemp","cmsxyY2XYZ","cmsD50_xyY","cmsxyY2XYZ","cmsAdaptToIlluminant","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsGetProfileInfo","cmsMLUdup","cmsMLUgetUTF8","cmsMLUfree","cmsPipelineFree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsfilelength","cmsWhitePointFromTemp","cmsxyY2XYZ","cmsD50_xyY","cmsxyY2XYZ","cmsAdaptToIlluminant","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/



// Simple error handler that does nothing
static void  id_000077_NullErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Do nothing - just suppress errors during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }
    
    // Set error handler
    cmsSetLogErrorHandlerTHR(context,  id_000077_NullErrorHandler);
    
    // Create input FILE* from the data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from stream
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Compute MD5 ID for the profile
    cmsBool md5Result = cmsMD5computeID(profile);
    (void)md5Result; // Result not used in fuzzing
    
    // Check if perceptual intent is supported
    cmsBool intentSupported = cmsIsIntentSupported(profile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    (void)intentSupported;
    
    // Get profile context ID
    cmsContext profileContext = cmsGetProfileContextID(profile);
    (void)profileContext;
    
    // Get profile I/O handler
    cmsIOHANDLER* ioHandler = cmsGetProfileIOhandler(profile);
    (void)ioHandler;
    
    // Create output FILE* for writing
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Open I/O handler from stream
        cmsIOHANDLER* outIOHandler = cmsOpenIOhandlerFromStream(context, out_file);
        
        // Save profile to I/O handler
        if (outIOHandler) {
            cmsUInt32Number saved = cmsSaveProfileToIOhandler(profile, outIOHandler);
            (void)saved;
            
            // Close I/O handler
            cmsCloseIOhandler(outIOHandler);
        }
        assert_file_closed(&out_file);;
    }
    
    // Create a pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Get profile info (description)
        wchar_t buffer[256];
        cmsUInt32Number infoSize = cmsGetProfileInfo(profile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer)/sizeof(wchar_t));
        (void)infoSize;
        
        // Try to get UTF8 version
        char utf8Buffer[256];
        cmsMLU* mlu = cmsMLUdup(nullptr); // Create empty MLU for testing
        if (mlu) {
            cmsUInt32Number utf8Size = cmsMLUgetUTF8(mlu, "en", "US", utf8Buffer, sizeof(utf8Buffer));
            (void)utf8Size;
            cmsMLUfree(mlu);
        }
        
        // Free pipeline
        cmsPipelineFree(pipeline);
    }
    
    // Create a transform from profile to sRGB
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    if (srgbProfile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                                                      profile, 
                                                      TYPE_RGB_DBL, 
                                                      srgbProfile, 
                                                      TYPE_RGB_DBL, 
                                                      INTENT_PERCEPTUAL, 
                                                      cmsFLAGS_NOCACHE);
        
        if (transform) {
            // Perform transformation on some test data
            double inputColor[3] = {0.5, 0.5, 0.5};
            double outputColor[3];
            cmsDoTransform(transform, inputColor, outputColor, 1);
            
            // Delete transform
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(srgbProfile);
    }
    
    // IT8 operations
    // Write data to a temporary file for IT8 loading
    FILE* it8_file = fopen("it8_temp_file", "wb");
    if (it8_file && size > 0) {
        fwrite(data, 1, size, it8_file);
        assert_file_closed(&it8_file);;
        
        // Load IT8 from file
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "it8_temp_file");
        if (it8) {
            // Get property
            const char* prop = cmsIT8GetPropertyMulti(it8, "KEY", "SUBKEY");
            (void)prop;
            
            // Set comment
            cmsIT8SetComment(it8, "Fuzz test comment");
            
            // Free IT8
            cmsIT8Free(it8);
        }
        
        // Get file length
        FILE* length_file = fopen("it8_temp_file", "rb");
        if (length_file) {
            long length = cmsfilelength(length_file);
            (void)length;
            assert_file_closed(&length_file);;
        }
        
        // Clean up temporary file
        remove("it8_temp_file");
    }
    
    // White point and adaptation operations
    cmsCIExyY whitePoint;
    cmsBool wpResult = cmsWhitePointFromTemp(&whitePoint, 6500.0); // D65
    if (wpResult) {
        cmsCIEXYZ d50, d65, adaptedXYZ;
        
        // Convert xyY to XYZ for D50 and D65
        cmsxyY2XYZ(&d50, cmsD50_xyY());
        cmsxyY2XYZ(&d65, &whitePoint);
        
        // Test value (mid gray)
        cmsCIEXYZ testValue = {0.2, 0.2, 0.2};
        
        // Adapt from D50 to D65
        cmsAdaptToIlluminant(&adaptedXYZ, &d50, &d65, &testValue);
    }
    
    // Clean up
    cmsCloseProfile(profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&it8_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}