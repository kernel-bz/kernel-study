// SPDX-License-Identifier: GPL-2.0-only
/*
 *      pci-test.c
 *
 *      Jung-JaeJoon <rgbi3307@gmail.com>
 */

#define pr_fmt(fmt)     "PCI-TEST: " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>

//#define PCI_VID	0x1106	//vendor id
#define PCI_VID		0x1556	//vendor id

//#define PCI_DID	0x3483	//device id
#define PCI_DID		0x1111	//device id

static struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(PCI_VID, PCI_DID) },
	{ }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_test_probe(struct pci_dev *dev, const struct pci_device_id *id) 
{
	u16 vid, did, status;
	u8 capa;
	u32 base0;

	pr_info("vender:0x%x, device:0x%x, class:0x%x\n", 
				dev->vendor, dev->device, dev->class);

	if (0 != pci_read_config_word(dev, PCI_VENDOR_ID, &vid))
		return -1;

	if (0 != pci_read_config_word(dev, PCI_DEVICE_ID, &did))
		return -1;

	pr_info("config id: vendor_id:0x%x, device_id:0x%x\n", vid, did);

	if (0 != pci_read_config_byte(dev, PCI_CAPABILITY_LIST, &capa))
		return -1;

	pr_info("config capability:0x%x\n", capa);

	if (0 != pci_read_config_dword(dev, PCI_BASE_ADDRESS_0, &base0))
		return -1;

	if (0 != pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0xffffffff))
		return -1;

	if (0 != pci_read_config_dword(dev, PCI_BASE_ADDRESS_0, &base0))
		return -1;

	if ((base0 & 0x3) == 1)
		pr_info("config base address 0 is IO space.\n");
	else 
		pr_info("config base address 0 is memory space.\n");

	if (0 != pci_read_config_word(dev, PCI_STATUS, &status))
		return -1;

	if ((status >> 3) & 0x1)
		pr_info("config interrupt bit is set.\n");
	else
		pr_info("config interrupt bit is not set.\n");

	return 0;
}

static void pci_test_remove(struct pci_dev *dev)
{
	pr_info("vender:0x%x, device:0x%x, class:0x%x\n", 
				dev->vendor, dev->device, dev->class);
}

static struct pci_driver pci_test_driver = {
	.name = "pci-test",
	.id_table = pci_ids,
	.probe = pci_test_probe,
	.remove = pci_test_remove,
};

static int __init pci_test_init(void)
{
	int ret;

        pr_info("Hello pci-test.\n");
	ret = pci_register_driver(&pci_test_driver);
	return ret;
}

static void __exit pci_test_exit(void)
{
        pr_info("Goodbye pci-test.\n");
	pci_unregister_driver(&pci_test_driver);
}

module_init(pci_test_init);                                                      
module_exit(pci_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux");
