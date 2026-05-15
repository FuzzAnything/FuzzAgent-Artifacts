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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 5
//<Quality> {"density":24,"unique_branches":{"ComputeCorrelates":[[181,9,181,15,0,0,4,1],[186,14,186,19,0,0,4,0],[188,13,188,18,0,0,4,0],[201,9,201,22,0,0,4,1],[205,14,205,26,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsGetColorSpace","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsLabEncoded2Float","cmsLabEncoded2FloatV2","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsGetDeviceClass","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetColorSpace","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsLabEncoded2Float","cmsLabEncoded2FloatV2","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsGetDeviceClass","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful processing

  cmsContext context = cmsCreateContext(NULL, NULL);
  if (!context) {
	return 0;
	}
	

  // 1. Open profile from memory
  cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
  if (!profile) {
    cmsDeleteContext(context);
    return 0;
  }

  // 2. Get formatter for profile's color space
  cmsColorSpaceSignature colorspace = cmsGetColorSpace(profile);
  cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profile, 4, FALSE);

  // 3. Create and populate MLU
  cmsMLU* mlu = cmsMLUalloc(context, 5);
  if (mlu) {
    cmsMLUsetASCII(mlu, "EN", "US", "Test MLU String");
    cmsMLUfree(mlu);
  }

  // 4. Create and use tone curve
  cmsUInt16Number curveData[256];
  for (int i = 0; i < 256; i++) {
    curveData[i] = (cmsUInt16Number)((i * 65535) / 255);
  }
  cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(context, 256, curveData);
  if (curve) {
    cmsFreeToneCurve(curve);
  }

  // 5. Convert encoded Lab to float
  cmsCIELab labFloat;
  cmsUInt16Number encodedLab[3] = {32768, 32768, 32768};
  cmsLabEncoded2Float(&labFloat, encodedLab);
  cmsLabEncoded2FloatV2(&labFloat, encodedLab);

  // 6. Create and use IT8 handle
  cmsHANDLE it8 = cmsIT8Alloc(context);
  if (it8) {
    cmsIT8SetDataFormat(it8, 0, "R");
    cmsIT8SetDataFormat(it8, 1, "G");
    cmsIT8SetDataFormat(it8, 2, "B");
    
    // Use a small portion of input data for patch names
    char patchName[20];
    char sampleName[20];
    char value[20];
    
    if (size > 100) {
      snprintf(patchName, sizeof(patchName), "Patch%d", data[0]);
      snprintf(sampleName, sizeof(sampleName), "Sample%d", data[1]);
      snprintf(value, sizeof(value), "Value%d", data[2]);
      cmsIT8SetData(it8, patchName, sampleName, value);
      
      cmsFloat64Number retrieved = cmsIT8GetDataDbl(it8, patchName, sampleName);
      (void)retrieved; // Prevent unused variable warning
    }
    
    cmsIT8Free(it8);
  }

  // 7. Create and use named color list
  cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
  if (namedColorList) {
    cmsUInt16Number pcs[3] = {32768, 32768, 32768};
    cmsUInt16Number colorant[16] = {0};
    
    // Add a few named colors using input data
    for (int i = 0; i < 3 && i * 10 + 10 < size; i++) {
      char colorName[20];
      snprintf(colorName, sizeof(colorName), "Color%d", data[i*10]);
      cmsAppendNamedColor(namedColorList, colorName, pcs, colorant);
    }
    
    if (size > 0) {
      cmsNamedColorIndex(namedColorList, "Color0");
    }
    
    cmsFreeNamedColorList(namedColorList);
  }

  // 8. Open profile from stream (using fmemopen)
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (in_file) {
    cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (profile2) {
      cmsCloseProfile(profile2);
    }
    assert_file_closed(&in_file);;
  }

  // 9. Use CIECAM02 for color conversion
  cmsViewingConditions vc;
  memset(&vc, 0, sizeof(vc));
  vc.whitePoint.X = 95.05;
  vc.whitePoint.Y = 100.00;
  vc.whitePoint.Z = 108.88;
  vc.Yb = 20.0;
  vc.La = 64.0;
  vc.surround = 0;
  vc.D_value = 1.0;
  
  cmsHANDLE cam02 = cmsCIECAM02Init(context, &vc);
  if (cam02) {
    cmsCIEXYZ xyzInput;
    xyzInput.X = 23.0;
    xyzInput.Y = 24.0;
    xyzInput.Z = 25.0;
    
    cmsJCh jchOutput;
    cmsCIECAM02Forward(cam02, &xyzInput, &jchOutput);
    cmsCIECAM02Done(cam02);
  }

  // 10. Get device class
  cmsProfileClassSignature deviceClass = cmsGetDeviceClass(profile);

  // Cleanup
  cmsCloseProfile(profile);
  cmsDeleteContext(context);

  // Prevent unused variable warnings in release builds
  (void)formatter;
  (void)colorspace;
  (void)deviceClass;

  assert_file_closed(&in_file);
	return 0;
}