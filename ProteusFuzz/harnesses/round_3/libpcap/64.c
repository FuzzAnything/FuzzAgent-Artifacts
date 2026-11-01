#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <pcap.h>

// Helper to create a minimal valid pcap file for fuzzing file sources
static int create_dummy_pcap(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    // Minimal pcap global header (24 bytes)
    // Magic number, Version, Timezone, Sigfigs, Snaplen, Network
    uint8_t header[24] = {
        0xa1, 0xb2, 0xc3, 0xd4, // Magic (little endian)
        0x02, 0x00, 0x04, 0x00, // Version 2.4
        0x00, 0x00, 0x00, 0x00, // Timezone
        0x00, 0x00, 0x00, 0x00, // Sigfigs
        0x00, 0x00, 0x00, 0x00, // Snaplen
        0x01, 0x00, 0x00, 0x00  // Network (Ethernet)
    };
    
    fwrite(header, 1, 24, f);
    
    // Minimal pcap packet header (16 bytes) + 1 byte data
    uint8_t pkt_header[16] = {
        0x00, 0x00, 0x00, 0x00, // ts_sec
        0x00, 0x00, 0x00, 0x00, // ts_usec
        0x01, 0x00, 0x00, 0x00, // incl_len
        0x01, 0x00, 0x00, 0x00  // orig_len
    };
    fwrite(pkt_header, 1, 16, f);
    uint8_t data[1] = {0x00};
    fwrite(data, 1, 1, f);

    fclose(f);
    return 0;
}

static void cleanup_pcap_if(pcap_if_t *devs) {
    if (devs != NULL) {
        pcap_freealldevs(devs);
    }
}

static void cleanup_fuzz_resources(void) {
    // Remove dummy file
    remove("./fuzz_pcap_dir/dummy.pcap");
    // Remove directory
    rmdir("./fuzz_pcap_dir");
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    char source[2048]; // Larger buffer to test overflow logic
    struct pcap_rmtauth auth;
    int ret;

    memset(errbuf, 0, sizeof(errbuf));
    memset(source, 0, sizeof(source));
    memset(&auth, 0, sizeof(auth));

    // Ensure minimal input
    if (Size == 0) {
        return 0;
    }

    // 1. Test Line 4639: Source string too long
    // PCAP_BUF_SIZE is typically 1024. Let's create a string > 1024.
    if (Size > 1024) {
        size_t len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
        memcpy(source, Data, len);
        source[len] = '\0';
        
        // Force it to be a valid prefix but too long
        // We just pass the long string. pcap_findalldevs_ex checks strlen(source) > PCAP_BUF_SIZE
        ret = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
        cleanup_pcap_if(alldevs);
        alldevs = NULL;
    }

    // 2. Test PCAP_SRC_IFLOCAL (Lines 4666, 4673, 4676, 4694, 4702, 4714, 4718)
    // Use "pcap://any" or "any" to trigger local interface discovery
    const char *local_src = "pcap://any";
    ret = pcap_findalldevs_ex(local_src, NULL, &alldevs, errbuf);
    cleanup_pcap_if(alldevs);
    alldevs = NULL;

    // 3. Test PCAP_SRC_FILE (Lines 4747, 4763, 4795, 4807, 4825, 4834, 4837, 4854, 4870, 4882, 4898, 4919)
    // Setup: Create a directory with a valid pcap file
    mkdir("./fuzz_pcap_dir", 0755);
    if (create_dummy_pcap("./fuzz_pcap_dir/dummy.pcap") == 0) {
        // Construct source string for file listing
        // Format: file:///path/to/dir
        snprintf(source, sizeof(source), "file:///fuzz_pcap_dir");
        
        ret = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
        cleanup_pcap_if(alldevs);
        alldevs = NULL;
    }
    
    // Cleanup resources created for file test
    cleanup_fuzz_resources();

    // 4. Test PCAP_SRC_IFREMOTE (Line 4933) and Default (Line 4942)
    // Try a remote source string
    const char *remote_src = "rpcap://127.0.0.1";
    ret = pcap_findalldevs_ex(remote_src, NULL, &alldevs, errbuf);
    cleanup_pcap_if(alldevs);
    alldevs = NULL;

    // Try an invalid source type to hit default case
    const char *invalid_src = "invalid://source";
    ret = pcap_findalldevs_ex(invalid_src, NULL, &alldevs, errbuf);
    cleanup_pcap_if(alldevs);
    alldevs = NULL;

    // 5. Test with Auth structure (though often ignored for local/file, good for coverage)
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = "fuzz_user";
    auth.password = "fuzz_pass";
    
    // Re-create dir for one more test with auth
    mkdir("./fuzz_pcap_dir", 0755);
    if (create_dummy_pcap("./fuzz_pcap_dir/dummy.pcap") == 0) {
        snprintf(source, sizeof(source), "file:///fuzz_pcap_dir");
        ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
        cleanup_pcap_if(alldevs);
        alldevs = NULL;
        cleanup_fuzz_resources();
    }

    return 0;
}