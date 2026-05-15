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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 15
//<Quality> {"density":21,"unique_branches":{"_cmsStageNormalizeFromXyzFloat":[[1097,9,1097,20,0,0,4,1]],"_cmsStageNormalizeToXyzFloat":[[1134,9,1134,20,0,0,4,1]],"cmsCreate_OkLabProfile":[[759,9,759,18,0,0,4,1],[773,9,773,57,0,0,4,1],[774,9,774,59,0,0,4,1],[775,9,775,59,0,0,4,1],[776,9,776,65,0,0,4,1],[777,9,777,65,0,0,4,1],[779,9,779,53,0,0,4,1],[781,9,781,65,0,0,4,1],[782,9,782,65,0,0,4,1],[783,9,783,59,0,0,4,1],[784,9,784,59,0,0,4,1],[785,9,785,57,0,0,4,1],[787,9,787,53,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreate_OkLabProfile","cmsDeleteContext","cmsGetTagCount","cmsDictAlloc","cmsCloseProfile","cmsDeleteContext","cmsDictFree","cmsCloseProfile","cmsDeleteContext","cmsDictFree","cmsCloseProfile","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsCloseProfile","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreate_OkLabProfile","cmsGetTagCount","cmsDictAlloc","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsCloseProfile","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to achieve the event of creating a device link profile from a CUBE file and analyzing its structure:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a context for lcms operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // 1. Create an OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    if (!okLabProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Get tag count from the OkLab profile
    cmsInt32Number tagCount = cmsGetTagCount(okLabProfile);
    // We don't need to do anything with tagCount, just call the API

    // 3. Allocate a dictionary handle
    cmsHANDLE dictHandle = cmsDictAlloc(ctx);
    if (!dictHandle) {
        cmsCloseProfile(okLabProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 4. Create a temporary CUBE file from input data
    FILE *cubeFile = fmemopen((void *)data, size, "rb");
    if (!cubeFile) {
        cmsDictFree(dictHandle);
        cmsCloseProfile(okLabProfile);
        cmsDeleteContext(ctx);
        assert_file_closed(&cubeFile);
	return 0;
    }

    // Write data to a temporary file for cmsCreateDeviceLinkFromCubeFileTHR
    const char *tempCubeFileName = "temp_input.cube";
    FILE *tempFile = fopen(tempCubeFileName, "wb");
    if (!tempFile) {
        assert_file_closed(&cubeFile);;
        cmsDictFree(dictHandle);
        cmsCloseProfile(okLabProfile);
        cmsDeleteContext(ctx);
        assert_file_closed(&tempFile);
	assert_file_closed(&cubeFile);
	return 0;
    }

    // Copy data from memory to temporary file
    fwrite(data, 1, size, tempFile);
    assert_file_closed(&tempFile);;
    assert_file_closed(&cubeFile);;

    // 5. Create device link from CUBE file
    cmsHPROFILE deviceLinkProfile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, fuzz_str_1);
    
    // Clean up temporary file
    remove(tempCubeFileName);

    if (deviceLinkProfile) {
        // 6. Get the pipeline from the device link to access stages
        cmsPipeline *pipeline = cmsGetTransformPipeline(deviceLinkProfile);
        if (pipeline) {
            // Get the first stage of the pipeline
            cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                // 7. Get input channels of the first stage
                cmsUInt32Number inputChannels = cmsStageInputChannels(firstStage);
                // API called, value not used further
            }
        }
        cmsCloseProfile(deviceLinkProfile);
    }

    // 8. Clean up all allocated resources
    cmsDictFree(dictHandle);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);

    assert_file_closed(&tempFile);
	assert_file_closed(&cubeFile);
	return 0;
}