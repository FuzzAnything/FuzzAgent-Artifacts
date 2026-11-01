// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>

static void setup_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal pcap global header to make it a valid savefile
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        int32_t tz = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t linktype = 1;

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&linktype, sizeof(linktype), 1, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *dummy_file = "./dummy_file";
    pcap_t *handle = NULL;
    int ret;
    const char *prefix = "FUZZ_PREFIX";

    // Ensure we have enough data to interpret as parameters if needed
    if (Size < 1) {
        return 0;
    }

    // Setup dummy file for savefile operations
    setup_dummy_file(dummy_file);

    // 1. Fuzz pcap_init
    // Use first byte to determine encoding mode (0 or 1)
    unsigned int enc_mode = (Data[0] % 2) ? PCAP_CHAR_ENC_UTF_8 : PCAP_CHAR_ENC_LOCAL;
    ret = pcap_init(enc_mode, errbuf);
    // Ignore return, just exercise the call

    // 2. Fuzz pcap_statustostr and pcap_strerror with various codes
    // Use second byte as status code if available
    if (Size > 1) {
        int status_code = (int)(Data[1]);
        const char *status_str = pcap_statustostr(status_code);
        (void)status_str; // Suppress unused warning
    }

    // Use third byte as error number if available
    if (Size > 2) {
        int err_num = (int)(Data[2]);
        const char *err_str = pcap_strerror(err_num);
        (void)err_str; // Suppress unused warning
    }

    // 3. Try to open a handle to test pcap_perror, pcap_geterr, pcap_setnonblock
    if (Size >= 3) {
        // Determine if we try to open file or live device based on byte 2 (index 2)
        // Re-using index 2 for logic, but we need index 3 for device name logic below
        // Let's use index 3 if available, else default
        int use_file = (Size > 3 && Data[3] % 2 == 0) ? 1 : 0;

        if (use_file) {
            // Try opening the dummy savefile
            handle = pcap_open_offline(dummy_file, errbuf);
        } else {
            // Try opening a device (likely to fail, which is good for error testing)
            char device_name[256];
            size_t dev_len = (Size > 4) ? (Size - 4) : 0;
            if (dev_len > 255) dev_len = 255;
            
            if (dev_len > 0) {
                memcpy(device_name, Data + 4, dev_len);
                device_name[dev_len] = '\0';
            } else {
                strcpy(device_name, "lo");
            }
            
            // If the generated name is empty or looks invalid, default to "lo"
            if (dev_len == 0 || device_name[0] == '\0') {
                strcpy(device_name, "lo");
            }
            
            handle = pcap_create(device_name, errbuf);
            if (handle) {
                // Try to activate it
                ret = pcap_activate(handle);
                if (ret != 0) {
                    // Activation failed, handle might still be usable for error checks
                }
            }
        }
    }

    if (handle) {
        // Test pcap_geterr
        const char *err_msg = pcap_geterr(handle);
        (void)err_msg;

        // Test pcap_setnonblock
        // Use byte 4 (if available) to determine nonblock mode
        int nonblock_mode = (Size > 4) ? (Data[4] % 2) : 0;
        ret = pcap_setnonblock(handle, nonblock_mode, errbuf);
        // Ignore return, just exercise

        // Test pcap_perror with a prefix derived from input
        pcap_perror(handle, prefix);

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Additional fuzzing for global functions if more data is available
    if (Size > 5) {
        int status_code2 = (int)(Data[5]);
        const char *status_str2 = pcap_statustostr(status_code2);
        (void)status_str2;
        
        if (Size > 6) {
            int err_num2 = (int)(Data[6] % 256);
            const char *err_str2 = pcap_strerror(err_num2);
            (void)err_str2;
        }
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}