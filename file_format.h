#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdint.h>
#include <stdbool.h>

/*==================================================
 *                                                 *
 *                                                 *
 *                ELF Format Begin                 *
 *                                                 *
 *                                                 *
 *================================================*/

typedef struct {
	unsigned char	ident[16];	/* Magic number and other info */
	uint16_t	type;		/* Object file type */
	uint16_t	isa;		/* Architecture */
	uint32_t	version;	/* Object file version */
	uint32_t	entry;		/* Entry point virtual address */
	uint32_t	phoff;		/* Program header table file offset */
	uint32_t	shoff;		/* Section header table file offset */
	uint32_t	flags;		/* Processor-specific flags */
	uint16_t	ehsize;	/* ELF header size in bytes */
	uint16_t	phentsize;	/* Program header table entry size */
	uint16_t	phnum;		/* Program header table entry count */
	uint16_t	shentsize;	/* Section header table entry size */
	uint16_t	shnum;		/* Section header table entry count */
	uint16_t	shstrndx;	/* Section header string table index */
} elf32_header;

typedef struct {
	unsigned char	ident[16];	/* Magic number and other info */
	uint16_t	type;		/* Object file type */
	uint16_t	isa;		/* Architecture */
	uint32_t	version;	/* Object file version */
	uint64_t	entry;		/* Entry point virtual address */
	uint64_t	phoff;		/* Program header table file offset */
	uint64_t	shoff;		/* Section header table file offset */
	uint32_t	flags;		/* Processor-specific flags */
	uint16_t	ehsize;	/* ELF header size in bytes */
	uint16_t	phentsize;	/* Program header table entry size */
	uint16_t	phnum;		/* Program header table entry count */
	uint16_t	shentsize;	/* Section header table entry size */
	uint16_t	shnum;		/* Section header table entry count */
	uint16_t	shstrndx;	/* Section header string table index */
} elf64_header;

/*
 * These will get quite big once we start to have other formats, so it might
 * might be a good idea to have disable directive for people who are using
 * this library at a limited resource machine
 */
#ifndef FILE_FORMAT_ELF_DISABLE_MAP_STR_STRUCT
static struct {
	unsigned char	ident[16];
	char*	type[10];
	char*	isa[79];
	uint32_t	version;
	uint32_t	entry;
	uint32_t	phoff;
	uint32_t	shoff;
	uint32_t	flags;
	uint16_t	ehsize;
	uint16_t	phentsize;
	uint16_t	phnum;
	uint16_t	shentsize;
	uint16_t	shnum;
	uint16_t	shstrndx;
} elf_map_str = {

	/* File type */
	.type[0] = "Unknwon type",
	.type[1] = "Relocatable file",
	.type[2] = "Executable file",
	.type[3] = "Shared object",
	.type[4] = "Core file",
	.type[5] = "", /* Reserved include range. Operating system specific */
	.type[6] = "", /* Reserved include range. Operating system specific */
	.type[7] = "", /* Reserved include range. Operating system specific */
	.type[8] = "", /* Reserved include range. Operating system specific */
	.type[9] = "", /* Reserved include range. Operating system specific */

	/* ISA */
	.isa[0] = "No specific instruction set",
	.isa[1] = "AT&T WE 32100",
	.isa[2] = "SPARC",
	.isa[3] = "x86",
	.isa[4] = "Motorola 68000 (M68k)",
	.isa[5] = "Motorola 88000 (M88k)",
	.isa[6] = "Intel MCU",
	.isa[7] = "Intel 80860",
	.isa[8] = "MIPS",
	.isa[9] = "IBM System/370",
	.isa[10] = "MIPS RS3000 Little-endian",
	.isa[11] = "future use ",
	.isa[12] = "Hewlett-Packard PA-RISC",
	.isa[13] = "future use",
	.isa[14] = "Intel 80960",
	.isa[15] = "PowerPC",
	.isa[16] = "PowerPC (64-bit)",
	.isa[17] = "S390, including S390x",
	.isa[18] = "IBM SPU/SPC",
	.isa[19] = "future use",
	.isa[20] = "NEC V800",
	.isa[21] = "Fujitsu FR20",
	.isa[22] = "TRW RH-32",
	.isa[23] = "Motorola RCE",
	.isa[24] = "Arm (up to Armv7/AArch32)",
	.isa[25] = "Digital Alpha",
	.isa[26] = "SuperH",
	.isa[27] = "SPARC Version 9",
	.isa[28] = "Siemens TriCore embedded processor",
	.isa[29] = "Argonaut RISC Core",
	.isa[30] = "Hitachi H8/300",
	.isa[31] = "Hitachi H8/300H",
	.isa[32] = "Hitachi H8S",
	.isa[33] = "Hitachi H8/500",
	.isa[34] = "IA-64",
	.isa[35] = "Stanford MIPS-X",
	.isa[36] = "Motorola ColdFire",
	.isa[37] = "Motorola M68HC12",
	.isa[38] = "Fujitsu MMA Multimedia Accelerator",
	.isa[39] = "Siemens PCP",
	.isa[40] = "Sony nCPU embedded RISC processor",
	.isa[41] = "Denso NDR1 microprocessor",
	.isa[42] = "Motorola Star*Core processor",
	.isa[43] = "Toyota ME16 processor",
	.isa[44] = "STMicroelectronics ST100 processor",
	.isa[45] = "Advanced Logic Corp. TinyJ embedded processor family",
	.isa[46] = "AMD x86-64",
	.isa[47] = "Sony DSP Processor",
	.isa[48] = "Digital Equipment Corp. PDP-10",
	.isa[49] = "Digital Equipment Corp. PDP-11",
	.isa[50] = "Siemens FX66 microcontroller",
	.isa[51] = "STMicroelectronics ST9+ 8/16 bit microcontroller",
	.isa[52] = "STMicroelectronics ST7 8-bit microcontroller",
	.isa[53] = "Motorola MC68HC16 Microcontroller",
	.isa[54] = "Motorola MC68HC11 Microcontroller",
	.isa[55] = "Motorola MC68HC08 Microcontroller",
	.isa[56] = "Motorola MC68HC05 Microcontroller",
	.isa[57] = "Silicon Graphics SVx",
	.isa[58] = "STMicroelectronics ST19 8-bit microcontroller",
	.isa[59] = "Digital VAX",
	.isa[60] = "Axis Communications 32-bit embedded processor",
	.isa[61] = "Infineon Technologies 32-bit embedded processor",
	.isa[62] = "Element 14 64-bit DSP Processor",
	.isa[63] = "LSI Logic 16-bit DSP Processor",
	.isa[64] = "future use",
	.isa[65] = "TMS320C6000 Family",
	.isa[66] = "future use",
	.isa[67] = "MCST Elbrus e2k",
	.isa[68] = "future use",
	.isa[69] = "Arm 64-bits (Armv8/AArch64)",
	.isa[70] = "future use",
	.isa[71] = "Zilog Z80",
	.isa[72] = "future use",
	.isa[73] = "RISC-V",
	.isa[74] = "future use",
	.isa[75] = "Berkeley Packet Filter",
	.isa[76] = "future use",
	.isa[77] = "WDC 65C816",
	.isa[78] = "or future use",
};
#endif /* FILE_FORMAT_ELF_DISABLE_MAP_STR_STRUCT */

enum elf_index{
	ELF_INDEX_ARCH = 4,
	ELF_INDEX_ENDIAN = 5,
	ELF_INDEX_VERSION = 6,
	ELF_INDEX_ABI = 7,
	ELF_INDEX_ABIVER = 8,
};

enum elf_arch {
	ELF_X32,
	ELF_X64,
};

enum elf_endian{
	ELF_LITTLE_ENDIAN,
	ELF_BIG_ENDIAN,
};
enum elf_ident_ver {
	ELF_IDENT_VER_1,
};

enum elf_abi {
	ELF_SYSTEMV = 0,
	FILE_FORMAT_HPUX,
	ELF_NETBSD,
	ELF_LINUX,
	ELF_GNUHURD,
	ELF_SOLARIS,
	ELF_AIX,
	ELF_IRIX,
	ELF_FREEBSD,
	ELF_TRU64,
	ELF_NOVELLMODESTO,
	ELF_OPENBSD,
	ELF_OPENVMS,
	ELF_NONSTOPKERNEL,
	ELF_AROS,
	ELF_FENIXOS,
	ELF_NUXI,
	ELF_STRATUS_OPENVOS
};

enum elf_abiver {
	ELF_UNKNOWN = 0x00000000,
	ELF_VER1 = 0x01000000,
	ELF_VER2 = 0x02000000,
	ELF_VER3 = 0x03000000,
	ELF_VER4 = 0x04000000,
	ELF_VER5 = 0x05000000,
};

enum elf_type {
	ELF_TYPE_NONE = 0,		/* Unknown */
	ELF_TYPE_REL = 1,		/* Relocatable file */
	ELF_TYPE_EXEC = 2,		/* Executable file */
	ELF_TYPE_DYN = 3,		/* Shared object */
	ELF_TYPE_CORE = 4,		/* Core file */
	ELF_TYPE_NUM = 5,		/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_LOOS = 0xfe00,	/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_HIOS = 0xfeff,	/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_LOPROC = 0xff00,	/* Reserved inclusive range. Processor specific */
	ELF_TYPE_HIPROC = 0xffff,	/* Reserved inclusive range. Processor specific */
};

enum elf_isa {
	ELF_ISA_UNKNOWN              = 0x00, /* No specific instruction set */
	ELF_ISA_ATAT32100            = 0x01, /* AT&T WE 32100 */
	ELF_ISA_SPARC                = 0x02, /* SPARC */
	ELF_ISA_X86                  = 0x03, /* x86 */
	ELF_ISA_MOTOROLA_680000      = 0x04, /* Motorola 68000 (M68k) */
	ELF_ISA_MOTOROLA_88000       = 0x05, /* Motorola 88000 (M88k) */
	ELF_ISA_INTEL_MCU            = 0x06, /* Intel MCU */
	ELF_ISA_INTEL_80860          = 0x07, /* Intel 80860 */
	ELF_ISA_MIPS                 = 0x08, /* MIPS */
	ELF_ISA_IBM_SYSTEM_370       = 0x09, /* IBM System/370 */
	ELF_ISA_MIPS_RS3000_LE       = 0x0A, /* MIPS RS3000 Little-endian */
	/* ELF_ISA 0x0B - 0x0E: Reserved for future use */
	ELF_ISA_HP_PA_RISC           = 0x0F, /* Hewlett-Packard PA-RISC */
	/* ELF_ISA 0x10 - 0x12: Reserved for future use */
	ELF_ISA_INTEL_80960          = 0x13, /* Intel 80960 */
	ELF_ISA_POWERPC              = 0x14, /* PowerPC */
	ELF_ISA_POWERPC_64           = 0x15, /* PowerPC (64-bit) */
	ELF_ISA_S390                 = 0x16, /* S390, including S390x */
	ELF_ISA_IBM_SPU_SPC          = 0x17, /* IBM SPU/SPC */
	/* ELF_ISA 0x18 - 0x23: Reserved for future use */
	ELF_ISA_NEC_V800             = 0x24, /* NEC V800 */
	ELF_ISA_FUJITSU_FR20         = 0x25, /* Fujitsu FR20 */
	ELF_ISA_TRW_RH32             = 0x26, /* TRW RH-32 */
	ELF_ISA_MOTOROLA_RCE         = 0x27, /* Motorola RCE */
	ELF_ISA_ARM                  = 0x28, /* Arm (up to Armv7/AArch32) */
	ELF_ISA_DIGITAL_ALPHA        = 0x29, /* Digital Alpha */
	ELF_ISA_SUPERH               = 0x2A, /* SuperH */
	ELF_ISA_SPARC_V9             = 0x2B, /* SPARC Version 9 */
	ELF_ISA_SIEMENS_TRICORE      = 0x2C, /* Siemens TriCore embedded processor */
	ELF_ISA_ARGONAUT_RISC_CORE   = 0x2D, /* Argonaut RISC Core */
	ELF_ISA_HITACHI_H8300        = 0x2E, /* Hitachi H8/300 */
	ELF_ISA_HITACHI_H8300H       = 0x2F, /* Hitachi H8/300H */
	ELF_ISA_HITACHI_H8300S       = 0x30, /* Hitachi H8S */
	ELF_ISA_HITACHI_H8500        = 0x31, /* Hitachi H8/500 */
	ELF_ISA_IA_64                = 0x32, /* IA-64 */
	ELF_ISA_STANFORD_MIPS_X      = 0x33, /* Stanford MIPS-X */
	ELF_ISA_MOTOROLA_COLDFIRE    = 0x34, /* Motorola ColdFire */
	ELF_ISA_MOTOROLA_M68HC12     = 0x35, /* Motorola M68HC12 */
	ELF_ISA_FUJITSU_MMA          = 0x36, /* Fujitsu MMA Multimedia Accelerator */
	ELF_ISA_SIEMENS_PCP          = 0x37, /* Siemens PCP */
	ELF_ISA_SONY_NCPU            = 0x38, /* Sony nCPU embedded RISC processor */
	ELF_ISA_DENSO_NDR1           = 0x39, /* Denso NDR1 microprocessor */
	ELF_ISA_MOTOROLA_STARCORE    = 0x3A, /* Motorola Star*Core processor */
	ELF_ISA_TOYOTA_ME16          = 0x3B, /* Toyota ME16 processor */
	ELF_ISA_STM_ST100            = 0x3C, /* STMicroelectronics ST100 processor */
	ELF_ISA_ALC_TINYJ            = 0x3D, /* Advanced Logic Corp. TinyJ embedded processor family */
	ELF_ISA_AMD_X86_64           = 0x3E, /* AMD x86-64 */
	ELF_ISA_SONY_DSP             = 0x3F, /* Sony DSP Processor */
	ELF_ISA_DIGITAL_PDP_10       = 0x40, /* Digital Equipment Corp. PDP-10 */
	ELF_ISA_DIGITAL_PDP_11       = 0x41, /* Digital Equipment Corp. PDP-11 */
	ELF_ISA_SIEMENS_FX66         = 0x42, /* Siemens FX66 microcontroller */
	ELF_ISA_STM_ST9P             = 0x43, /* STMicroelectronics ST9+ 8/16 bit microcontroller */
	ELF_ISA_STM_ST7              = 0x44, /* STMicroelectronics ST7 8-bit microcontroller */
	ELF_ISA_MOTOROLA_MC68HC16    = 0x45, /* Motorola MC68HC16 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC11    = 0x46, /* Motorola MC68HC11 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC08    = 0x47, /* Motorola MC68HC08 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC05    = 0x48, /* Motorola MC68HC05 Microcontroller */
	ELF_ISA_SGI_SVX              = 0x49, /* Silicon Graphics SVx */
	ELF_ISA_STM_ST19             = 0x4A, /* STMicroelectronics ST19 8-bit microcontroller */
	ELF_ISA_DIGITAL_VAX          = 0x4B, /* Digital VAX */
	ELF_ISA_AXIS_32_BIT          = 0x4C, /* Axis Communications 32-bit embedded processor */
	ELF_ISA_INFINEON_32_BIT      = 0x4D, /* Infineon Technologies 32-bit embedded processor */
	ELF_ISA_ELEMENT14_DSP        = 0x4E, /* Element 14 64-bit DSP Processor */
	ELF_ISA_LSI_LOGIC_DSP        = 0x4F, /* LSI Logic 16-bit DSP Processor */
	/* ELF_ISA 0x50 - 0x8B: Reserved for future use */
	ELF_ISA_TI_C6000             = 0x8C, /* TMS320C6000 Family */
	/* ELF_ISA 0x8D - 0xAE: Reserved for future use */
	ELF_ISA_MCST_ELBRUS_E2K      = 0xAF, /* MCST Elbrus e2k */
	/* ELF_ISA 0xB0 - 0xB6: Reserved for future use */
	ELF_ISA_ARM_64               = 0xB7, /* Arm 64-bits (Armv8/AArch64) */
	/* ELF_ISA 0xB8 - 0xDB: Reserved for future use */
	ELF_ISA_ZILOG_Z80            = 0xDC, /* Zilog Z80 */
	/* ELF_ISA 0xDD - 0xF2: Reserved for future use */
	ELF_ISA_RISC_V               = 0xF3, /* RISC-V */
	/* ELF_ISA 0xF4 - 0xF6: Reserved for future use */
	ELF_ISA_BPF                  = 0xF7, /* Berkeley Packet Filter */
	/* ELF_ISA 0xF8 - 0xFE: Reserved for future use */
	ELF_ISA_WDC_65C816           = 0x101 /* WDC 65C816 */
	/* ELF_ISA 0x102 - 0xFFFF: Reserved for future use */
};

enum elf_version {
	ELF_VER_1,
};

enum header_type {
	HEADER_FORMAT_ELF,
	HEADER_FORMAT_PE32,
	HEADER_FORMAT_UNKNOWN,
};
void elf32_swap_bytes(elf32_header *header);
void elf64_swap_bytes(elf32_header *header);

/*==================================================
 *                                                 *
 *                                                 *
 *                PE32 Format Begin                *
 *                                                 *
 *                                                 *
 *================================================*/

typedef struct {
	uint32_t magic;
	uint16_t machine;
	uint16_t number_of_sections;
	uint32_t time_date_stamp;
	uint32_t pointer_to_symbol_table; /* This should be 0 for image because COFF debug info is deprecated */
	uint32_t number_of_symbols;       /* This should be 0 for image because COFF debug info is deprecated */
	uint16_t size_of_optional_header;
	uint16_t characteristics;
} pe32_header;

typedef struct {
	uint16_t ident; /* 0x010b - PE32, 0x020b - PE32+ (64 bit) */
	uint8_t  major_linker_ver;
	uint8_t  minor_linker_ver;
	uint32_t code_size;
	uint32_t initialized_data_size;
	uint32_t uninitialized_data_size;
	uint32_t entry_point_address;
	uint32_t base_of_code;
	uint32_t base_of_data;
	uint32_t image_base;
	uint32_t section_alignement;
	uint32_t file_alignement;
	uint16_t major_operating_system_ver;
	uint16_t minor_operating_system_ver;
	uint16_t major_image_ver;
	uint16_t minor_image_ver;
	uint16_t major_subsystem_ver;
	uint16_t minor_subsystem_ver;
	uint32_t win32_ver_value;
	uint32_t sizeof_image;
	uint32_t sizeof_headers;
	uint32_t checksum;
	uint16_t subsystem;
	uint16_t dll_characteristics;
	uint32_t sizeof_stack_reserve;
	uint32_t sizeof_stack_commit;
	uint32_t sizeof_heap_reserve;
	uint32_t sizeof_heap_commit;
	uint32_t laoder_flags;
	uint32_t numof_rva_and_sizes;
} pe32_optionalheader;

enum pe32_machine_type {
	PE32_MACHINE_TYPE_UNKNOWN    = 0x0,    /* The content of this field is assumed to be applicable to any machine type */
	PE32_MACHINE_TYPE_ALPHA      = 0x184,  /* Alpha AXP, 32-bit address space */
	PE32_MACHINE_TYPE_ALPHA64    = 0x284,  /* Alpha 64, 64-bit address space */
	PE32_MACHINE_TYPE_AM33       = 0x1d3,  /* Matsushita AM33 */
	PE32_MACHINE_TYPE_AMD64      = 0x8664, /* x64 */
	PE32_MACHINE_TYPE_ARM        = 0x1c0,  /* ARM little endian */
	PE32_MACHINE_TYPE_ARM64      = 0xaa64, /* ARM64 little endian */
	PE32_MACHINE_TYPE_ARMNT      = 0x1c4,  /* ARM Thumb-2 little endian */
	PE32_MACHINE_TYPE_AXP64      = 0x284,  /* AXP 64 (Same as Alpha 64) */
	PE32_MACHINE_TYPE_EBC        = 0xebc,  /* EFI byte code */
	PE32_MACHINE_TYPE_I386       = 0x14c,  /* Intel 386 or later processors and compatible processors */
	PE32_MACHINE_TYPE_IA64       = 0x200,  /* Intel Itanium processor family */
	PE32_MACHINE_TYPE_LOONGARCH32 = 0x6232,/* LoongArch 32-bit processor family */
	PE32_MACHINE_TYPE_LOONGARCH64 = 0x6264,/* LoongArch 64-bit processor family */
	PE32_MACHINE_TYPE_M32R       = 0x9041, /* Mitsubishi M32R little endian */
	PE32_MACHINE_TYPE_MIPS16     = 0x266,  /* MIPS16 */
	PE32_MACHINE_TYPE_MIPSFPU    = 0x366,  /* MIPS with FPU */
	PE32_MACHINE_TYPE_MIPSFPU16  = 0x466,  /* MIPS16 with FPU */
	PE32_MACHINE_TYPE_POWERPC    = 0x1f0,  /* Power PC little endian */
	PE32_MACHINE_TYPE_POWERPCFP  = 0x1f1,  /* Power PC with floating point support */
	PE32_MACHINE_TYPE_R4000      = 0x166,  /* MIPS little endian */
	PE32_MACHINE_TYPE_RISCV32    = 0x5032, /* RISC-V 32-bit address space */
	PE32_MACHINE_TYPE_RISCV64    = 0x5064, /* RISC-V 64-bit address space */
	PE32_MACHINE_TYPE_RISCV128   = 0x5128, /* RISC-V 128-bit address space */
	PE32_MACHINE_TYPE_SH3        = 0x1a2,  /* Hitachi SH3 */
	PE32_MACHINE_TYPE_SH3DSP     = 0x1a3,  /* Hitachi SH3 DSP */
	PE32_MACHINE_TYPE_SH4        = 0x1a6,  /* Hitachi SH4 */
	PE32_MACHINE_TYPE_SH5        = 0x1a8,  /* Hitachi SH5 */
	PE32_MACHINE_TYPE_THUMB      = 0x1c2,  /* Thumb */
	PE32_MACHINE_TYPE_WCEMIPSV2  = 0x169   /* MIPS little-endian WCE v2 */
};

struct pe32_machine_type_str_map {
    enum pe32_machine_type machine_type;
    const char* str;
};

static struct pe32_machine_type_str_map pe32_machine_type_map[] = {
	{ PE32_MACHINE_TYPE_UNKNOWN, "Unknown machine type" },
	{ PE32_MACHINE_TYPE_ALPHA, "Alpha AXP, 32-bit address space" },
	{ PE32_MACHINE_TYPE_ALPHA64, "Alpha 64, 64-bit address space" },
	{ PE32_MACHINE_TYPE_AM33, "Matsushita AM33" },
	{ PE32_MACHINE_TYPE_AMD64, "x64" },
	{ PE32_MACHINE_TYPE_ARM, "ARM little endian" },
	{ PE32_MACHINE_TYPE_ARM64, "ARM64 little endian" },
	{ PE32_MACHINE_TYPE_ARMNT, "ARM Thumb-2 little endian" },
	{ PE32_MACHINE_TYPE_AXP64, "AXP 64 (Same as Alpha 64)" },
	{ PE32_MACHINE_TYPE_EBC, "EFI byte code" },
	{ PE32_MACHINE_TYPE_I386, "Intel 386 or later processors and compatible processor" },
	{ PE32_MACHINE_TYPE_IA64, "Intel Itanium processor family" },
	{ PE32_MACHINE_TYPE_LOONGARCH32, "LoongArch 32-bit processor family" },
	{ PE32_MACHINE_TYPE_LOONGARCH64, "LoongArch 64-bit processor family" },
	{ PE32_MACHINE_TYPE_M32R, "Mitsubishi M32R little endian" },
	{ PE32_MACHINE_TYPE_MIPS16, "MIPS16" },
	{ PE32_MACHINE_TYPE_MIPSFPU, "MIPS with FPU" },
	{ PE32_MACHINE_TYPE_MIPSFPU16, "MIPS16 with FPU" },
	{ PE32_MACHINE_TYPE_POWERPC, "Power PC little endian" },
	{ PE32_MACHINE_TYPE_POWERPCFP, "Power PC with floating point support" },
	{ PE32_MACHINE_TYPE_R4000, "MIPS little endian" },
	{ PE32_MACHINE_TYPE_RISCV32, "RISC-V 32-bit address space" },
	{ PE32_MACHINE_TYPE_RISCV64, "RISC-V 64-bit address space" },
	{ PE32_MACHINE_TYPE_RISCV128, "RISC-V 128-bit address space" },
	{ PE32_MACHINE_TYPE_SH3, "Hitachi SH3" },
	{ PE32_MACHINE_TYPE_SH3DSP, "Hitachi SH3 DSP" },
	{ PE32_MACHINE_TYPE_SH4, "Hitachi SH4" },
	{ PE32_MACHINE_TYPE_SH5, "Hitachi SH5" },
	{ PE32_MACHINE_TYPE_THUMB, "Thumb" },
	{ PE32_MACHINE_TYPE_WCEMIPSV2, "MIPS little-endian WCE v2" },
};

enum pe32_image_characteristics {
	IMAGE_FILE_RELOCS_STRIPPED       = 0x0001, /* File does not contain base relocations. */
	IMAGE_FILE_EXECUTABLE_IMAGE      = 0x0002, /* File is valid and can be run. */
	IMAGE_FILE_LINE_NUMS_STRIPPED    = 0x0004, /* Deprecated: COFF line numbers have been removed. */
	IMAGE_FILE_LOCAL_SYMS_STRIPPED   = 0x0008, /* Deprecated: COFF symbol table entries for local symbols have been removed. */
	IMAGE_FILE_AGGRESSIVE_WS_TRIM    = 0x0010, /* Obsolete: Aggressively trim working set. */
	IMAGE_FILE_LARGE_ADDRESS_AWARE   = 0x0020, /* Application can handle > 2-GB addresses. */
	IMAGE_FILE_RESERVED_0040         = 0x0040, /* Reserved for future use. */
	IMAGE_FILE_BYTES_REVERSED_LO     = 0x0080, /* Deprecated: Little endian - the least significant bit (LSB) precedes the most significant bit (MSB) in memory. */
	IMAGE_FILE_32BIT_MACHINE         = 0x0100, /* Machine is based on a 32-bit-word architecture. */
	IMAGE_FILE_DEBUG_STRIPPED        = 0x0200, /* Debugging information is removed from the image file. */
	IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP = 0x0400, /* If the image is on removable media, fully load it and copy it to the swap file. */
	IMAGE_FILE_NET_RUN_FROM_SWAP     = 0x0800, /* If the image is on network media, fully load it and copy it to the swap file. */
	IMAGE_FILE_SYSTEM               = 0x1000, /* The image file is a system file, not a user program. */
	IMAGE_FILE_DLL                  = 0x2000, /* The image file is a dynamic-link library (DLL). Such files are considered executable files for almost all purposes, although they cannot be directly run. */
	IMAGE_FILE_UP_SYSTEM_ONLY       = 0x4000, /* The file should be run only on a uniprocessor machine. */
	IMAGE_FILE_BYTES_REVERSED_HI    = 0x8000  /* Deprecated: Big endian - the MSB precedes the LSB in memory. */
};

#define FILE_FORMAT_PE32_SIGNATURE_EXISTS 0x00004550

int pe32_does_signature_exist(char *data);
const char* pe32_get_machine_type_string(enum pe32_machine_type machine_type);
int pe32_is_flag_set(uint16_t characteristics, enum pe32_image_characteristics bit);

enum pe32_index {
	tmp,
};


int determine_link_format(char *data);

#endif /* FILE_FORMAT_H */

#ifdef FILE_FORMAT_IMPLEMENTATION

int __swap_uint16(uint16_t byte)
{
	return ((byte>>8) | (byte<<8));
}

int __swap_uint32(uint32_t byte)
{
	return ((byte>>24)&0xff) | ((byte<<8)&0xff0000) |
		((byte>>8)&0xff00) | ((byte<<24)&0xff000000);
}

uint64_t __swap_uint64(uint64_t byte) {
	return ((byte >> 56) & 0x00000000000000FF) |
		((byte >> 40) & 0x000000000000FF00) |
		((byte >> 24) & 0x0000000000FF0000) |
		((byte >> 8)  & 0x00000000FF000000) |
		((byte << 8)  & 0x000000FF00000000) |
		((byte << 24) & 0x0000FF0000000000) |
		((byte << 40) & 0x00FF000000000000) |
		((byte << 56) & 0xFF00000000000000);
}

void elf32_swap_bytes(elf32_header *header)
{
	header->type = __swap_uint16(header->type);
	header->isa = __swap_uint16(header->isa);
	header->version = __swap_uint32(header->version);
	header->entry = __swap_uint32(header->entry);
	header->phoff = __swap_uint32(header->phoff);
	header->shoff = __swap_uint32(header->shoff);
	header->flags = __swap_uint32(header->flags);
	header->ehsize = __swap_uint16(header->ehsize);
	header->phentsize = __swap_uint16(header->phentsize);
	header->phnum = __swap_uint16(header->phnum);
	header->shentsize = __swap_uint16(header->shentsize);
	header->shnum = __swap_uint16(header->shnum);
	header->shstrndx = __swap_uint16(header->shstrndx);
}

void elf64_swap_bytes(elf32_header *header)
{
	header->type = __swap_uint16(header->type);
	header->isa = __swap_uint16(header->isa);
	header->version = __swap_uint32(header->version);
	header->entry = __swap_uint64(header->entry);
	header->phoff = __swap_uint64(header->phoff);
	header->shoff = __swap_uint64(header->shoff);
	header->flags = __swap_uint32(header->flags);
	header->ehsize = __swap_uint16(header->ehsize);
	header->phentsize = __swap_uint16(header->phentsize);
	header->phnum = __swap_uint16(header->phnum);
	header->shentsize = __swap_uint16(header->shentsize);
	header->shnum = __swap_uint16(header->shnum);
	header->shstrndx = __swap_uint16(header->shstrndx);
}

/**
 * Check if signature bytes PE\0\0 exist at index 0x3c
 * Signature only exists on image files and not object files
 * so this may be used to check the file type as well.
 *
 * Note that this value can also be checked with pe32_header
 * struct, the 'magic' value at the struct should be compared
 * agains FILE_FORMAT_PE32_SIGNATURE_EXISTS
 *
 * @return {int}: Returns 1 if exists, otherwise 0 is returned.
 */
int pe32_does_signature_exist(char *data)
{
	if (*(uint32_t*)(((uint8_t*)data) + 0xE8) == FILE_FORMAT_PE32_SIGNATURE_EXISTS)
		return 1;
	else
		return 0;
}

/**
 * Get machine type string, this info is
 * extracted from pe32_header.machine
 *
 * return {char*}: Return the corresponding string for machine type
 */
const char* pe32_get_machine_type_string(enum pe32_machine_type machine_type) {
	for (size_t i = 0; i < sizeof(pe32_machine_type_map) / sizeof(pe32_machine_type_map[0]); ++i) {
		if (pe32_machine_type_map[i].machine_type == machine_type) {
			return pe32_machine_type_map[i].str;
		}
	}
	return "Invalid type";
}

/**
 * Check if flag is set in pe32_header.characteristics
 *
 * return {int}: Return 1 if flag is set, otherwise return 0.
 */
int pe32_is_flag_set(uint16_t characteristics, enum pe32_image_characteristics bit)
{
	return !!(characteristics & bit);
}

/* Determine if file link format */
int determine_link_format(char *data)
{
	if(data[0] == 0x7f && data[1] == 'E' &&
	   data[2] == 'L' && data[3] == 'F') {
		return HEADER_FORMAT_ELF;
	}

	if ((data[0] == 'M') && (data[1] == 'Z'))
		return HEADER_FORMAT_PE32;

	return HEADER_FORMAT_UNKNOWN;
}

#endif /* FILE_FORMAT_IMPLEMENTATION */
