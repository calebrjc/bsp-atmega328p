#include "bsp/dsa/queue.h"

#include <string.h>

void queue_enqueue(queue *q, const void *data) {
    // If the queue is full, do nothing
    if (!q || !data || queue_is_full(q)) return;

    // Enqueue the data
    memcpy(&q->data[q->head_idx * q->element_size], data, q->element_size);

    // Wrap around if the head_idx is at the end of the queue
    q->head_idx = (q->head_idx + 1 == q->data_size) ? 0 : q->head_idx + 1;

    // If the head_idx is equal to the tail_idx, the queue is full
    if (q->head_idx == q->tail_idx) q->is_full = true;
}

void queue_dequeue(queue *q, void *o_data) {
    // If the queue is empty, return 0
    if (!q || !o_data || queue_is_empty(q)) {
        o_data = NULL;
        return;
    }

    // Dequeue the data
    memcpy(o_data, &q->data[q->tail_idx * q->element_size], q->element_size);

    // Wrap around if the tail_idx is at the end of the queue
    q->tail_idx = (q->tail_idx + 1 == q->data_size) ? 0 : q->tail_idx + 1;

    // If the head_idx is not equal to the tail_idx, the queue is not full
    if (q->head_idx != q->tail_idx) q->is_full = false;
}

void queue_peek(const queue *q, void *o_data) {
    // If the queue is empty, return 0
    if (!q || !o_data || queue_is_empty(q)) {
        o_data = NULL;
        return;
    }

    // Peek at the data
    memcpy(o_data, &q->data[q->tail_idx * q->element_size], q->element_size);
}

size_t queue_size(const volatile queue *q) {
    if (!q) return 0;

    if (q->is_full) return q->data_size;

    if (q->head_idx >= q->tail_idx) return q->head_idx - q->tail_idx;

    return q->data_size - q->tail_idx + q->head_idx;
}

bool queue_is_empty(const volatile queue *q) {
    if (!q) return true;

    return q->head_idx == q->tail_idx && !q->is_full;
}

bool queue_is_full(const volatile queue *q) {
    if (!q) return false;

    return q->is_full;
}
