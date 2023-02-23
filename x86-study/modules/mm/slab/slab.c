/*
 * This module emits "Hello, world" on printk when loaded.
 *
 * It is designed to be used for basic evaluation of the module loading
 * subsystem (for example when validating module signing/verification). It
 * lacks any extra dependencies, and will not normally be loaded by the
 * system unless explicitly requested by name.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include <linux/slab.h>
/**
    struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
			unsigned int align, slab_flags_t flags, void (*ctor)(void *));
    void kmem_cache_destroy(struct kmem_cache *);

    void *kmem_cache_alloc(struct kmem_cache *s, gfp_t gfpflags);
    void kmem_cache_free(struct kmem_cache *, void *);

    1) 기본 slab 캐시를 하나 만들고 object를 할당하는 샘플
    2) 에러를 강제 발생하고 디버깅(TRACE, owner trace 출력 분석, /sys/kernel/slab 확인
    3) tools/vm/slabinfo 빌드하여 실행 (or /proc/slabinfo)
    4) posion, red-zone
    5) printk("%pK", "%px", &kernel_addr): echo 1 > /proc/sys/kernel/kptr_restrict
    6) /proc/sys/kernel/slab/name
    7) sudo slabtop
*/

#define SLAB_TEST_SIZE      30
#define SLAB_TEST_ALIGN     56

static struct kmem_cache *test_cache;

void slab_test_kmem_alloc(void)
{
    void *buf;

	///buf = kmem_cache_alloc(test_cache, GFP_KERNEL);
	buf = kmem_cache_alloc(test_cache, 0);
	if (!buf) {
		pr_warn("kmem_cache_alloc() error!\n");
        return;
	} else {
        pr_info("kmem_cache_alloc(): buf p=%p, sp=%lu, sd=%lu\n"
                , buf, sizeof(buf), sizeof(*buf));
	}

	memset(buf, 0x55, SLAB_TEST_SIZE);
	memset(buf, 0xAA, SLAB_TEST_ALIGN);

	//void print_hex_dump(const char *level, const char *prefix_str, int prefix_type,
	//	    int rowsize, int groupsize, const void *buf, size_t len, bool ascii)
	print_hex_dump(KERN_DEBUG, "  ", DUMP_PREFIX_ADDRESS,
                    16, 1, buf, SLAB_TEST_ALIGN, 1);

    kmem_cache_free(test_cache, buf);

    memset(buf, 0xFF, 1);
    print_hex_dump(KERN_DEBUG, "  ", DUMP_PREFIX_ADDRESS,
                    16, 1, buf, SLAB_TEST_ALIGN, 1);
}

void slab_test_kmem_create(void)
{
    ///test_cache = kmem_cache_create("test_cache", SLAB_TEST_SIZE, SLAB_TEST_ALIGN
    ///                , SLAB_TYPESAFE_BY_RCU | SLAB_DEBUG_OBJECTS, NULL);    ///30, 56
    ///test_cache = kmem_cache_create("test_cache", 30, 0, SLAB_TYPESAFE_BY_RCU, NULL);    ///30, 102
    test_cache = kmem_cache_create("test_cache", SLAB_TEST_SIZE, SLAB_TEST_ALIGN
                    , SLAB_POISON | SLAB_RED_ZONE | SLAB_CONSISTENCY_CHECKS	| SLAB_STORE_USER | SLAB_TRACE, NULL);

	if (!test_cache) {
		pr_warn("slab_test_kmem() error!\n");
        return;
	} else {
        pr_info("kmem_cache_create(): test_cache p=%p\n", test_cache);
	}
}

void slab_test_kmalloc(void)
{
    void *p = kmalloc(32, GFP_KERNEL);
    pr_info("slab_test_kmalloc(): kmalloc p=%p\n", p);

    kfree(p);
}

static int __init slab_module_init(void)
{
	pr_warn("Hello, slab test\n");

    slab_test_kmalloc();
    slab_test_kmem_create();
    slab_test_kmem_alloc();

	return 0;
}

module_init(slab_module_init);

static void __exit slab_module_exit(void)
{
    kmem_cache_destroy(test_cache);

	pr_warn("Goodbye, slab test\n");
}

module_exit(slab_module_exit);

MODULE_AUTHOR("JaeJoon Jung<rgbi3307@nate.com>");
MODULE_LICENSE("GPL");
