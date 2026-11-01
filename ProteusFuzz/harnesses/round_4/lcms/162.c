#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static const char* extract_string_from_fuzz(const uint8_t *Data, size_t Size, size_t *out_len) {
    if (Size == 0) {
        *out_len = 0;
        return NULL;
    }
    size_t len = 0;
    while (len < Size && Data[len] != '\0') len++;
    if (len == 0) {
        *out_len = 0;
        return NULL;
    }
    *out_len = len;
    return (const char*)Data;
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsHANDLE hIT8 = NULL;

    // Write fuzz data to a dummy file
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write raw fuzz data
        fwrite(Data, 1, Size, f);
        
        // Append specific test cases to ensure coverage of ParseFloatNumber branches
        // These lines attempt to trigger:
        // - Sign handling (+/-)
        // - Decimal point handling
        // - Exponent handling (E/e)
        // - NULL handling (via non-existent keys in GetData calls)
        fprintf(f, "\nTestProp -12.34E+10\n");
        fprintf(f, "TestProp2 +5.67e-5\n");
        fprintf(f, "TestProp3 123.456\n");
        fprintf(f, "TestProp4 0.0\n");
        fprintf(f, "TestProp5 1E20\n");
        fprintf(f, "TestProp6 -0.001E-3\n");
        
        fclose(f);
    }

    if (f) {
        hIT8 = cmsIT8LoadFromFile(ContextID, "./dummy_file");
    }

    if (hIT8) {
        // 1. Test cmsIT8GetPropertyDbl (Call Chain 0)
        // Triggers ParseFloatNumber with various formats
        cmsFloat64Number res1 = cmsIT8GetPropertyDbl(hIT8, "TestProp");
        (void)res1;
        cmsFloat64Number res2 = cmsIT8GetPropertyDbl(hIT8, "TestProp2");
        (void)res2;
        cmsFloat64Number res3 = cmsIT8GetPropertyDbl(hIT8, "TestProp3");
        (void)res3;
        cmsFloat64Number res4 = cmsIT8GetPropertyDbl(hIT8, "TestProp4");
        (void)res4;
        cmsFloat64Number res5 = cmsIT8GetPropertyDbl(hIT8, "TestProp5");
        (void)res5;
        cmsFloat64Number res6 = cmsIT8GetPropertyDbl(hIT8, "TestProp6");
        (void)res6;
        
        // Try with a property name from fuzz data
        size_t prop_len;
        const char* prop_name = extract_string_from_fuzz(Data, Size, &prop_len);
        if (prop_name && prop_len > 0) {
            cmsFloat64Number res7 = cmsIT8GetPropertyDbl(hIT8, prop_name);
            (void)res7;
        }

        // 2. Test cmsIT8GetDataDbl to cover NULL case (Line 719)
        // If patch/sample doesn't exist, cmsIT8GetData returns NULL,
        // which is passed directly to ParseFloatNumber.
        cmsFloat64Number res8 = cmsIT8GetDataDbl(hIT8, "NonExistentPatch", "NonExistentSample");
        (void)res8;
        
        // Try with fuzz data as patch/sample names
        size_t patch_len, sample_len;
        const char* patch = extract_string_from_fuzz(Data + 1, Size > 1 ? Size - 1 : 0, &patch_len);
        const char* sample = extract_string_from_fuzz(Data + 2, Size > 2 ? Size - 2 : 0, &sample_len);
        
        if (patch && patch_len > 0) {
            cmsFloat64Number res9 = cmsIT8GetDataDbl(hIT8, patch, sample ? sample : "dummy");
            (void)res9;
        }

        // 3. Test cmsIT8GetDataRowColDbl
        cmsFloat64Number res10 = cmsIT8GetDataRowColDbl(hIT8, 0, 0);
        (void)res10;
        cmsFloat64Number res11 = cmsIT8GetDataRowColDbl(hIT8, -1, 0);
        (void)res11;
        cmsFloat64Number res12 = cmsIT8GetDataRowColDbl(hIT8, 100, 100);
        (void)res12;
    }

    // Cleanup
    if (hIT8) cmsIT8Free(hIT8);
    if (ContextID) cmsDeleteContext(ContextID);
    
    remove("./dummy_file");

    return 0;
}