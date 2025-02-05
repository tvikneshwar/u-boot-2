/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __L1028A_COMMON_H
#define __L1028A_COMMON_H

/* SPL build */
#ifdef CONFIG_SPL_BUILD
#define SPL_NO_FMAN
#define SPL_NO_DSPI
#define SPL_NO_PCIE
#define SPL_NO_ENV
#define SPL_NO_MISC
#define SPL_NO_USB
#define SPL_NO_SATA
#define SPL_NO_QE
#define SPL_NO_EEPROM
#endif
#if (defined(CONFIG_SPL_BUILD) && defined(CONFIG_NAND_BOOT))
#define SPL_NO_MMC
#endif
#if (defined(CONFIG_SPL_BUILD) && defined(CONFIG_SD_BOOT_QSPI))
#define SPL_NO_IFC
#endif

#define CONFIG_REMAKE_ELF
#define CONFIG_FSL_LAYERSCAPE
#define CONFIG_MP

#include <asm/arch/stream_id_lsch3.h>
#include <asm/arch/config.h>
#include <asm/arch/soc.h>

/* Link Definitions */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_FSL_OCRAM_BASE + 0xfff0)

#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_VERY_BIG_RAM
#define CONFIG_SYS_DDR_SDRAM_BASE	0x80000000UL
#define CONFIG_SYS_FSL_DDR_SDRAM_BASE_PHY	0
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE
#define CONFIG_SYS_DDR_BLOCK2_BASE	0x2080000000ULL
#define CONFIG_SYS_FSL_DDR_MAIN_NUM_CTRLS	1

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START        0x80000000
#define CONFIG_SYS_MEMTEST_END          0x9fffffff

/*
 * SMP Definitinos
 */
#define CPU_RELEASE_ADDR		secondary_boot_func

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		25000000	/* 25MHz */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 2048 * 1024)

/* I2C */
#define CONFIG_SYS_I2C

/* Serial Port */
#define CONFIG_CONS_INDEX       1
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE     1
#define CONFIG_SYS_NS16550_CLK          (get_bus_freq(0) / 2)

#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* ENETC */
#ifdef CONFIG_FSL_ENETC
#define CFG_ENETC_PHYS_ADDR 0x1f0000000ULL
#define CFG_ENETC_PHYS_SIZE 0x10000000UL
#define CONFIG_FSL_MEMAC
#endif

/* required for MDIO muxing */
#define CONFIG_LAST_STAGE_INIT

/* Miscellaneous configurable options */
#define CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_DDR_SDRAM_BASE + 0x10000000)

/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

#define CONFIG_NR_DRAM_BANKS		2

#define CONFIG_HWCONFIG
#define HWCONFIG_BUFFER_SIZE		128

#ifndef SPL_NO_ENV
/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#ifndef CONFIG_EMU
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0)
#include <config_distro_bootcmd.h>

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"board=ls1028ardb\0"			\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"fdt_addr=0x00f00000\0"                 \
	"kernel_addr=0x01000000\0"              \
	"scriptaddr=0x80000000\0"               \
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"         \
	"kernelheader_addr_r=0x80200000\0"      \
	"load_addr=0xa0000000\0"            \
	"kernel_addr_r=0x81000000\0"            \
	"fdt_addr_r=0x90000000\0"               \
	"ramdisk_addr_r=0xa0000000\0"           \
	"kernel_start=0x1000000\0"		\
	"kernelheader_start=0x800000\0"		\
	"kernel_load=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"kernelheader_size=0x40000\0"		\
	"kernel_addr_sd=0x8000\0"		\
	"kernel_size_sd=0x14000\0"		\
	"kernelhdr_addr_sd=0x4000\0"		\
	"kernelhdr_size_sd=0x10\0"		\
	"console=ttyS0,115200\0"                \
	"mtdparts=" CONFIG_MTDPARTS_DEFAULT "\0"	\
	BOOTENV					\
	"boot_scripts=ls1028ardb_boot.scr\0"    \
	"boot_script_hdr=hdr_ls1028ardb_bs.out\0"	\
	"scan_dev_for_boot_part="               \
		"part list ${devtype} ${devnum} devplist; "   \
		"env exists devplist || setenv devplist 1; "  \
		"for distro_bootpart in ${devplist}; do "     \
		  "if fstype ${devtype} "                  \
			"${devnum}:${distro_bootpart} "      \
			"bootfstype; then "                  \
			"run scan_dev_for_boot; "            \
		  "fi; "                                   \
		"done\0"                                   \
	"scan_dev_for_boot="				  \
		"echo Scanning ${devtype} "		  \
				"${devnum}:${distro_bootpart}...; "  \
		"for prefix in ${boot_prefixes}; do "	  \
			"run scan_dev_for_scripts; "	  \
		"done;"					  \
		"\0"					  \
	"boot_a_script="				  \
		"load ${devtype} ${devnum}:${distro_bootpart} "  \
			"${scriptaddr} ${prefix}${script}; "    \
		"env exists secureboot && load ${devtype} "     \
			"${devnum}:${distro_bootpart} "		\
			"${scripthdraddr} ${prefix}${boot_script_hdr} " \
			"&& esbc_validate ${scripthdraddr};"    \
		"source ${scriptaddr}\0"	  \
	"qspi_bootcmd=echo Trying load from flexspi..;"      \
		"sf probe && sf read $load_addr "         \
		"$kernel_start $kernel_size; env exists secureboot "	\
		"&& sf read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; " \
		"bootm $load_addr#$board\0"		\
	"qspi_hdploadcmd=echo Trying load HDP firmware from flexspi..;"      \
		"hdp load 0x20900000 0x2000\0"		\
	"sd_bootcmd=echo Trying load from SD ..;"	\
		"mmcinfo; mmc read $load_addr "		\
		"$kernel_addr_sd $kernel_size_sd && "	\
		"env exists secureboot && mmc read $kernelheader_addr_r " \
		"$kernelhdr_addr_sd $kernelhdr_size_sd "		\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$board\0"		\
	"sd_hdploadcmd=echo Trying load HDP firmware from SD..;"      \
		"mmcinfo;mmc read $load_addr 0x4800 0x200 "		\
		"&& hdp load $load_addr 0x2000\0"	\
	"emmc_bootcmd=echo Trying load from EMMC ..;"	\
		"mmcinfo; mmc dev 1; mmc read $load_addr "		\
		"$kernel_addr_sd $kernel_size_sd && "	\
		"env exists secureboot && mmc read $kernelheader_addr_r " \
		"$kernelhdr_addr_sd $kernelhdr_size_sd "		\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$board\0"			\
	"emmc_hdploadcmd=echo Trying load HDP firmware from EMMC..;"      \
		"mmc dev 1;mmcinfo;mmc read $load_addr 0x4800 0x200 "		\
		"&& hdp load $load_addr 0x2000\0"

#undef CONFIG_BOOTCOMMAND
#if defined(CONFIG_SD_BOOT)
#define CONFIG_BOOTCOMMAND "run sd_hdploadcmd; run distro_bootcmd;run sd_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#elif defined(CONFIG_EMMC_BOOT)
#define CONFIG_BOOTCOMMAND "run emmc_hdploadcmd; run distro_bootcmd;run emmc_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#else
#define CONFIG_BOOTCOMMAND "run qspi_hdploadcmd; run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#endif
#endif
#endif

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE /* Boot args buffer */

#ifndef SPL_NO_MISC
#ifndef CONFIG_CMDLINE_EDITING
#define CONFIG_CMDLINE_EDITING		1
#endif
#endif

#define CONFIG_SYS_MAXARGS		64	/* max command args */

#define CONFIG_SYS_BOOTM_LEN   (64 << 20)      /* Increase max gunzip size */

/*  MMC  */
#ifndef SPL_NO_MMC
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif
#endif

#ifdef CONFIG_SPL
#define CONFIG_SPL_BSS_START_ADDR      0x80100000
#define CONFIG_SPL_BSS_MAX_SIZE                0x00100000
#define CONFIG_SPL_LDSCRIPT "arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_MAX_SIZE            0x16000
#define CONFIG_SPL_STACK               (CONFIG_SYS_FSL_OCRAM_BASE + 0x9ff0)
#define CONFIG_SPL_TARGET              "u-boot-with-spl.bin"
#define CONFIG_SPL_TEXT_BASE           0x18010000

#define CONFIG_SPL_I2C_SUPPORT

#define CONFIG_SYS_SPL_MALLOC_SIZE     0x00100000
#define CONFIG_SYS_SPL_MALLOC_START    0x80200000
#define CONFIG_SYS_MONITOR_LEN         (768 * 1024)
#endif

#ifdef CONFIG_EMU
#define CONFIG_ENV_SIZE			0x2000          /* 8KB */
#define CONFIG_SYS_MMC_ENV_DEV         0
#else
#if defined(CONFIG_SD_BOOT) || defined(CONFIG_EMMC_BOOT)
#define CONFIG_ENV_OFFSET              0x300000        /* 3MB */
#define CONFIG_ENV_SIZE			0x2000          /* 8KB */
#define CONFIG_TZPC_OCRAM_BSS_HEAP_NS
#define OCRAM_NONSECURE_SIZE		0x00010000
#else
#define CONFIG_ENV_OFFSET              0x300000        /* 3MB */
#define CONFIG_SYS_FSL_QSPI_BASE	0x20000000
#define CONFIG_ENV_ADDR			CONFIG_SYS_FSL_QSPI_BASE + CONFIG_ENV_OFFSET
#define CONFIG_ENV_SIZE			0x2000          /* 8KB */
#define CONFIG_ENV_SECT_SIZE           0x40000
#endif
#endif

#if defined(CONFIG_SD_BOOT)
#define CONFIG_SYS_MMC_ENV_DEV		0
#elif defined(CONFIG_EMMC_BOOT)
#define CONFIG_SYS_MMC_ENV_DEV		1
#endif

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_MONITOR_BASE CONFIG_SPL_TEXT_BASE
#else
#define CONFIG_SYS_MONITOR_BASE CONFIG_SYS_TEXT_BASE
#endif

/*  MMC  */
#ifndef SPL_NO_MMC
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif
#endif

/* I2C bus multiplexer */
#define I2C_MUX_PCA_ADDR_PRI            0x77 /* Primary Mux*/
#define I2C_MUX_CH_DEFAULT              0x8

/* RTC */
#define CONFIG_RTC_PCF2127
#define CONFIG_SYS_I2C_RTC_ADDR                0x51  /* Channel 3*/
#define CONFIG_CMD_DATE

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM		0
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

#endif /* __L1028A_COMMON_H */
