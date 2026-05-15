#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "magic.h"

namespace harness_018 {
/*
 * Fuzzing harness for libmagic library - Advanced ELF Core Dump Processing with donote Function Targeting
 * Targets: dophn_core() and donote() functions in readelf.c with 639 undiscovered branches, 0% coverage
 * MODULE: ELF processing module (readelf.c)
 * STRATEGY: Target magic_buffer() API with diverse ELF core dump test data focusing on note sections
 * DIFFERENTIATION: Unlike harness_008 which focused on synthetic ELF structures, this harness uses:
 *   - Raw fuzzer input with strategic ELF core dump modifications
 *   - Focus on donote() function calls within dophn_core() for deeper coverage
 *   - Multiple ELF class variants (32-bit, 64-bit) with mixed endianness
 *   - Diverse note section types and structures
 *   - Edge case handling for corrupted/partial ELF headers
 * API SEQUENCE: magic_open() -> magic_buffer() with ELF core dump data -> magic_close()
 * Goal: Reach untouched donote() function paths that handle various note types in ELF core dumps
 */


// Include libmagic header

// ELF constants from readelf.h
#define EI_NIDENT 16
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ET_CORE 4
#define PT_NOTE 4

// ELF class constants
#define ELFCLASS32 1
#define ELFCLASS64 2

// ELF data encoding
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

// Note types for ELF core dumps
#define NT_PRSTATUS 1
#define NT_PRFPREG 2
#define NT_PRPSINFO 3
#define NT_PRXREG 4
#define NT_TASKSTRUCT 4
#define NT_PLATFORM 5
#define NT_AUXV 6
#define NT_FILE 0x46494c45  // "FILE" in hex
#define NT_SIGINFO 0x534947494e464f  // "SIGINFO" in hex

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic ELF header
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on ELF parsing flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for parser errors
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Strategy selection for ELF core dump generation
    // 0 = 32-bit ELF core with diverse note sections
    // 1 = 64-bit ELF core with mixed endianness  
    // 2 = Malformed ELF targeting error paths in dophn_core
    // 3 = Raw fuzzer input with strategic ELF modifications
    // 4 = Focus on donote() function with various note types
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    std::vector<uint8_t> elf_buffer;
    
    switch (strategy) {
        case 0: {
            // Strategy 0: 32-bit ELF core with diverse note sections
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
            if (fdp.remaining_bytes() < elf_size) {
                elf_size = fdp.remaining_bytes();
            }
            
            elf_buffer.resize(elf_size);
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Ensure ELF magic
            if (elf_size >= 4) {
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
            }
            
            // Set ELF class (32-bit)
            if (elf_size >= 4) {
                elf_buffer[4] = ELFCLASS32;
            }
            
            // Set data encoding
            if (elf_size >= 5) {
                elf_buffer[5] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
            }
            
            // Set ELF version
            if (elf_size >= 6) {
                elf_buffer[6] = 1;
            }
            
            // Set e_type to ET_CORE at offset 16
            if (elf_size >= 18) {
                elf_buffer[16] = ET_CORE;
                elf_buffer[17] = 0;
            }
            
            // Set e_phnum (number of program headers) - ensure at least 1
            if (elf_size >= 44) {
                elf_buffer[44] = 1;  // e_phnum LSB
                elf_buffer[45] = 0;  // e_phnum MSB
            }
            
            // Try to add PT_NOTE program headers if there's space
            if (elf_size >= 100) {
                // Create a simple note section to exercise donote()
                // Note header structure: namesz (4), descsz (4), type (4)
                uint32_t note_offset = 100;
                
                // Write note header
                if (elf_size >= note_offset + 12) {
                    // namesz = 4 (for "CORE" null-terminated)
                    elf_buffer[note_offset] = 4;
                    elf_buffer[note_offset + 1] = 0;
                    elf_buffer[note_offset + 2] = 0;
                    elf_buffer[note_offset + 3] = 0;
                    
                    // descsz = variable size based on remaining space
                    uint32_t desc_size = fdp.ConsumeIntegralInRange<uint32_t>(16, 64);
                    elf_buffer[note_offset + 4] = desc_size & 0xFF;
                    elf_buffer[note_offset + 5] = (desc_size >> 8) & 0xFF;
                    elf_buffer[note_offset + 6] = (desc_size >> 16) & 0xFF;
                    elf_buffer[note_offset + 7] = (desc_size >> 24) & 0xFF;
                    
                    // type = NT_PRSTATUS (1) or other note type
                    uint32_t note_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                    if (note_type == 0) note_type = NT_PRSTATUS;
                    else if (note_type == 1) note_type = NT_PRFPREG;
                    else if (note_type == 2) note_type = NT_PRPSINFO;
                    else if (note_type == 3) note_type = NT_AUXV;
                    else if (note_type == 4) note_type = NT_FILE & 0xFF;
                    else if (note_type == 5) note_type = (NT_FILE >> 8) & 0xFF;
                    else if (note_type == 6) note_type = (NT_FILE >> 16) & 0xFF;
                    else note_type = (NT_FILE >> 24) & 0xFF;
                    
                    elf_buffer[note_offset + 8] = note_type & 0xFF;
                    elf_buffer[note_offset + 9] = (note_type >> 8) & 0xFF;
                    elf_buffer[note_offset + 10] = (note_type >> 16) & 0xFF;
                    elf_buffer[note_offset + 11] = (note_type >> 24) & 0xFF;
                    
                    // Write note name "CORE"
                    if (elf_size >= note_offset + 16) {
                        elf_buffer[note_offset + 12] = 'C';
                        elf_buffer[note_offset + 13] = 'O';
                        elf_buffer[note_offset + 14] = 'R';
                        elf_buffer[note_offset + 15] = 'E';
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: 64-bit ELF core with mixed endianness
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(512, 4096);
            if (fdp.remaining_bytes() < elf_size) {
                elf_size = fdp.remaining_bytes();
            }
            
            elf_buffer.resize(elf_size);
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Set ELF magic
            if (elf_size >= 4) {
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
            }
            
            // Set ELF class (64-bit)
            if (elf_size >= 4) {
                elf_buffer[4] = ELFCLASS64;
            }
            
            // Randomly choose endianness
            if (elf_size >= 5) {
                uint8_t endian_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
                if (endian_choice == 0) elf_buffer[5] = ELFDATA2LSB;
                else if (endian_choice == 1) elf_buffer[5] = ELFDATA2MSB;
                // else leave as random from input
            }
            
            // Set e_type to ET_CORE for 64-bit ELF (offset 16)
            if (elf_size >= 18) {
                elf_buffer[16] = ET_CORE;
                elf_buffer[17] = 0;
            }
            
            // Create multiple note sections to exercise donote() loops
            if (elf_size >= 200) {
                size_t note_start = 200;
                uint8_t num_notes = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
                
                for (uint8_t i = 0; i < num_notes && (note_start + 32) < elf_size; i++) {
                    // Each note: header (12 bytes) + name (variable) + desc (variable)
                    size_t note_size = 12 + 4; // header + "CORE"
                    
                    if ((note_start + note_size) >= elf_size) break;
                    
                    // Write note header
                    // namesz = 4
                    elf_buffer[note_start] = 4;
                    elf_buffer[note_start + 1] = 0;
                    elf_buffer[note_start + 2] = 0;
                    elf_buffer[note_start + 3] = 0;
                    
                    // descsz = small random size
                    uint8_t desc_size = fdp.ConsumeIntegralInRange<uint8_t>(8, 32);
                    elf_buffer[note_start + 4] = desc_size;
                    elf_buffer[note_start + 5] = 0;
                    elf_buffer[note_start + 6] = 0;
                    elf_buffer[note_start + 7] = 0;
                    
                    // type = random note type
                    uint32_t note_type = 0;
                    uint8_t type_choice = fdp.ConsumeIntegral<uint8_t>() % 10;
                    switch (type_choice) {
                        case 0: note_type = NT_PRSTATUS; break;
                        case 1: note_type = NT_PRFPREG; break;
                        case 2: note_type = NT_PRPSINFO; break;
                        case 3: note_type = NT_AUXV; break;
                        case 4: note_type = NT_FILE & 0xFFFFFFFF; break;
                        case 5: note_type = 0x46494c45; // "FILE" in hex
                        case 6: note_type = 0x53494749; // "SIGI" part of SIGINFO
                        default: note_type = fdp.ConsumeIntegral<uint32_t>(); break;
                    }
                    
                    elf_buffer[note_start + 8] = note_type & 0xFF;
                    elf_buffer[note_start + 9] = (note_type >> 8) & 0xFF;
                    elf_buffer[note_start + 10] = (note_type >> 16) & 0xFF;
                    elf_buffer[note_start + 11] = (note_type >> 24) & 0xFF;
                    
                    // Write note name "CORE"
                    elf_buffer[note_start + 12] = 'C';
                    elf_buffer[note_start + 13] = 'O';
                    elf_buffer[note_start + 14] = 'R';
                    elf_buffer[note_start + 15] = 'E';
                    
                    note_start += note_size + desc_size;
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Malformed ELF targeting error paths in dophn_core
            // Create data that triggers error handling branches
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
            if (fdp.remaining_bytes() < elf_size) {
                elf_size = fdp.remaining_bytes();
            }
            
            elf_buffer.resize(elf_size);
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Set ELF magic to ensure it reaches ELF parser
            if (elf_size >= 4) {
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
            }
            
            // Create intentional errors:
            // 1. Set e_type to ET_CORE but with invalid program header count
            if (elf_size >= 18) {
                elf_buffer[16] = ET_CORE;
                elf_buffer[17] = 0;
            }
            
            // 2. Set e_phnum to 0 (no program headers) - should trigger "no program header" path
            if (elf_size >= 44) {
                elf_buffer[44] = 0;  // e_phnum = 0
                elf_buffer[45] = 0;
            }
            
            // 3. Or set extremely large e_phnum to trigger "too many" error
            if (fdp.ConsumeBool() && elf_size >= 44) {
                elf_buffer[44] = 0xFF;  // Large value
                elf_buffer[45] = 0xFF;
            }
            
            // 4. Corrupt e_phentsize (program header size)
            if (elf_size >= 42) {
                elf_buffer[42] = fdp.ConsumeIntegral<uint8_t>();  // Random size
                elf_buffer[43] = fdp.ConsumeIntegral<uint8_t>();
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Raw fuzzer input with strategic ELF modifications
            // Use most of input as-is but ensure it has ELF magic
            elf_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (elf_buffer.size() >= 4) {
                // Always set ELF magic
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
                
                // Randomly set e_type to ET_CORE if there's enough data
                if (elf_buffer.size() >= 18 && fdp.ConsumeBool()) {
                    elf_buffer[16] = ET_CORE;
                    elf_buffer[17] = 0;
                }
                
                // Try to create note-like structures in random positions
                size_t modifications = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                for (size_t i = 0; i < modifications && elf_buffer.size() >= 50; i++) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(20, elf_buffer.size() - 30);
                    
                    // Write "CORE" at random position
                    if (pos + 4 < elf_buffer.size()) {
                        elf_buffer[pos] = 'C';
                        elf_buffer[pos + 1] = 'O';
                        elf_buffer[pos + 2] = 'R';
                        elf_buffer[pos + 3] = 'E';
                    }
                    
                    // Try to create note header pattern
                    if (pos + 12 < elf_buffer.size()) {
                        // Set namesz to 4
                        elf_buffer[pos + 4] = 4;
                        elf_buffer[pos + 5] = 0;
                        
                        // Set descsz to something
                        elf_buffer[pos + 6] = 8;
                        elf_buffer[pos + 7] = 0;
                        
                        // Set type to something that might be recognized
                        elf_buffer[pos + 8] = NT_PRSTATUS;
                        elf_buffer[pos + 9] = 0;
                        elf_buffer[pos + 10] = 0;
                        elf_buffer[pos + 11] = 0;
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Strategy 4: Focus on donote() function with various note types
            // Build minimal ELF core structure focusing on note sections
            size_t total_size = fdp.ConsumeIntegralInRange<size_t>(300, 1500);
            if (fdp.remaining_bytes() < total_size) {
                total_size = fdp.remaining_bytes();
            }
            
            elf_buffer.resize(total_size);
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(total_size);
            memcpy(elf_buffer.data(), random_data.data(), total_size);
            
            // Build proper ELF header
            if (total_size >= 64) {
                // ELF magic
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
                
                // 32-bit ELF
                elf_buffer[4] = ELFCLASS32;
                
                // Little-endian
                elf_buffer[5] = ELFDATA2LSB;
                
                // Version 1
                elf_buffer[6] = 1;
                
                // ET_CORE type
                elf_buffer[16] = ET_CORE;
                elf_buffer[17] = 0;
                
                // Set e_phoff to point to program headers at offset 64
                if (total_size >= 28) {
                    uint32_t phoff = 64;
                    memcpy(&elf_buffer[28], &phoff, 4);
                }
                
                // Set e_phnum = 1
                if (total_size >= 44) {
                    elf_buffer[44] = 1;
                    elf_buffer[45] = 0;
                }
                
                // Set e_phentsize = 32 (size of program header)
                if (total_size >= 42) {
                    elf_buffer[42] = 32;
                    elf_buffer[43] = 0;
                }
                
                // Create PT_NOTE program header at offset 64
                if (total_size >= 96) {
                    // p_type = PT_NOTE (4)
                    elf_buffer[64] = 4;
                    elf_buffer[65] = 0;
                    elf_buffer[66] = 0;
                    elf_buffer[67] = 0;
                    
                    // p_offset = 128 (where notes start)
                    uint32_t note_offset = 128;
                    memcpy(&elf_buffer[68], &note_offset, 4);
                    
                    // p_filesz = size of note data (rest of buffer)
                    uint32_t filesz = total_size - note_offset;
                    if (filesz > 500) filesz = 500;
                    memcpy(&elf_buffer[80], &filesz, 4);
                    
                    // Now create diverse note sections starting at offset 128
                    size_t note_pos = 128;
                    uint8_t num_notes = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
                    
                    for (uint8_t i = 0; i < num_notes && (note_pos + 32) < total_size; i++) {
                        // Note header
                        // namesz = 4
                        elf_buffer[note_pos] = 4;
                        elf_buffer[note_pos + 1] = 0;
                        elf_buffer[note_pos + 2] = 0;
                        elf_buffer[note_pos + 3] = 0;
                        
                        // descsz = variable
                        uint16_t descsz = fdp.ConsumeIntegralInRange<uint16_t>(8, 64);
                        elf_buffer[note_pos + 4] = descsz & 0xFF;
                        elf_buffer[note_pos + 5] = (descsz >> 8) & 0xFF;
                        elf_buffer[note_pos + 6] = 0;
                        elf_buffer[note_pos + 7] = 0;
                        
                        // type = diverse note types
                        uint32_t note_type = 0;
                        switch (i % 6) {
                            case 0: note_type = NT_PRSTATUS; break;
                            case 1: note_type = NT_PRFPREG; break;
                            case 2: note_type = NT_PRPSINFO; break;
                            case 3: note_type = NT_AUXV; break;
                            case 4: note_type = 0x46494c45; // "FILE"
                            case 5: note_type = fdp.ConsumeIntegral<uint32_t>(); break;
                        }
                        
                        memcpy(&elf_buffer[note_pos + 8], &note_type, 4);
                        
                        // name = "CORE"
                        if (note_pos + 16 < total_size) {
                            elf_buffer[note_pos + 12] = 'C';
                            elf_buffer[note_pos + 13] = 'O';
                            elf_buffer[note_pos + 14] = 'R';
                            elf_buffer[note_pos + 15] = 'E';
                        }
                        
                        note_pos += 16 + descsz;
                    }
                }
            }
            break;
        }
    }
    
    // Test the generated ELF core dump with magic_buffer
    if (!elf_buffer.empty()) {
        const char* result = magic_buffer(magic, elf_buffer.data(), elf_buffer.size());
        // Result can be NULL - we're testing the parsing path, not the output
        (void)result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_018
