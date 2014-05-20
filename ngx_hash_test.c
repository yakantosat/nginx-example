#include <stdio.h>
#include "basic.h"
#include "ngx_array.h"
#include "ngx_hash.h"

static ngx_str_t names[] = {
        ngx_string("rainx"), ngx_string("xiaozhe"), ngx_string("zhoujian")
};

static char* descs[] = {
        "rainx's id is 1",
        "xiaozhe's id is 2",
        "zhoujian's id is 3"
};

int main(void) {
    ngx_uint_t k;
    ngx_pool_t *pool;
    ngx_hash_init_t  hash_init;
    ngx_hash_t *hash;
    ngx_array_t  *elements;
    ngx_hash_key_t   *arr_node;
    char   *find;
    int i;

    ngx_cacheline_size = 32;

    ngx_str_t str = ngx_string("hello, world");
    k = ngx_hash_key_lc(str.data, str.len);
    pool = ngx_create_pool(1024 * 10, NULL);
    printf("caculated key is %u\n", k);

    hash = (ngx_hash_t*) ngx_pcalloc(pool, sizeof(hash));
    hash_init.hash = hash;
    hash_init.key = &ngx_hash_key_lc;
    hash_init.max_size = 1024 * 10;
    hash_init.bucket_size = 64;
    hash_init.name = "yahoo_guy_hash";
    hash_init.pool = pool;
    hash_init.temp_pool = NULL;

    elements = ngx_array_create(pool, 32, sizeof(ngx_hash_key_t));
    for (i = 0; i < 3; i++) {
        arr_node = (ngx_hash_key_t *) ngx_array_push(elements);
        arr_node->key = (names[i]);
        arr_node->key_hash = ngx_hash_key_lc(arr_node->key.data, arr_node->key.len);
        arr_node->value = (void *) descs[i];

        printf("key: %s, key_hash: %u\n", arr_node->key.data, arr_node->key_hash);
    }

    if (ngx_hash_init(&hash_init, (ngx_hash_key_t *) elements->elts, elements->nelts) != NGX_OK) {
        return 1;
    }

    k = ngx_hash_key_lc(names[0].data, names[0].len);
    printf("%s key is %d\n", names[0].data, k);
    find = (char *) ngx_hash_find(hash, k, (u_char *) names[0].data, names[0].len);

    if (find) {
        printf("get desc of rainx: %s\n", (char *) find);

    }

    ngx_array_destroy(elements);
    ngx_destroy_pool(pool);

    return 0;
}
