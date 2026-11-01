// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize global state
    char errbuf_init[PCAP_ERRBUF_SIZE];
    pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf_init);

    // Prepare source string from input
    char source[256];
    size_t src_len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, src_len);
    source[src_len] = '\0';

    // Ensure source is not empty for file operations
    if (src_len == 0) {
        strcpy(source, "./dummy_file");
    }

    // Create dummy file if source looks like a file path or is arbitrary data
    write_dummy_file(Data, Size);

    // Try to open a capture session
    pcap_t *handle = pcap_open(source, 65535, 0, 1000, NULL, errbuf_init);

    if (handle) {
        struct pcap_pkthdr *header;
        const u_char *packet;
        int ret;

        // Fuzz pcap_next_ex
        ret = pcap_next_ex(handle, &header, &packet);
        (void)ret;

        // Fuzz pcap_stats
        struct pcap_stat stats;
        memset(&stats, 0, sizeof(stats));
        ret = pcap_stats(handle, &stats);
        (void)ret;

        // Fuzz pcap_perror
        pcap_perror(handle, "Fuzz prefix");

        // Cleanup
        pcap_close(handle);
    } else {
        // If open failed, test error handling functions
        // pcap_geterr returns a string, pcap_strerror takes an int error code.
        // We cannot pass the string from pcap_geterr to pcap_strerror.
        // Instead, we use standard error codes or PCAP_ERROR.
        const char *err_str = pcap_strerror(PCAP_ERROR);
        (void)err_str;
        
        // Do not call pcap_perror with NULL handle as it may dereference NULL internally
        // based on the sanitizer error.
    }

    // Re-initialize with different encoding to test state changes
    char errbuf_init2[PCAP_ERRBUF_SIZE];
    pcap_init(PCAP_CHAR_ENC_UTF_8, errbuf_init2);

    // Test pcap_strerror with various error codes
    pcap_strerror(0);
    pcap_strerror(-1);
    pcap_strerror(-2);
    pcap_strerror(PCAP_ERROR);
    pcap_strerror(PCAP_ERROR_NO_SUCH_DEVICE);

    return 0;
}