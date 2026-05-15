#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include "magic.h"

namespace harness_005 {
/*
 * Fuzzing harness for libmagic targeting deep ELF file format parsing
 * Primary Targets: doshn (815 undiscovered branches), dophn_exec (727), dophn_core (645), donote (595)
 * Focus on ELF parsing functions in readelf.c: section header processing, program headers, note sections
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic buffer detection with random operations
 * - harness_001: Configuration management and parameter tuning  
 * - harness_002: Database operations (compile, check, list)
 * - harness_003: Basic file operations and filesystem objects
 * - harness_004: Filesystem magic logic with minimal ELF testing
 * - harness_005: Deep ELF parsing with complex section structures, program headers, note sections
 */



// ELF structure definitions from readelf.h
#define EI_NIDENT 16
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define EM_NONE 0
#define EM_386 3
#define EM_SPARC 2
#define EM_SPARCV9 43
#define EM_AMD64 62
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_SUNW_cap 0x6ffffff5
#define NT_GNU_BUILD_ID 3
#define NT_PRSTATUS 1
#define NT_PRFPREG 2
#define NT_PRPSINFO 3

// Helper to generate ELF headers
class ELFGenerator {
public:
    static void generate_elf_header(uint8_t* buffer, size_t buffer_size, FuzzedDataProvider& fdp) {
        if (buffer_size < 64) return;
        
        // Determine ELF characteristics from fuzzer input
        uint8_t elf_class = fdp.ConsumeBool() ? ELFCLASS32 : ELFCLASS64;
        uint8_t elf_data = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
        uint16_t elf_type = fdp.ConsumeIntegralInRange<uint16_t>(ET_NONE, ET_CORE);
        uint16_t elf_machine = fdp.PickValueInArray({EM_NONE, EM_386, EM_SPARC, EM_SPARCV9, EM_AMD64});
        
        // Write ELF magic
        buffer[0] = ELFMAG0;
        buffer[1] = ELFMAG1;
        buffer[2] = ELFMAG2;
        buffer[3] = ELFMAG3;
        buffer[4] = elf_class;
        buffer[5] = elf_data;
        buffer[6] = 1; // Version
        buffer[7] = 0; // OS ABI
        
        // Fill rest of e_ident with random data
        for (int i = 8; i < EI_NIDENT; i++) {
            buffer[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Write header fields
        size_t offset = EI_NIDENT;
        
        // e_type
        buffer[offset] = elf_type & 0xFF;
        buffer[offset + 1] = (elf_type >> 8) & 0xFF;
        
        // e_machine
        buffer[offset + 2] = elf_machine & 0xFF;
        buffer[offset + 3] = (elf_machine >> 8) & 0xFF;
        
        // e_version
        buffer[offset + 4] = 1;
        buffer[offset + 5] = 0;
        buffer[offset + 6] = 0;
        buffer[offset + 7] = 0;
        
        // Fill remaining header with fuzzer data
        size_t header_remaining = buffer_size - offset - 8;
        if (header_remaining > 0) {
            auto header_data = fdp.ConsumeBytes<uint8_t>(header_remaining);
            memcpy(buffer + offset + 8, header_data.data(), std::min<size_t>(header_remaining, header_data.size()));
        }
    }
    
    static void generate_section_header(uint8_t* buffer, size_t buffer_size, size_t offset, FuzzedDataProvider& fdp, 
                                        uint32_t sh_type, uint32_t sh_name = 0) {
        // Section header is 11 uint32_t fields = 44 bytes total
        if (offset + 44 > buffer_size) return; // Safety check using actual buffer size
        
        // Write section header fields - use memcpy to avoid unaligned memory access
        // Write sh_name
        uint32_t sh_name_val = sh_name;
        memcpy(buffer + offset, &sh_name_val, sizeof(sh_name_val));
        
        // Write sh_type
        uint32_t sh_type_val = sh_type;
        memcpy(buffer + offset + 4, &sh_type_val, sizeof(sh_type_val));
        
        // sh_flags, sh_addr, sh_offset, sh_size (5 fields)
        for (int i = 2; i < 7; i++) {
            uint32_t val = fdp.ConsumeIntegral<uint32_t>();
            memcpy(buffer + offset + i * 4, &val, sizeof(val));
        }
        
        // sh_link, sh_info, sh_addralign, sh_entsize (4 fields)
        for (int i = 7; i < 11; i++) {
            uint32_t val = fdp.ConsumeIntegral<uint32_t>();
            memcpy(buffer + offset + i * 4, &val, sizeof(val));
        }
    }
    
    static void generate_note_section(uint8_t* buffer, size_t offset, FuzzedDataProvider& fdp,
                                      const char* name, uint32_t type, size_t max_size) {
        if (offset + 12 > max_size) return;
        
        uint32_t namesz = strlen(name) + 1; // Include null terminator
        uint32_t descsz = fdp.ConsumeIntegralInRange<uint32_t>(0, static_cast<uint32_t>(std::min<size_t>(256, max_size - offset - 12 - namesz)));
        
        // Write note header - use memcpy to avoid unaligned memory access
        // Write namesz
        uint32_t namesz_val = namesz;
        memcpy(buffer + offset, &namesz_val, sizeof(namesz_val));
        
        // Write descsz
        uint32_t descsz_val = descsz;
        memcpy(buffer + offset + 4, &descsz_val, sizeof(descsz_val));
        
        // Write type
        uint32_t type_val = type;
        memcpy(buffer + offset + 8, &type_val, sizeof(type_val));
        
        // Write note name
        size_t name_offset = offset + 12;
        memcpy(buffer + name_offset, name, strlen(name));
        buffer[name_offset + strlen(name)] = '\0';
        
        // Write note descriptor (if any)
        if (descsz > 0) {
            size_t desc_offset = name_offset + ((namesz + 3) & ~3); // Aligned
            auto desc_data = fdp.ConsumeBytes<uint8_t>(descsz);
            memcpy(buffer + desc_offset, desc_data.data(), std::min<size_t>(descsz, desc_data.size()));
        }
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful ELF testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags from input, include MAGIC_CONTINUE for multi-file analysis
    int flags = fdp.ConsumeIntegral<int>();
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME |
              MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
              MAGIC_EXTENSION | MAGIC_COMPRESS_TRANSP | MAGIC_NO_COMPRESS_FORK);
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        return 0;
    }
    
    // Load magic database
    int load_result = magic_load(magic, nullptr);
    if (load_result == -1) {
        // Try alternative paths
        const char* magic_paths[] = {"/etc/magic", "/usr/share/misc/magic"};
        for (const char* path : magic_paths) {
            load_result = magic_load(magic, path);
            if (load_result != -1) {
                break;
            }
        }
    }
    
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Generate ELF test data with various structures
    // Add input validation: ensure elf_size is within reasonable bounds
    const size_t MAX_ELF_SIZE = 65536;
    size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
    // Ensure elf_size doesn't exceed maximum reasonable size
    elf_size = std::min(elf_size, MAX_ELF_SIZE);
    std::vector<uint8_t> elf_buffer(elf_size, 0);
    
    // Generate ELF header
    ELFGenerator::generate_elf_header(elf_buffer.data(), elf_size, fdp);
    
    // Determine number of sections to generate
    size_t section_count = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    size_t section_offset = 64; // Start after header
    
    // Generate string table section for section names
    if (section_count > 0 && fdp.ConsumeBool()) {
        // Create .shstrtab string table
        const char* section_names[] = {".text", ".data", ".bss", ".rodata", 
                                      ".debug_info", ".debug_line", ".symtab",
                                      ".strtab", ".dynsym", ".dynstr", ".note",
                                      ".comment", ".interp"};
        
        size_t strtab_size = 1; // Start with null byte
        for (const auto& name : section_names) {
            strtab_size += strlen(name) + 1;
        }
        
        if (section_offset + 40 < elf_size) {
            ELFGenerator::generate_section_header(elf_buffer.data(), elf_size, section_offset, fdp, SHT_STRTAB);
            section_offset += 40;
            
            // Write string table data if space permits
            size_t strtab_data_offset = section_offset;
            elf_buffer[strtab_data_offset] = '\0'; // First entry is empty string
            size_t current_offset = 1;
            
            for (const auto& name : section_names) {
                size_t name_len = strlen(name);
                if (strtab_data_offset + current_offset + name_len + 1 < elf_size) {
                    memcpy(elf_buffer.data() + strtab_data_offset + current_offset, name, name_len);
                    elf_buffer[strtab_data_offset + current_offset + name_len] = '\0';
                    current_offset += name_len + 1;
                }
            }
            section_offset += current_offset;
        }
    }
    
    // Generate various section types to trigger different code paths
    for (size_t i = 0; i < section_count && section_offset + 40 < elf_size; i++) {
        uint32_t section_type = fdp.PickValueInArray({
            SHT_NULL, SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_RELA, 
            SHT_HASH, SHT_DYNAMIC, SHT_NOTE, SHT_NOBITS, SHT_REL, 
            SHT_SHLIB, SHT_DYNSYM
        });
        
        ELFGenerator::generate_section_header(elf_buffer.data(), elf_size, section_offset, fdp, section_type);
        section_offset += 40;
        
        // Add section content for certain types
        if (section_type == SHT_NOTE && fdp.ConsumeBool()) {
            // Generate note section with various note types
            uint32_t note_type = fdp.PickValueInArray({NT_GNU_BUILD_ID, NT_PRSTATUS, NT_PRFPREG, NT_PRPSINFO});
            const char* note_name = fdp.PickValueInArray({"GNU", "CORE", "NetBSD", "OpenBSD", "FreeBSD"});
            
            ELFGenerator::generate_note_section(elf_buffer.data(), section_offset, fdp, 
                                               note_name, note_type, elf_size - section_offset);
            section_offset += 128; // Advance past note section
        }
        
        // Add .debug_info section for debug detection path
        if (section_type == SHT_PROGBITS && fdp.ConsumeBool() && section_offset + 100 < elf_size) {
            const char debug_name[] = ".debug_info";
            size_t name_offset = section_offset;
            memcpy(elf_buffer.data() + name_offset, debug_name, sizeof(debug_name));
            section_offset += sizeof(debug_name);
        }
    }
    
    // Test edge cases: zero section headers, corrupted sizes, large offsets
    if (fdp.ConsumeBool()) {
        // Test with zero section headers
        if (section_offset + 4 < elf_size) {
            // Set e_shnum to 0 in header (offset depends on ELF class)
            if (elf_buffer[4] == ELFCLASS32) {
                // 32-bit ELF: e_shnum at offset 48
                if (48 + 2 <= elf_size) {
                    elf_buffer[48] = 0;
                    elf_buffer[49] = 0;
                }
            } else if (elf_buffer[4] == ELFCLASS64) {
                // 64-bit ELF: e_shnum at offset 60  
                if (60 + 2 <= elf_size) {
                    elf_buffer[60] = 0;
                    elf_buffer[61] = 0;
                }
            }
        }
    }
    
    if (fdp.ConsumeBool()) {
        // Test with corrupted section header size
        if (section_offset + 4 < elf_size) {
            // Set e_shentsize to invalid value
            if (elf_buffer[4] == ELFCLASS32 && 46 + 2 <= elf_size) {
                elf_buffer[46] = 0xFF; // Invalid size
                elf_buffer[47] = 0xFF;
            } else if (elf_buffer[4] == ELFCLASS64 && 58 + 2 <= elf_size) {
                elf_buffer[58] = 0xFF;
                elf_buffer[59] = 0xFF;
            }
        }
    }
    
    if (fdp.ConsumeBool()) {
        // Test with large offsets that might exceed file size
        for (size_t i = 64; i + 8 < elf_size && i < section_offset; i += 40) {
            if (fdp.ConsumeBool()) {
                // Set large offset in section header - use memcpy for unaligned access
                if (i + 16 < elf_size) {
                    uint32_t large_offset = 0xFFFFFFFF; // sh_offset
                    memcpy(elf_buffer.data() + i + 16, &large_offset, sizeof(large_offset));
                }
            }
        }
    }
    
    // Test magic_buffer with generated ELF data
    const char* buffer_result = magic_buffer(magic, elf_buffer.data(), elf_buffer.size());
    (void)buffer_result; // Use result to avoid unused variable warning
    
    // Check for errors
    const char* error_msg = magic_error(magic);
    (void)error_msg;
    
    // Test with MAGIC_NO_CHECK_ELF flag to see different behavior
    if (fdp.ConsumeBool()) {
        int current_flags = magic_getflags(magic);
        magic_setflags(magic, current_flags | MAGIC_NO_CHECK_ELF);
        
        const char* buffer_result2 = magic_buffer(magic, elf_buffer.data(), elf_buffer.size());
        (void)buffer_result2;
        
        magic_setflags(magic, current_flags); // Restore flags
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}


} // namespace harness_005
