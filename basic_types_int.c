#include <stdio.h>
#include "../../core/ngx_config.h"

int main(void) {

    ngx_uint_t a;
    ngx_int_t  b;

    a = 1000;
    b = -1000;

    printf("%d + %d = %d\n", a, b, a+b);

    return 0;
}
