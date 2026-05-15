#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace harness_004 {
/*
 * libmagic fuzzing harness - Advanced ELF file parsing for readelf.c
 * Target: readelf.c module which has only 0.85% region coverage (1878 regions, 1862 missed)
 * 
 * This harness generates sophisticated ELF test cases beyond basic signatures:
 * 1) Different ELF types (ET_EXEC, ET_DYN, ET_CORE, ET_REL)
 * 2) Architectures (EM_X86_64, EM_386, EM_ARM, etc.)
 * 3) Endianness (ELFDATA2LSB, ELFDATA2MSB)
 * 4) ELF section headers, program headers
 * 5) Complex ELF structures that trigger deeper parsing paths in readelf.c
 * 
 * Semantically distinct from harness_003 which focused on basic ELF signatures and flags.
 * This harness creates complete ELF structures with varying complexity.
 */



// ELF constants from readelf.h
#define EI_NIDENT        16

// e_ident indices
#define EI_MAG0          0
#define EI_MAG1          1
#define EI_MAG2          2
#define EI_MAG3          3
#define EI_CLASS         4
#define EI_DATA          5
#define EI_VERSION       6
#define EI_PAD           7

// ELF magic
#define ELFMAG0          0x7f
#define ELFMAG1          'E'
#define ELFMAG2          'L'
#define ELFMAG3          'F'
#define OLFMAG1          'O'

// ELF class
#define ELFCLASSNONE     0
#define ELFCLASS32       1
#define ELFCLASS64       2

// ELF data encoding (endianness)
#define ELFDATANONE      0
#define ELFDATA2LSB      1
#define ELFDATA2MSB      2

// ELF types
#define ET_NONE          0
#define ET_REL           1
#define ET_EXEC          2
#define ET_DYN           3
#define ET_CORE          4

// Machine architectures
#define EM_NONE          0
#define EM_SPARC         2
#define EM_386           3
#define EM_SPARC32PLUS   18
#define EM_SPARCV9       43
#define EM_IA_64         50
#define EM_AMD64         62
#define EM_ARM           40
#define EM_AARCH64       183
#define EM_PPC           20
#define EM_PPC64         21
#define EM_MIPS          8
#define EM_RISCV         243

// Section header types
#define SHT_NULL         0
#define SHT_PROGBITS     1
#define SHT_SYMTAB       2
#define SHT_STRTAB       3
#define SHT_RELA         4
#define SHT_HASH         5
#define SHT_DYNAMIC      6
#define SHT_NOTE         7
#define SHT_NOBITS       8
#define SHT_REL          9
#define SHT_SHLIB        10
#define SHT_DYNSYM       11
#define SHT_SUNW_cap     0x6ffffff5

// Program header types
#define PT_NULL          0
#define PT_LOAD          1
#define PT_DYNAMIC       2
#define PT_INTERP        3
#define PT_NOTE          4
#define PT_SHLIB         5
#define PT_PHDR          6

// ELF version
#define EV_NONE          0
#define EV_CURRENT       1

// 32-bit ELF header structure
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf32_Ehdr;

// 64-bit ELF header structure
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

// 32-bit program header
typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} Elf32_Phdr;

// 64-bit program header
typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

// 32-bit section header
typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Shdr;

// 64-bit section header
typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} Elf64_Shdr;

// Create a minimal ELF file with configurable parameters
std::vector<uint8_t> create_elf_file(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> elf_data;
    
    // Determine ELF class (32-bit or 64-bit)
    int elf_class = fdp.ConsumeIntegralInRange<int>(0, 2);
    if (elf_class == 0) elf_class = ELFCLASS32;  // Default to 32-bit
    
    // Determine endianness
    int endianness = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Determine ELF type
    int elf_type = fdp.ConsumeIntegralInRange<int>(0, 8);
    if (elf_type > 4) elf_type = ET_CORE;  // Map higher values to valid types
    
    // Determine machine architecture
    static const int machines[] = {
        EM_NONE, EM_SPARC, EM_386, EM_SPARC32PLUS, EM_SPARCV9,
        EM_IA_64, EM_AMD64, EM_ARM, EM_AARCH64, EM_PPC, EM_PPC64,
        EM_MIPS, EM_RISCV
    };
    int machine = fdp.PickValueInArray(machines);
    
    // Determine number of program headers and section headers
    int phnum = fdp.ConsumeIntegralInRange<int>(0, 16);
    int shnum = fdp.ConsumeIntegralInRange<int>(0, 32);
    
    if (elf_class == ELFCLASS32) {
        // Create 32-bit ELF header
        Elf32_Ehdr ehdr;
        memset(&ehdr, 0, sizeof(ehdr));
        
        // Set e_ident
        ehdr.e_ident[EI_MAG0] = ELFMAG0;
        ehdr.e_ident[EI_MAG1] = ELFMAG1;
        ehdr.e_ident[EI_MAG2] = ELFMAG2;
        ehdr.e_ident[EI_MAG3] = ELFMAG3;
        ehdr.e_ident[EI_CLASS] = ELFCLASS32;
        ehdr.e_ident[EI_DATA] = endianness;
        ehdr.e_ident[EI_VERSION] = EV_CURRENT;
        
        // Set other header fields
        ehdr.e_type = elf_type;
        ehdr.e_machine = machine;
        ehdr.e_version = EV_CURRENT;
        ehdr.e_entry = fdp.ConsumeIntegral<uint32_t>();
        ehdr.e_phoff = sizeof(Elf32_Ehdr);  // Program headers immediately after ELF header
        ehdr.e_shoff = sizeof(Elf32_Ehdr) + phnum * sizeof(Elf32_Phdr);
        ehdr.e_flags = fdp.ConsumeIntegral<uint32_t>();
        ehdr.e_ehsize = sizeof(Elf32_Ehdr);
        ehdr.e_phentsize = sizeof(Elf32_Phdr);
        ehdr.e_phnum = phnum;
        ehdr.e_shentsize = sizeof(Elf32_Shdr);
        ehdr.e_shnum = shnum;
        ehdr.e_shstrndx = fdp.ConsumeIntegralInRange<uint16_t>(0, shnum > 0 ? shnum - 1 : 0);
        
        // Add ELF header to data
        uint8_t* ehdr_bytes = reinterpret_cast<uint8_t*>(&ehdr);
        elf_data.insert(elf_data.end(), ehdr_bytes, ehdr_bytes + sizeof(ehdr));
        
        // Add program headers
        for (int i = 0; i < phnum; i++) {
            Elf32_Phdr phdr;
            memset(&phdr, 0, sizeof(phdr));
            
            static const int ptypes[] = {PT_NULL, PT_LOAD, PT_DYNAMIC, PT_INTERP, PT_NOTE, PT_SHLIB, PT_PHDR};
            phdr.p_type = fdp.PickValueInArray(ptypes);
            phdr.p_offset = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_vaddr = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_paddr = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_filesz = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_memsz = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_flags = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_align = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
            
            uint8_t* phdr_bytes = reinterpret_cast<uint8_t*>(&phdr);
            elf_data.insert(elf_data.end(), phdr_bytes, phdr_bytes + sizeof(phdr));
        }
        
        // Add section headers
        for (int i = 0; i < shnum; i++) {
            Elf32_Shdr shdr;
            memset(&shdr, 0, sizeof(shdr));
            
            static const int stypes[] = {SHT_NULL, SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, 
                                       SHT_RELA, SHT_HASH, SHT_DYNAMIC, SHT_NOTE, SHT_NOBITS,
                                       SHT_REL, SHT_SHLIB, SHT_DYNSYM};
            shdr.sh_name = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_type = fdp.PickValueInArray(stypes);
            shdr.sh_flags = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_addr = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_offset = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_size = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_link = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_info = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_addralign = fdp.ConsumeIntegralInRange<uint32_t>(0, 4096);
            shdr.sh_entsize = fdp.ConsumeIntegral<uint32_t>();
            
            uint8_t* shdr_bytes = reinterpret_cast<uint8_t*>(&shdr);
            elf_data.insert(elf_data.end(), shdr_bytes, shdr_bytes + sizeof(shdr));
        }
        
    } else { // ELFCLASS64
        // Create 64-bit ELF header
        Elf64_Ehdr ehdr;
        memset(&ehdr, 0, sizeof(ehdr));
        
        // Set e_ident
        ehdr.e_ident[EI_MAG0] = ELFMAG0;
        ehdr.e_ident[EI_MAG1] = ELFMAG1;
        ehdr.e_ident[EI_MAG2] = ELFMAG2;
        ehdr.e_ident[EI_MAG3] = ELFMAG3;
        ehdr.e_ident[EI_CLASS] = ELFCLASS64;
        ehdr.e_ident[EI_DATA] = endianness;
        ehdr.e_ident[EI_VERSION] = EV_CURRENT;
        
        // Set other header fields
        ehdr.e_type = elf_type;
        ehdr.e_machine = machine;
        ehdr.e_version = EV_CURRENT;
        ehdr.e_entry = fdp.ConsumeIntegral<uint64_t>();
        ehdr.e_phoff = sizeof(Elf64_Ehdr);
        ehdr.e_shoff = sizeof(Elf64_Ehdr) + phnum * sizeof(Elf64_Phdr);
        ehdr.e_flags = fdp.ConsumeIntegral<uint32_t>();
        ehdr.e_ehsize = sizeof(Elf64_Ehdr);
        ehdr.e_phentsize = sizeof(Elf64_Phdr);
        ehdr.e_phnum = phnum;
        ehdr.e_shentsize = sizeof(Elf64_Shdr);
        ehdr.e_shnum = shnum;
        ehdr.e_shstrndx = fdp.ConsumeIntegralInRange<uint16_t>(0, shnum > 0 ? shnum - 1 : 0);
        
        // Add ELF header to data
        uint8_t* ehdr_bytes = reinterpret_cast<uint8_t*>(&ehdr);
        elf_data.insert(elf_data.end(), ehdr_bytes, ehdr_bytes + sizeof(ehdr));
        
        // Add program headers
        for (int i = 0; i < phnum; i++) {
            Elf64_Phdr phdr;
            memset(&phdr, 0, sizeof(phdr));
            
            static const int ptypes[] = {PT_NULL, PT_LOAD, PT_DYNAMIC, PT_INTERP, PT_NOTE, PT_SHLIB, PT_PHDR};
            phdr.p_type = fdp.PickValueInArray(ptypes);
            phdr.p_flags = fdp.ConsumeIntegral<uint32_t>();
            phdr.p_offset = fdp.ConsumeIntegral<uint64_t>();
            phdr.p_vaddr = fdp.ConsumeIntegral<uint64_t>();
            phdr.p_paddr = fdp.ConsumeIntegral<uint64_t>();
            phdr.p_filesz = fdp.ConsumeIntegral<uint64_t>();
            phdr.p_memsz = fdp.ConsumeIntegral<uint64_t>();
            phdr.p_align = fdp.ConsumeIntegralInRange<uint64_t>(0, 4096);
            
            uint8_t* phdr_bytes = reinterpret_cast<uint8_t*>(&phdr);
            elf_data.insert(elf_data.end(), phdr_bytes, phdr_bytes + sizeof(phdr));
        }
        
        // Add section headers
        for (int i = 0; i < shnum; i++) {
            Elf64_Shdr shdr;
            memset(&shdr, 0, sizeof(shdr));
            
            static const int stypes[] = {SHT_NULL, SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, 
                                       SHT_RELA, SHT_HASH, SHT_DYNAMIC, SHT_NOTE, SHT_NOBITS,
                                       SHT_REL, SHT_SHLIB, SHT_DYNSYM};
            shdr.sh_name = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_type = fdp.PickValueInArray(stypes);
            shdr.sh_flags = fdp.ConsumeIntegral<uint64_t>();
            shdr.sh_addr = fdp.ConsumeIntegral<uint64_t>();
            shdr.sh_offset = fdp.ConsumeIntegral<uint64_t>();
            shdr.sh_size = fdp.ConsumeIntegral<uint64_t>();
            shdr.sh_link = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_info = fdp.ConsumeIntegral<uint32_t>();
            shdr.sh_addralign = fdp.ConsumeIntegralInRange<uint64_t>(0, 4096);
            shdr.sh_entsize = fdp.ConsumeIntegral<uint64_t>();
            
            uint8_t* shdr_bytes = reinterpret_cast<uint8_t*>(&shdr);
            elf_data.insert(elf_data.end(), shdr_bytes, shdr_bytes + sizeof(shdr));
        }
    }
    
    // Add some section content if requested
    bool add_content = fdp.ConsumeBool();
    if (add_content && elf_data.size() < 1024) {
        size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::vector<uint8_t> content = fdp.ConsumeBytes<uint8_t>(content_size);
        elf_data.insert(elf_data.end(), content.begin(), content.end());
    }
    
    return elf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int flags = fdp.ConsumeIntegral<int>();
    bool create_custom_elf = fdp.ConsumeBool();
    
    // Mask flags to valid MAGIC_* values
    flags = flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
                     MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
                     MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
                     MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Initialize magic structure
    magic_t ms = magic_open(flags);
    if (ms == NULL) {
        return 0;
    }
    
    // Set parameters
    size_t indir_max = fdp.ConsumeIntegral<size_t>() % 21;  // 0-20
    size_t name_max = fdp.ConsumeIntegral<size_t>() % 101;  // 0-100
    magic_setparam(ms, MAGIC_PARAM_INDIR_MAX, &indir_max);
    magic_setparam(ms, MAGIC_PARAM_NAME_MAX, &name_max);
    // Try to compile if there's enough data
    if (fdp.remaining_bytes() > 100) {
        magic_compile(ms, NULL);
    }
    // Test with generated ELF file
    std::vector<uint8_t> elf_data;
    
    if (create_custom_elf) {
        // Create custom ELF file using fuzzer input
        elf_data = create_elf_file(fdp);
    } else {
        // Use remaining input as ELF-like data
        elf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Ensure it has minimal ELF signature
        if (elf_data.size() >= 16) {
            elf_data[EI_MAG0] = ELFMAG0;
            elf_data[EI_MAG1] = ELFMAG1;
            elf_data[EI_MAG2] = ELFMAG2;
            elf_data[EI_MAG3] = ELFMAG3;
        }
    }
    
    // Test the ELF data
    if (!elf_data.empty()) {
        // Use magic_buffer to test the ELF data
        const char* result = magic_buffer(ms, elf_data.data(), elf_data.size());
        // Result is not used, but the parsing happens internally in readelf.c
    }
    
    // Test with file descriptor if we have enough data
    if (elf_data.size() > 100) {
        // Create temporary file
        char filename[] = "/tmp/fuzz_elf_XXXXXX";
        int fd = mkstemp(filename);
        if (fd >= 0) {
            write(fd, elf_data.data(), elf_data.size());
            lseek(fd, 0, SEEK_SET);
            
            // Test with file descriptor
            const char* result = magic_descriptor(ms, fd);
            // Result is not used
            
            close(fd);
            unlink(filename);
        }
    }
    
    // Clean up
    magic_close(ms);
    
    return 0;
}

} // namespace harness_004
