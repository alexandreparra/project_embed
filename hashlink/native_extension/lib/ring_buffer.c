#include <stdio.h>

#define HL_NAME(n) ring_buffer_##n
#include "hl.h"

typedef struct _rb {
    void (*finalize)(void *);
    void *array;
    size_t read_at;
    size_t write_at;
    int capacity;
} ring_buffer;

static void finalize(void *rb) {
    printf("Native extension: Deleting array\n");

    ring_buffer *r = (ring_buffer *) rb;
    free(r->array);
}

HL_PRIM ring_buffer *HL_NAME(alloc)(int initial_capacity) {
    // All printf can be seen when running your Haxe code.
    printf("Native extension: initialize ring buffer\n");

    // Here we'll be creating the ring buffer and allocating it to the GC, so it'll be used as a normal Haxe object.
    // This function will not be visible for anyone accessing the RingBuffer in Haxe (which we'll define in the Haxe code later).
    ring_buffer *buffer = hl_gc_alloc_finalizer(sizeof(ring_buffer));
    buffer->array = malloc(sizeof(int) * initial_capacity);

    // When the garbage collector release our struct Hashlink will call finalize function pointer to correctly clean up any
    // resources. Here we'll be freeing our array.
    buffer->finalize = finalize;

    buffer->read_at  = 0;
    buffer->write_at = 0;
    buffer->capacity = initial_capacity;
    return buffer;
}

HL_PRIM void HL_NAME(write)(ring_buffer *rb, int item) {
    if (rb->write_at >= rb->capacity) {
        rb->write_at = 0;
    }

    memcpy((uintptr_t *) rb->array + (rb->write_at * sizeof(int)), &item, sizeof(int));

    rb->write_at++;
    printf("Native extension: write %i at %zu\n", item, rb->write_at);
}

HL_PRIM int HL_NAME(read)(ring_buffer *rb) {
    if (rb->read_at >= rb->capacity) {
        rb->read_at = 0;
    }

    int *item = (int *) (uintptr_t *) rb->array+ (rb->read_at * sizeof(int));
    printf("Native extension: read item %i\n", *item);

    rb->read_at++;

    return *item;
}

HL_PRIM int HL_NAME(get_capacity)(ring_buffer *rb) {
    return rb->capacity;
}

#define _RING_BUFFER _ABSTRACT(ring_buffer)

DEFINE_PRIM(_RING_BUFFER, alloc, _I32)
DEFINE_PRIM(_VOID, write, _RING_BUFFER _I32)
DEFINE_PRIM(_I32, read, _RING_BUFFER)
DEFINE_PRIM(_I32, get_capacity, _RING_BUFFER)
