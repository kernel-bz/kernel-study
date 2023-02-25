#include <stdio.h>
//#include <stddef.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })

int main()
{
        struct data_ {
                char a;
                char a2;
                int b;
                long c;
                long long d;
                char e;
        } __packed;

        struct data_ data_st;
        data_st.a = 10;
        data_st.b = 20;
        data_st.c = 30;

        struct data_ *dp;
        dp = container_of(&data_st.c, struct data_, c);

        printf("%p\n", dp);
        printf("%p\n", &data_st);

        return 0;
}
