#include <linux/module.h>

/* validate @native and @pcp counter values match @expected */
#define CHECK(native, pcp, expected)                        \
	do {                                                    \
		WARN((native) != (expected),                        \
		     "raw %ld (0x%lx) != expected %lld (0x%llx)",	\
		     (native), (native),				            \
		     (long long)(expected), (long long)(expected));	\
		WARN(__this_cpu_read(pcp) != (expected),            \
		     "pcp %ld (0x%lx) != expected %lld (0x%llx)",	\
		     __this_cpu_read(pcp), __this_cpu_read(pcp),	\
		     (long long)(expected), (long long)(expected));	\
	} while (0)

static DEFINE_PER_CPU(long, long_counter);
static DEFINE_PER_CPU(unsigned long, ulong_counter);

static int __init percpu_test_init(void)
{
	/*
	 * volatile prevents compiler from optimizing it uses, otherwise the
	 * +ul_one/-ul_one below would replace with inc/dec instructions.
	 */
	volatile unsigned int ui_one = 1;
	long l = 0;
	unsigned long ul = 0;
	unsigned int cpu;

	pr_info("percpu test start...\n");

	/**
	struct cpumask __cpu_possible_mask;

	unsigned int cpumask_next(int n, const struct cpumask *srcp)
    {
        if (n != -1)
            cpumask_check(n);
        return find_next_bit(cpumask_bits(srcp), nr_cpumask_bits, n + 1);
    }

	#define for_each_possible_cpu(cpu) for_each_cpu((cpu), cpu_possible_mask)

	#define for_each_cpu(cpu, mask)				\
	for ((cpu) = -1;				            \
		(cpu) = cpumask_next((cpu), (mask)),	\
		(cpu) < nr_cpu_ids;)

	for (((cpu)) = -1;
        ((cpu)) = cpumask_next(((cpu)), (((const struct cpumask *)&__cpu_possible_mask))), ((cpu)) < nr_cpu_ids;) {
        printk("\001" "6" "DBG(%s): cpu=%u\n", "/home/jungjaejoon/Projects/kernel/linux-modules/lib/percpu_test.c", cpu);
    }
    */
	for_each_possible_cpu(cpu) {
        pr_info("DBG(%s): cpu=%u/%u\n", __FILE__, cpu, nr_cpu_ids);
	}

	/**
    __per_cpu_offset[0]=26A00000
    __per_cpu_offset[1]=26A80000
    __per_cpu_offset[2]=26B00000
    __per_cpu_offset[3]=26B80000
    */
	///arch/x86/kernel/setup_percpu.c, line 41 (as a variable)
	for (ul=0; ul < nr_cpu_ids; ul++) {
        pr_info("__per_cpu_offset[%lu]=%Lx\n", ul, __per_cpu_offset[ul]);
	}
	pr_info("NR_CPUS=%u, nr_cpu_ids=%u\n", NR_CPUS, nr_cpu_ids);
	//pr_info("BOOT_PERCPU_OFFSET=%x\n", BOOT_PERCPU_OFFSET);
	pr_info("__per_cpu_offset=%Lx\n", __per_cpu_offset[0]);
	pr_info("__per_cpu_offset=%Lx\n", __per_cpu_offset[NR_CPUS-1]);
	pr_info("__per_cpu_offset=%lu\n", __per_cpu_offset[0]);
	pr_info("__per_cpu_offset=%lu\n", __per_cpu_offset[NR_CPUS-1]);

	pr_info("__my_cpu_offset=%Lx\n", __my_cpu_offset);



	preempt_disable();

	l += -1;
	__this_cpu_add(long_counter, -1);
	CHECK(l, long_counter, -1);

	l += 1;
	__this_cpu_add(long_counter, 1);
	CHECK(l, long_counter, 0);

	ul = 0;
	__this_cpu_write(ulong_counter, 0);

	ul += 1UL;
	__this_cpu_add(ulong_counter, 1UL);
	CHECK(ul, ulong_counter, 1);

	ul += -1UL;
	__this_cpu_add(ulong_counter, -1UL);
	CHECK(ul, ulong_counter, 0);

	ul += -(unsigned long)1;
	__this_cpu_add(ulong_counter, -(unsigned long)1);
	CHECK(ul, ulong_counter, -1);

	ul = 0;
	__this_cpu_write(ulong_counter, 0);

	ul -= 1;
	__this_cpu_dec(ulong_counter);
	CHECK(ul, ulong_counter, -1);
	CHECK(ul, ulong_counter, ULONG_MAX);

	l += -ui_one;
	__this_cpu_add(long_counter, -ui_one);
	CHECK(l, long_counter, 0xffffffff);

	l += ui_one;
	__this_cpu_add(long_counter, ui_one);
	CHECK(l, long_counter, (long)0x100000000LL);


	l = 0;
	__this_cpu_write(long_counter, 0);

	l -= ui_one;
	__this_cpu_sub(long_counter, ui_one);
	CHECK(l, long_counter, -1);

	l = 0;
	__this_cpu_write(long_counter, 0);

	l += ui_one;
	__this_cpu_add(long_counter, ui_one);
	CHECK(l, long_counter, 1);

	l += -ui_one;
	__this_cpu_add(long_counter, -ui_one);
	CHECK(l, long_counter, (long)0x100000000LL);

	l = 0;
	__this_cpu_write(long_counter, 0);

	l -= ui_one;
	this_cpu_sub(long_counter, ui_one);
	CHECK(l, long_counter, -1);
	CHECK(l, long_counter, ULONG_MAX);

	ul = 0;
	__this_cpu_write(ulong_counter, 0);

	ul += ui_one;
	__this_cpu_add(ulong_counter, ui_one);
	CHECK(ul, ulong_counter, 1);

	ul = 0;
	__this_cpu_write(ulong_counter, 0);

	ul -= ui_one;
	__this_cpu_sub(ulong_counter, ui_one);
	CHECK(ul, ulong_counter, -1);
	CHECK(ul, ulong_counter, ULONG_MAX);

	ul = 3;
	__this_cpu_write(ulong_counter, 3);

	ul = this_cpu_sub_return(ulong_counter, ui_one);
	CHECK(ul, ulong_counter, 2);

	ul = __this_cpu_sub_return(ulong_counter, ui_one);
	CHECK(ul, ulong_counter, 1);

	preempt_enable();

	pr_info("percpu test done.\n");

	return -EAGAIN;  /* Fail will directly unload the module */
	///return 0;
}

static void __exit percpu_test_exit(void)
{
    pr_info("percpu test exit.\n");
}

module_init(percpu_test_init)
module_exit(percpu_test_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Thelen");
MODULE_DESCRIPTION("percpu operations test");
