// SPDX-License-Identifier: GPL-2.0-only
/*
 *      pci-test.c
 *
 *      Jung-JaeJoon <rgbi3307@gmail.com>
 */

#define pr_fmt(fmt)     "MY-MODULE: " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>

#define PCI_VID_XHCI	0x8086	//vendor id
#define PCI_DID_XHCI	0x9d23	//device id

static struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(PCI_VID_XHCI, PCI_DID_XHCI) },
	{ }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_test_probe(struct pci_dev *dev, const struct pci_device_id *id) 
{
	u16 vid, did;
	u8 cap_ptr;
	u32 bar0, sbar0;

	pr_info("vender:%u, device:%u, class:%u\n", 
				dev->vendor, dev->device, dev->class);

	if (0 != pci_read_config_word(dev, 0x0, &vid)) {
		return -1;
	}

	if (0 != pci_read_config_word(dev, 0x2, &did)) {
		return -1;
	}

	if (0 != pci_read_config_byte(dev, 0x34, &cap_ptr)) {
		return -1;
	}

	if (0 != pci_read_config_dword(dev, 0x10, &bar0)) {
		return -1;
	}
	sbar0 = bar0;

	if (0 != pci_write_config_dword(dev, 0x10, 0xffffffff)) {
		return -1;
	}

	if (0 != pci_read_config_dword(dev, 0x10, &bar0)) {
		return -1;
	}


	return 0;
}

static void pci_test_remove(struct pci_dev *dev)
{
	pr_info("vender:%u, device:%u, class:%u\n", 
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
