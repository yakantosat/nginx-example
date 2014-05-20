#include <stdio.h>
#include "basic.h"
#include "ngx_queue.h"

typedef struct yahoo_s {
    ngx_queue_t queue;
} yahoo_t;

typedef struct yahoo_guy_s {
    ngx_uint_t id;
    u_char *name;
    ngx_queue_t queue;
} yahoo_guy_t;

ngx_int_t yahoo_no_cmp(const ngx_queue_t *p, const ngx_queue_t *n) {
    yahoo_guy_t *pre, *next;
    pre = (yahoo_guy_t*) ngx_queue_data(p, yahoo_guy_t, queue);
    next = (yahoo_guy_t*) ngx_queue_data(n, yahoo_guy_t, queue);
    return ((pre->id > next->id) ? 1:0);
}

int main(void) {
    ngx_pool_t *pool;
    yahoo_guy_t *guy;
    ngx_queue_t *q;
    yahoo_t *yahoo;
    pool = ngx_create_pool(1024 * 10, NULL);
    int i;

    const ngx_str_t names[] = {
        ngx_string("rainx"), ngx_string("xiaozhe"), ngx_string("zhoujian")
    };
    const int ids[] = {4611, 8322, 6111};

    yahoo = ngx_palloc(pool, sizeof(yahoo_t));
    ngx_queue_init(&yahoo->queue);

    for (i=0; i < 3; i++) {
        guy = (yahoo_guy_t *) ngx_palloc(pool, sizeof(yahoo_guy_t));
        guy->id = ids[i];
        guy->name = (u_char*) ngx_pstrdup(pool, (ngx_str_t*) &(names[i]));

        ngx_queue_init(&guy->queue);
        ngx_queue_insert_head(&yahoo->queue, &guy->queue);
    }

    for (q = ngx_queue_last(&yahoo->queue);
         q != ngx_queue_sentinel(&yahoo->queue);
         q = ngx_queue_prev(q)) {
        guy = ngx_queue_data(q, yahoo_guy_t, queue);
        printf("No, %d guy in yahoo is %s\n", guy->id, guy->name);
    }

    ngx_queue_sort(&yahoo->queue, yahoo_no_cmp);
    printf("Sorting...\n");
    for (q = ngx_queue_prev(&yahoo->queue);
         q != ngx_queue_sentinel(&yahoo->queue);
         q = ngx_queue_last(q)) {
        guy = ngx_queue_data(q, yahoo_guy_t, queue);
        printf("No. %d guy in yahoo is %s\n", guy->id, guy->name);
    }

    ngx_destroy_pool(pool);

    return 0;
}
