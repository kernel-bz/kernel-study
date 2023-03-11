/**
    M:	JaeJoon Jung <rgbi3307@nate.com>
    W:	https://www.kernel.bz
    F:	modules/data/xarray/xa_test01.c

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
*/

#include <linux/xarray.h>
#include <linux/module.h>
#include <linux/slab.h>

static DEFINE_XARRAY(XArray);    ///xarray 구조체 정의

///#define XA_DEBUG
///xarray 구조체 안의 내용을 확인하는 함수
static void xa_user_dump(const struct xarray *xa)
{
    void *entry = xa->xa_head;
    unsigned int shift = 0;
    ///구조체값 출력
    pr_info("xarray: %px head %px flags %x marks %d %d %d\n", xa, entry,
            xa->xa_flags, xa_marked(xa, XA_MARK_0),
            xa_marked(xa, XA_MARK_1), xa_marked(xa, XA_MARK_2));
}

static int xa_test_run(void)
{
    unsigned long i;
    void *ret;
    ///xarray에 연결(추가)하기 위한 사용자 데이터 구조체
    struct item {
        unsigned long index;
        unsigned int order;
    };
    struct item *item;
    pr_info("xa_test_run() starting...\n");

    item = kmalloc(sizeof(*item), GFP_KERNEL);  ///사용자 데이터 구조체 메모리 할당
    item->index = 0;
    item->order = 100;
    pr_info("[0] item=%p\n", item);             ///사용자 데이터 구조체  주소 출력

    ///xarray 인데스 0에 저장
    ret = xa_store(&XArray, 0, (void*)item, GFP_KERNEL);

    ///xarray에 인덱스 1부터 10까지 item을 저장
    for (i = 1; i <= 10; i++) {
        ///사용자 데이터 구조체 메모리 할당
        item = kmalloc(sizeof(*item), GFP_KERNEL);
        pr_info("[%d] item=%p, ", i, item);
        item->index = i;
        item->order = i+100;
        ///xarray 인덱스 i에 저장
        ret = xa_store(&XArray, i, (void*)item, GFP_KERNEL);
        pr_info("ret=%p\n", ret);   ///반환 주소 확인
        xa_user_dump(&XArray);       ///xarray 내용 출력
    }

    ///조회 하기
    ret = xa_load(&XArray, 0);    ///xarray 인덱스 0에 있는 내용 가져옴.
    pr_info("load  ret=%p, %d, %d\n", ret,
        ((struct item *)ret)->index, ((struct item *)ret)->order);  ///내용 출력
    ret = xa_load(&XArray, 8);    /// xarray 인데스 8에 있는 내용 가져옴
    pr_info("load  ret=%p, %d, %d\n", ret,
        ((struct item *)ret)->index, ((struct item *)ret)->order);  ///내용 출력

    ///삭제 하기
    ret = xa_erase(&XArray, 4);    /// xarray 인덱스 4에 있는 내용 삭제
    pr_info("erase  ret=%p, %d, %d\n", ret,
        ((struct item *)ret)->index, ((struct item *)ret)->order);
    ret = xa_erase(&XArray, 7);    /// xarray 인덱스 7에 있는 내용 삭제
    pr_info("erase  ret=%p, %d, %d\n", ret,
        ((struct item *)ret)->index, ((struct item *)ret)->order);

    i = 9;
    ///xarray 인덱스 9 검색
    ret = xa_find(&XArray, &i, ULONG_MAX, XA_PRESENT);
    pr_info("find  ret=%p, %d, %d\n", ret,
        ((struct item *)ret)->index, ((struct item *)ret)->order);

    ///xarray 순환 탐색
    xa_for_each(&XArray, i, ret) {
        pr_info("each  ret=%p, %d, %d\n", ret,
            ((struct item *)ret)->index, ((struct item *)ret)->order);
    }

    ///xarray 삭제
    xa_destroy(&XArray);

    pr_info("xa_test_run() end.\n");

    return 0;
}

static void xa_test_exit(void)
{
    pr_info("xa_test_exit().\n");
}

static int __init this_module_init(void)
{
	xa_test_run();
	return 0;
}

static void __exit this_module_exit(void)
{
	xa_test_exit();
}

module_init(this_module_init);
module_exit(this_module_exit);
MODULE_AUTHOR("JaeJoon Jung <rgbi3307@nate.com>");
MODULE_LICENSE("GPL");

