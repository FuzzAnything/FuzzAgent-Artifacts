// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t *data, size_t size) {
    static cmsIOHANDLER dummy_io;
    static uint8_t buffer[4096];
    
    // Initialize the handler structure
    memset(&dummy_io, 0, sizeof(dummy_io));
    
    // Copy fuzzer data to buffer if available
    size_t copy_size = size < sizeof(buffer) ? size : sizeof(buffer);
    if (copy_size > 0 && data != NULL) {
        memcpy(buffer, data, copy_size);
    }
    
    // Set up function pointers for basic operations
    dummy_io.Read = NULL;  // Will be implemented if needed
    dummy_io.Write = NULL; // Will be implemented if needed
    dummy_io.Seek = NULL;
    dummy_io.Close = NULL;
    dummy_io.Tell = NULL;
    
    // Store the data in stream pointer
    dummy_io.stream = buffer;
    dummy_io.UsedSpace = copy_size;
    dummy_io.ReportedSize = sizeof(buffer);
    
    return &dummy_io;
}

static cmsHPROFILE create_dummy_profile(void) {
    // Create a simple sRGB profile for testing
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    return profile;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    // Function 1: _cms15Fixed16toDouble
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(fix32));
        cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
        (void)result1; // Use result to avoid unused variable warning
    }
    
    // Function 2: cmsGetProfileVersion
    cmsHPROFILE profile = create_dummy_profile();
    if (profile != NULL) {
        cmsFloat64Number version = cmsGetProfileVersion(profile);
        (void)version;
        cmsCloseProfile(profile);
    }
    
    // Function 3: cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    (void)cmm_version;
    
    // Function 4: _cmsDoubleTo15Fixed16
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number double_val;
        memcpy(&double_val, Data, sizeof(double_val));
        cmsS15Fixed16Number fixed_result = _cmsDoubleTo15Fixed16(double_val);
        (void)fixed_result;
    }
    
    // Function 5: _cmsRead15Fixed16Number
    cmsIOHANDLER* io_read = create_dummy_iohandler(Data, Size);
    if (io_read != NULL) {
        cmsFloat64Number read_number;
        cmsBool read_success = _cmsRead15Fixed16Number(io_read, &read_number);
        (void)read_success;
        (void)read_number;
    }
    
    // Function 6: _cmsWrite15Fixed16Number
    cmsIOHANDLER* io_write = create_dummy_iohandler(NULL, 0);
    if (io_write != NULL && Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number write_number;
        memcpy(&write_number, Data, sizeof(write_number));
        cmsBool write_success = _cmsWrite15Fixed16Number(io_write, write_number);
        (void)write_success;
    }
    
    return 0;
}