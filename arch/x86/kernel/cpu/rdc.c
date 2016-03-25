/*
 * See Documentation/x86/rdc.txt
 *
 * mark@bifferos.com
 */

#include <linux/pci.h>
#include <asm/pci-direct.h>
#include "cpu.h"


static void __cpuinit rdc_identify(struct cpuinfo_x86 *c)
{
	u16 vendor, device;
	u32 customer_id;

	if (!early_pci_allowed())
		return;

	/* RDC CPU is SoC (system-on-chip), Northbridge is always present */
	vendor = read_pci_config_16(0, 0, 0, PCI_VENDOR_ID);
	device = read_pci_config_16(0, 0, 0, PCI_DEVICE_ID);

	if (vendor != PCI_VENDOR_ID_RDC || device != PCI_DEVICE_ID_RDC_R6020)
		return;  /* not RDC */
	/*
	 * NB: We could go on and check other devices, e.g. r6040 NIC, but
	 * that's probably overkill
	 */

	customer_id = read_pci_config(0, 0, 0, 0x90);

	switch (customer_id) {
		/* id names are from RDC */
	case 0x00321000:
		strcpy(c->x86_model_id, "R3210/R3211");
		break;
	case 0x00321001:
		strcpy(c->x86_model_id, "AMITRISC20000/20010");
		break;
	case 0x00321002:
		strcpy(c->x86_model_id, "R3210X/Edimax");
		break;
	case 0x00321003:
		strcpy(c->x86_model_id, "R3210/Kcodes");
		break;
	case 0x00321004:  /* tested */
		strcpy(c->x86_model_id, "S3282/CodeTek");
		break;
	case 0x00321007:
		strcpy(c->x86_model_id, "R8610");
		break;
	default:
		pr_info("RDC CPU: Unrecognised Customer ID (0x%x) please report to linux-kernel@vger.kernel.org\n", customer_id);
		break;
	}

	strcpy(c->x86_vendor_id, "RDC");
	c->x86_vendor = X86_VENDOR_RDC;
}

static const struct cpu_dev __cpuinitconst rdc_cpu_dev = {
	.c_vendor	= "RDC",
	.c_ident	= { "RDC" },
	.c_identify	= rdc_identify,
	.c_x86_vendor	= X86_VENDOR_RDC,
};

cpu_dev_register(rdc_cpu_dev);
