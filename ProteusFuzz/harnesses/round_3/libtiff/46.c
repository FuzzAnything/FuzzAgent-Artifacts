// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open the dummy file to get a TIFF handle
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (tif == NULL) {
        // Try opening in write mode if read+write fails (file might be empty or invalid)
        tif = TIFFOpen("./dummy_file", "w");
        if (tif == NULL) {
            remove("./dummy_file");
            return 0;
        }
    }

    // 1. TIFFGetWriteProc: Retrieve the custom write callback
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc; // Suppress unused variable warning

    // 2. TIFFClientdata: Retrieve user-defined data
    thandle_t client_data = TIFFClientdata(tif);
    (void)client_data;

    // 3. TIFFSetClientdata: Store custom pointer
    // We use a static buffer to ensure it doesn't go out of scope immediately
    static char dummy_data[64];
    memset(dummy_data, 0, sizeof(dummy_data));
    thandle_t prev_data = TIFFSetClientdata(tif, (thandle_t)dummy_data);
    (void)prev_data;

    // 4. TIFFGetClientInfo: Retrieve user-defined data by name
    // First, we need to set some client info to test retrieval
    TIFFSetClientInfo(tif, "fuzz_key", (void*)"fuzz_value");
    void *retrieved_info = TIFFGetClientInfo(tif, "fuzz_key");
    (void)retrieved_info;

    // 5. TIFFAccessTagMethods: Access internal tag methods
    TIFFTagMethods *tag_methods = TIFFAccessTagMethods(tif);
    if (tag_methods != NULL) {
        // Just ensure we can access the struct without crashing
        (void)tag_methods;
    }

    // 6. Cleanup: 
    // TIFFClose() internally calls TIFFCleanup() and frees the TIFF structure.
    // Calling TIFFCleanup() explicitly before TIFFClose() causes a double-free/use-after-free.
    // Therefore, we only call TIFFClose().
    TIFFClose(tif);

    // Cleanup the dummy file
    remove("./dummy_file");

    return 0;
}