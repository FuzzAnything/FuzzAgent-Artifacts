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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGetTagSignature","cmsSetColorSpace","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandler2THR","cmsCreateXYZProfileTHR","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsCreateProofingTransform","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGetTagSignature","cmsSetColorSpace","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Open profile from memory (input data)
    cmsHPROFILE profile_from_mem = cmsOpenProfileFromMem(data, size);
    
    // Create stream from input data
    FILE *stream = fmemopen((void*)data, size, "rb");
    if (stream) {
        // Create I/O handler from stream
        cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromStream(context, stream);
        
        if (io_handler) {
            // Open profile from I/O handler (read mode)
            cmsHPROFILE profile_from_io = cmsOpenProfileFromIOhandler2THR(context, io_handler, FALSE);
            
            // Create XYZ profile
            cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
            
            // Get D50 white point
            const cmsCIExyY *d50 = cmsD50_xyY();
            
            // Create Lab4 profile with D50 white point
            cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, d50);
            
            // Create proofing transform if we have valid profiles
            cmsHTRANSFORM transform = NULL;
            if (profile_from_mem && xyz_profile && lab_profile) {
                transform = cmsCreateProofingTransform(
                    profile_from_mem,      // Input profile
                    TYPE_RGB_8,            // Input format
                    xyz_profile,           // Output profile  
                    TYPE_XYZ_16,           // Output format
                    lab_profile,           // Proofing profile
                    INTENT_PERCEPTUAL,     // Intent
                    INTENT_RELATIVE_COLORIMETRIC, // Proofing intent
                    0                      // Flags
                );
            }
            
            // Create linearization device link (RGB with identity curves)
            cmsToneCurve* curves[3] = {
                cmsBuildGamma(NULL, 1.0),  // Identity curve for R
                cmsBuildGamma(NULL, 1.0),  // Identity curve for G  
                cmsBuildGamma(NULL, 1.0)   // Identity curve for B
            };
            
            cmsHPROFILE device_link = NULL;
            if (curves[0] && curves[1] && curves[2]) {
                device_link = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
                
                // Free tone curves
                cmsFreeToneCurve(curves[0]);
                cmsFreeToneCurve(curves[1]);
                cmsFreeToneCurve(curves[2]);
            }
            
            // Get tag signature from profile
            if (profile_from_mem) {
                cmsTagSignature sig = cmsGetTagSignature(profile_from_mem, 0);
            }
            
            // Set color space of profile
            if (profile_from_io) {
                cmsSetColorSpace(profile_from_io, cmsSigRgbData);
            }
            
            // Create IT8 handle
            cmsHANDLE it8 = cmsIT8Alloc(context);
            if (it8) {
                // Set IT8 property
                cmsIT8SetPropertyDbl(it8, "SampleID", 123.456);
                
                // Get IT8 property
                cmsFloat64Number prop_val = cmsIT8GetPropertyDbl(it8, "SampleID");
                
                // Set data in IT8 table
                cmsIT8SetDataRowCol(it8, 0, 0, "TestData");
                
                // Save IT8 to file
                cmsIT8SaveToFile(it8, "output_file");
                
                // Free IT8 handle
                cmsIT8Free(it8);
            }
            
            // Clean up profiles
            if (profile_from_io) cmsCloseProfile(profile_from_io);
            if (xyz_profile) cmsCloseProfile(xyz_profile);
            if (lab_profile) cmsCloseProfile(lab_profile);
            if (device_link) cmsCloseProfile(device_link);
            
            // Clean up transform
            if (transform) cmsDeleteTransform(transform);
        }
        
        assert_file_closed(&stream);;
    }
    
    // Clean up profile from memory
    if (profile_from_mem) cmsCloseProfile(profile_from_mem);
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&stream);
	return 0;
}