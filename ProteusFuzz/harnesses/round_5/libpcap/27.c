// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal pcap header to make it a valid pcap file for libpcap
        // Magic number for microsecond precision: 0xa1b2c3d4
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);

        // Write the fuzz input as packet data if it's large enough
        if (Size > 0) {
            // Write a packet header
            struct pcap_pkthdr hdr;
            hdr.ts.tv_sec = 0;
            hdr.ts.tv_usec = 0;
            hdr.caplen = Size;
            hdr.len = Size;
            fwrite(&hdr, sizeof(hdr), 1, f);
            
            // Write packet data
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;
    FILE *fp = NULL;
    int file_closed_by_pcap = 0;
    
    // Ensure we have some data to work with
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzz input
    write_dummy_file(Data, Size);

    // Test pcap_open_offline
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        // Test pcap_next_ex
        ret = pcap_next_ex(handle, &header, &packet);
        if (ret == 1) {
            // Packet read successfully
            // Access data to trigger potential issues
            if (packet && header) {
                volatile uint8_t x = packet[0];
                (void)x;
            }
        } else if (ret == -2) {
            // EOF or timeout, normal for small files
        } else if (ret == -1) {
            // Error
        }
        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_open_offline_with_tstamp_precision
    handle = pcap_open_offline_with_tstamp_precision("./dummy_file", PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (handle) {
        ret = pcap_next_ex(handle, &header, &packet);
        if (ret == 1) {
            if (packet && header) {
                volatile uint8_t x = packet[0];
                (void)x;
            }
        }
        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline(fp, errbuf);
        if (handle) {
            file_closed_by_pcap = 1;
            ret = pcap_next_ex(handle, &header, &packet);
            if (ret == 1) {
                if (packet && header) {
                    volatile uint8_t x = packet[0];
                    (void)x;
                }
            }
            pcap_close(handle);
            handle = NULL;
        } else {
            file_closed_by_pcap = 0;
        }
        
        // Only close if pcap_fopen_offline failed (it didn't take ownership)
        if (!file_closed_by_pcap && fp) {
            fclose(fp);
            fp = NULL;
        }
    }

    // Test pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline_with_tstamp_precision(fp, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
        if (handle) {
            file_closed_by_pcap = 1;
            ret = pcap_next_ex(handle, &header, &packet);
            if (ret == 1) {
                if (packet && header) {
                    volatile uint8_t x = packet[0];
                    (void)x;
                }
            }
            pcap_close(handle);
            handle = NULL;
        } else {
            file_closed_by_pcap = 0;
        }

        // Only close if pcap_fopen_offline_with_tstamp_precision failed (it didn't take ownership)
        if (!file_closed_by_pcap && fp) {
            fclose(fp);
            fp = NULL;
        }
    }

    // Test pcap_file
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        FILE *stream = pcap_file(handle);
        if (stream) {
            // Try to read from the stream directly
            fseek(stream, 0, SEEK_SET);
            char buf[1024];
            size_t read_size = fread(buf, 1, sizeof(buf), stream);
            (void)read_size;
        }
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}