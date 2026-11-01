// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static thandle_t dummy_client_data;
static uint8_t dummy_buffer[4096];

static tmsize_t DummyReadProc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    if (size > sizeof(dummy_buffer)) {
        size = sizeof(dummy_buffer);
    }
    if (size <= 0) return 0;
    memcpy(buf, dummy_buffer, size);
    return size;
}

static tmsize_t DummyWriteProc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    if (size > sizeof(dummy_buffer)) {
        size = sizeof(dummy_buffer);
    }
    if (size <= 0) return 0;
    memcpy(dummy_buffer, buf, size);
    return size;
}

static toff_t DummySeekProc(thandle_t clientdata, toff_t off, int whence) {
    (void)clientdata;
    (void)off;
    (void)whence;
    return 0;
}

static int DummyCloseProc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static toff_t DummySizeProc(thandle_t clientdata) {
    (void)clientdata;
    return sizeof(dummy_buffer);
}

static int DummyMapFileProc(thandle_t clientdata, void **buf, toff_t *size) {
    (void)clientdata;
    if (size) *size = sizeof(dummy_buffer);
    if (buf) *buf = (void*)dummy_buffer;
    return 1;
}

static void DummyUnmapFileProc(thandle_t clientdata, tdata_t buf, toff_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare environment: copy input to dummy buffer for I/O simulation
    size_t copy_size = (Size < sizeof(dummy_buffer)) ? Size : sizeof(dummy_buffer);
    memcpy(dummy_buffer, Data, copy_size);
    
    // Initialize client data pointer
    dummy_client_data = (thandle_t)0x12345678;

    TIFFOpenOptions *opts = NULL;

    // Test TIFFClientOpenExt
    TIFF *tif = TIFFClientOpenExt(
        "fuzz.tiff",
        "r",
        dummy_client_data,
        DummyReadProc,
        DummyWriteProc,
        DummySeekProc,
        DummyCloseProc,
        DummySizeProc,
        (TIFFMapFileProc)DummyMapFileProc,
        DummyUnmapFileProc,
        opts
    );

    if (tif) {
        // Test TIFFSetClientdata
        thandle_t old_data = TIFFSetClientdata(tif, (thandle_t)0xDEADBEEF);
        (void)old_data;

        // Test TIFFClientdata
        thandle_t retrieved_data = TIFFClientdata(tif);
        (void)retrieved_data;

        // Test TIFFSetClientInfo
        char *test_key = "fuzz_key";
        void *test_value = (void *)0xCAFEBABE;
        TIFFSetClientInfo(tif, test_value, test_key);

        // Test TIFFGetClientInfo
        void *retrieved_info = TIFFGetClientInfo(tif, test_key);
        (void)retrieved_info;

        // Test TIFFGetClientInfo with non-existent key
        void *null_info = TIFFGetClientInfo(tif, "non_existent_key");
        (void)null_info;

        // TIFFClose calls TIFFCleanup internally. 
        // Calling TIFFCleanup again after TIFFClose causes use-after-free.
        // We only call TIFFClose here.
        TIFFClose(tif);
    }

    // Additional test: Open in write mode to trigger more paths
    tif = TIFFClientOpenExt(
        "fuzz_write.tiff",
        "w",
        dummy_client_data,
        DummyReadProc,
        DummyWriteProc,
        DummySeekProc,
        DummyCloseProc,
        DummySizeProc,
        (TIFFMapFileProc)DummyMapFileProc,
        DummyUnmapFileProc,
        opts
    );

    if (tif) {
        // Set client data and info again
        TIFFSetClientdata(tif, (thandle_t)0x11111111);
        TIFFSetClientInfo(tif, (void *)0x22222222, "write_mode_key");
        
        // Retrieve
        (void)TIFFClientdata(tif);
        (void)TIFFGetClientInfo(tif, "write_mode_key");
        
        // Close only, do not call Cleanup again
        TIFFClose(tif);
    }

    // Test with NULL opts explicitly
    tif = TIFFClientOpenExt(
        "fuzz_null_opts.tiff",
        "r",
        dummy_client_data,
        DummyReadProc,
        DummyWriteProc,
        DummySeekProc,
        DummyCloseProc,
        DummySizeProc,
        (TIFFMapFileProc)DummyMapFileProc,
        DummyUnmapFileProc,
        NULL
    );

    if (tif) {
        TIFFSetClientInfo(tif, (void *)0x33333333, "null_opts_key");
        (void)TIFFGetClientInfo(tif, "null_opts_key");
        // Close only
        TIFFClose(tif);
    }

    return 0;
}