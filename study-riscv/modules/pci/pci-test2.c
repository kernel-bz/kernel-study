// SPDX-License-Identifier: GPL-2.0-only
/*
 *      pci-test.c
 *
 *      Jung-JaeJoon <rgbi3307@gmail.com>
 *	Refer: https://olegkutkov.me/2021/01/07/writing-a-pci-device-driver-for-linux/
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

#define PCI_IRQ_USR	42 
#define PCI_DRV_NAME	"pci-test"

static struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(PCI_VID, PCI_DID) },
	{ }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

/* This is a "private" data structure */
/* You can store there any data that should be passed between driver's functions */
struct pci_usr_struct {
	u8 __iomem *hwmem;
};

static irqreturn_t pci_irq_handler(int irq, void *cookie)
{
	(void) cookie;
	pr_info("Handle IRQ #%d\n", irq);

	return IRQ_HANDLED;
}

/* Reqest interrupt and setup handler */
static int pci_set_interrupts(struct pci_dev *pdev)
{
	int ret;
#if 0
	/* We want MSI interrupt, 3 lines (just an example) */
	ret = pci_alloc_irq_vectors(pdev, 3, 3, PCI_IRQ_MSI);

	if (ret < 0)
		return ret;

	/* Request IRQ #42 */
	return request_threaded_irq(PCI_IRQ_USR, 
			pci_irq_handler, NULL, 0, "TEST IRQ", pdev);
#endif
	if (pdev->irq) {
		ret = devm_request_irq(&pdev->dev, pdev->irq, 
				pci_irq_handler, 0, PCI_DRV_NAME, NULL);
	}

	pr_info("pdev->irq: %d, ret:%d\n", pdev->irq, ret);

	return ret;
}

/* Write some data to the device */
static void pci_write_sample_data(struct pci_dev *pdev)
{
	int data_sample = 0xDEADBEEF; /* Just a random trash */

	struct pci_usr_struct *pdata = (struct pci_usr_struct *)pci_get_drvdata(pdev);

	if (!pdata)
		return;

	/* Write 32-bit data to the device memory */
	iowrite32(data_sample, pdata->hwmem);
}

static void pci_read_sample_data(struct pci_dev *pdev)
{
	unsigned int data;

	struct pci_usr_struct *pdata = (struct pci_usr_struct *)pci_get_drvdata(pdev);

	if (!pdata)
		return;

	/* Write 32-bit data to the device memory */
	data = ioread32(pdata->hwmem);

	pr_info("pci_read: 0x%X\n", data);
}

static void pci_release_device(struct pci_dev *pdev)
{
	/* Disable IRQ #42*/
	//free_irq(PCI_IRQ_USR, pdev);

	/* Free memory region */
	pci_release_region(pdev, pci_select_bars(pdev, IORESOURCE_MEM));

	/* And disable device */
	pci_disable_device(pdev);
}

static int pci_test_probe(struct pci_dev *pdev, const struct pci_device_id *id) 
{
	u16 vid, did;
	int bar, err;
	unsigned long mmio_start, mmio_len;
	struct pci_usr_struct *pdata;

	pr_info("probe vender:0x%x, device:0x%x, class:0x%x\n", 
				pdev->vendor, pdev->device, pdev->class);

	if (0 != pci_read_config_word(pdev, PCI_VENDOR_ID, &vid))
		return -1;

	if (0 != pci_read_config_word(pdev, PCI_DEVICE_ID, &did))
		return -1;

	pr_info("config id: vendor_id:0x%x, device_id:0x%x\n", vid, did);


	/* Request IO BAR from resource*/
	bar = pci_select_bars(pdev, IORESOURCE_MEM);

	/* Enable device memory */
	//err = pci_enable_device_mem(pdev);
	err = pci_enable_device(pdev);
	if (err)
		return err;

	/* Request memory region for the BAR */
	err = pci_request_region(pdev, bar, PCI_DRV_NAME);
	if (err) {
		pci_disable_device(pdev);
		return err;
	}

	err = pci_set_interrupts(pdev);
	if (err) {
		pr_info("pci_set_interrupts() error!\n");
		return err;
	}

	/* Get start and stop memory offsets */
	mmio_start = pci_resource_start(pdev, 0);
	mmio_len = pci_resource_len(pdev, 0);
	pr_info("mmio: %lu: %lu\n", mmio_start, mmio_len);

	/* Allocate memory for the driver private data */
	//devm_kzalloc(pdev->dev, sizeof(struct pci_usr_struct), GFP_KERNEL);
	pdata = kzalloc(sizeof(struct pci_usr_struct), GFP_KERNEL);
	if (!pdata) {
		pci_release_device(pdev);
		return -ENOMEM;
	}

	/* Remap BAR to the local pointer */
	pdata->hwmem = ioremap(mmio_start, mmio_len);
	pr_info("pdata->hwmem: %p\n", pdata->hwmem);
	if (!pdata->hwmem) {
		pci_release_device(pdev);
		return -EIO;
	}

	/* Set driver private data */
	/* Now we can access mapped "hwmem" from the any driver's function */
	pci_set_drvdata(pdev, pdata);

	pci_write_sample_data(pdev);
	pci_read_sample_data(pdev);

	return 0;
}

static void pci_test_remove(struct pci_dev *pdev)
{
	pr_info("remove vender:0x%x, device:0x%x, class:0x%x\n", 
				pdev->vendor, pdev->device, pdev->class);

	struct pci_usr_struct *pdata = pci_get_drvdata(pdev);

	if (pdata) {
		if (pdata->hwmem) 
			iounmap(pdata->hwmem);

		pci_free_irq_vectors(pdev);

		kfree(pdata);
	}

	pci_release_device(pdev);
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
