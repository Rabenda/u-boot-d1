#include <linux/stringify.h>

#define CONFIG_SYS_CACHELINE_SIZE	64

/* FIXME: Need a real clock driver! */
#define CONFIG_SYS_NS16550_CLK		24000000
#define CONFIG_SYS_TCLK			24000000

#ifdef CONFIG_USB_EHCI_HCD
#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS 1
#endif

#define CONFIG_SYS_INIT_SP_ADDR		0x4fe00000
#define CONFIG_SPL_BSS_MAX_SIZE		0x00080000 /* 512 KiB */
#define CONFIG_SPL_BSS_START_ADDR	0x4ff80000
#define CONFIG_SPL_STACK		0x00048000

#define SDRAM_OFFSET(x)			0x4##x
#define BOOTM_SIZE			__stringify(0xa000000)
#define KERNEL_ADDR_R			__stringify(SDRAM_OFFSET(0200000))
#define KERNEL_COMP_ADDR_R		__stringify(SDRAM_OFFSET(4000000))
#define KERNEL_COMP_SIZE		__stringify(0xb000000)
#define FDT_ADDR_R			__stringify(SDRAM_OFFSET(FA00000))
#define SCRIPT_ADDR_R			__stringify(SDRAM_OFFSET(FC00000))
#define PXEFILE_ADDR_R			__stringify(SDRAM_OFFSET(FD00000))
#define FDTOVERLAY_ADDR_R		__stringify(SDRAM_OFFSET(FE00000))
#define RAMDISK_ADDR_R			__stringify(SDRAM_OFFSET(FF00000))

#define MEM_LAYOUT_ENV_SETTINGS \
	"bootm_size=" BOOTM_SIZE "\0" \
	"fdt_addr_r=" FDT_ADDR_R "\0" \
	"fdtoverlay_addr_r=" FDTOVERLAY_ADDR_R "\0" \
	"kernel_addr_r=" KERNEL_ADDR_R "\0" \
	"kernel_comp_addr_r=" KERNEL_COMP_ADDR_R "\0" \
	"kernel_comp_size=" KERNEL_COMP_SIZE "\0" \
	"pxefile_addr_r=" PXEFILE_ADDR_R "\0" \
	"ramdisk_addr_r=" RAMDISK_ADDR_R "\0" \
	"scriptaddr=" SCRIPT_ADDR_R "\0"

#ifdef CONFIG_MMC
#define BOOT_TARGET_DEVICES_MMC(func) func(MMC, mmc, 0)
#else
#define BOOT_TARGET_DEVICES_MMC(func)
#endif

#ifdef CONFIG_USB_STORAGE
#define BOOT_TARGET_DEVICES_USB(func) func(USB, usb, 0)
#else
#define BOOT_TARGET_DEVICES_USB(func)
#endif

#ifdef CONFIG_CMD_DHCP
#define BOOT_TARGET_DEVICES_DHCP(func) func(DHCP, dhcp, na)
#else
#define BOOT_TARGET_DEVICES_DHCP(func)
#endif

#define BOOT_TARGET_DEVICES(func) \
	BOOT_TARGET_DEVICES_MMC(func) \
	BOOT_TARGET_DEVICES_USB(func) \
	BOOT_TARGET_DEVICES_DHCP(func)

#include <config_distro_bootcmd.h>
#include <environment/distro/sf.h>

#define SUN20I_FEDORA_BOOTENV \
	"bootdir=/boot\0" \
	"bootenv=uEnv.txt\0" \
	"mmcdev=0\0" \
	"mmcpart=3\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
	MEM_LAYOUT_ENV_SETTINGS \
	SUN20I_FEDORA_BOOTENV \
	"loadaddr=0x50000000\0" \
	"loadbootenv=fatload mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"ext4bootenv=ext4load mmc ${bootpart} ${loadaddr} ${bootdir}/${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc${mmcdev} ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"mmcbootenv=setenv bootpart ${mmcdev}:${mmcpart}; " \
		"mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"run loadbootenv && run importbootenv; " \
			"run ext4bootenv && run importbootenv; " \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...; " \
				"run uenvcmd; " \
			"fi; " \
		"fi\0" \
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	BOOTENV \
	BOOTENV_SF
