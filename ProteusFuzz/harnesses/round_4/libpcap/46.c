// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static int invoke_pcap_init(const uint8_t *data, size_t size) {
    int mode = 0;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (size > 0) {
        mode = data[0] % 2; // 0 or 1
    }

    // Try PCAP_CHAR_ENC_LOCAL (0) or PCAP_CHAR_ENC_UTF_8 (1)
    // Note: The actual enum values might differ, but we fuzz with 0 and 1
    // If the library defines them differently, this might trigger errors, which is fine for fuzzing.
    return pcap_init(mode, errbuf);
}

static int invoke_pcap_setnonblock(const uint8_t *data, size_t size) {
    // We cannot easily create a valid pcap_t handle without a real interface or file.
    // However, we can try to call it with a NULL handle or a dummy struct to test error handling paths
    // if the library checks for NULL.
    // Since pcap_setnonblock requires a valid handle, and creating one requires pcap_open_live or pcap_open_offline,
    // which depend on system state, we will skip direct invocation of pcap_setnonblock with a valid handle
    // unless we can open a dummy file.
    
    // Let's try to open a dummy file first to get a handle, then call setnonblock.
    const char *filename = "./dummy_file";
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Write some dummy data to the file to make it look like a pcap file (even if invalid)
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write minimal pcap header (magic number + version)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        // Linktype (ethernet)
        uint32_t linktype = 1;
        fwrite(&linktype, sizeof(linktype), 1, f);
        // Snapshot length
        uint32_t snaplen = 65535;
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fclose(f);
    }

    pcap_t *handle = pcap_open_offline(filename, errbuf);
    if (handle == NULL) {
        // If we can't open the file, we can't test setnonblock properly.
        // We return 0 to indicate we didn't test this path, but the fuzzer continues.
        return 0;
    }

    int nonblock = 0;
    if (size > 0) {
        nonblock = data[0] % 2;
    }

    int ret = pcap_setnonblock(handle, nonblock, errbuf);
    
    pcap_close(handle);
    return ret;
}

static void invoke_pcap_geterr(const uint8_t *data, size_t size) {
    // pcap_geterr requires a pcap_t handle.
    // Similar to setnonblock, we need a handle.
    const char *filename = "./dummy_file";
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Ensure file exists
    FILE *f = fopen(filename, "wb");
    if (f) {
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        uint32_t linktype = 1;
        fwrite(&linktype, sizeof(linktype), 1, f);
        uint32_t snaplen = 65535;
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fclose(f);
    }

    pcap_t *handle = pcap_open_offline(filename, errbuf);
    if (handle != NULL) {
        char *err = pcap_geterr(handle);
        // Just use the result to avoid unused variable warnings
        if (err) {
            (void)strlen(err);
        }
        pcap_close(handle);
    } else {
        // If handle is NULL, pcap_geterr might crash or return generic error depending on impl.
        // We skip calling it with NULL to avoid immediate crash, focusing on valid handle paths.
        // However, some fuzzers like to test NULL. Let's try it if the input suggests it.
        if (size > 0 && data[0] == 0xFF) {
             // Intentional NULL test if data indicates it
             // This might crash, which is a valid fuzzing outcome (crash = bug found)
             pcap_geterr(NULL);
        }
    }
}

static void invoke_pcap_perror(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    char errbuf[PCAP_ERRBUF_SIZE];
    
    FILE *f = fopen(filename, "wb");
    if (f) {
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        uint32_t linktype = 1;
        fwrite(&linktype, sizeof(linktype), 1, f);
        uint32_t snaplen = 65535;
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fclose(f);
    }

    pcap_t *handle = pcap_open_offline(filename, errbuf);
    
    // Generate a prefix from input data
    char prefix[256];
    size_t prefix_len = (size > 255) ? 255 : size;
    memcpy(prefix, data, prefix_len);
    prefix[prefix_len] = '\0';

    if (handle != NULL) {
        pcap_perror(handle, prefix);
        pcap_close(handle);
    } else {
        if (size > 0 && data[0] == 0xFF) {
            pcap_perror(NULL, prefix);
        }
    }
}

static void invoke_pcap_statustostr(const uint8_t *data, size_t size) {
    int status = 0;
    if (size > 0) {
        status = (int)data[0];
        if (size > 1) {
            status |= ((int)data[1]) << 8;
        }
        if (size > 2) {
            status |= ((int)data[2]) << 16;
        }
        if (size > 3) {
            status |= ((int)data[3]) << 24;
        }
    }
    
    const char *str = pcap_statustostr(status);
    if (str) {
        (void)strlen(str);
    }
}

static void invoke_pcap_strerror(const uint8_t *data, size_t size) {
    int errnum = 0;
    if (size > 0) {
        errnum = (int)data[0];
        if (size > 1) {
            errnum |= ((int)data[1]) << 8;
        }
        if (size > 2) {
            errnum |= ((int)data[2]) << 16;
        }
        if (size > 3) {
            errnum |= ((int)data[3]) << 24;
        }
    }
    
    const char *str = pcap_strerror(errnum);
    if (str) {
        (void)strlen(str);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // 1. Invoke pcap_init early
    // We call it every time to test re-initialization errors
    invoke_pcap_init(Data, Size);

    // 2. Invoke pcap_statustostr and pcap_strerror (no state required)
    invoke_pcap_statustostr(Data, Size);
    invoke_pcap_strerror(Data, Size);

    // 3. Invoke functions requiring a handle (pcap_geterr, pcap_perror, pcap_setnonblock)
    // These functions create a dummy file and open it to get a handle
    invoke_pcap_geterr(Data, Size);
    invoke_pcap_perror(Data, Size);
    invoke_pcap_setnonblock(Data, Size);

    return 0;
}