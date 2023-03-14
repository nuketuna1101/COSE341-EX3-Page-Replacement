#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Problem #1 :: LRU Algorithm using Stack :: in sw, 
int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
}Node;

int isEmpty(Node* frame) {
    if (frame->next == NULL) {
        return 1;
    }
    return 0;
}

void push(Node* frame, int data) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    if (isEmpty(frame) == 0) {
        frame->next->prev = newnode;
    }
    newnode->data = data;
    newnode->next = frame->next;
    newnode->prev = frame;
    frame->next = newnode;
}

void pop_top(Node* frame) {
    // frame is header, remove the top
    Node* top = frame->next;
    frame->next = top->next;
    top->next->prev = frame;
    free(top);
}

void pop_bot(Node* frame) {
    Node* current = frame;
    while (1) {
        if (current->next == NULL) {
            // current is bottom
            current->prev->next = NULL;
            free(current);
            break;
        }
        else {
            current = current->next;
        }
    }
}

void search_pop(Node* frame, int data) {
    Node* current = frame;

    while (1) {
        if (current->next == NULL) {

        }
        else {
            current = current->next;
            if (current->data == data) {
                // we find it
                if (current->next == NULL) {
                    current->prev->next = current->next;
                    free(current);
                    break;
                }
                else {
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    free(current);
                    break;
                }
            }
        }
    }
}

int find(Node* frame, int data) {
    Node* current = frame;
    while (1) {
        if (current->next == NULL) {
            return 0;
        }
        else {
            current = current->next;
            if (current->data == data) {
                // 1 as we find it
                return 1;
            }
        }
    }
}

int isFull(Node* frame, size_t frame_sz) {
    Node* current = frame;
    for (int i = 0; i < frame_sz; i++) {
        if (current->next == NULL) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void showRefarr(int* ref, size_t refsize) {
    printf("+--<Ref array>--+\n");
    for (int i = 0; i < refsize; i++) {
        printf("%d ", ref[i]);
    }
    printf("\n");
}

void showFrame(Node* frame, size_t frame_sz) {
    Node* current = frame;
    printf("Frames :: ");
    for (int i = 0; i < frame_sz; i++) {
        if (current->next == NULL) {
            printf("- ");
        }
        else {
            current = current->next;
            printf("%d ", current->data);
        }
    }
}

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
    if (argc < 4) return -1;

    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int page_faults = lru(ref, ref_arr_sz, frame_sz);
    printf("Total page faults :: %d\n", page_faults);
    free(ref);
    return 0;
}

int* generate_ref_arr(size_t sz, size_t page_max) {
    int* ref_arr = (int*)malloc(sizeof(int) * sz);

    // TODO :: Generate Random Refernece String

    // for random value.
    srand(time(NULL));
    // sz as # of frames
    for (int i = 0; i < sz; i++) {
        ref_arr[i] = rand() % page_max;
    }
    return ref_arr;
}

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz) {
    int i, j;
    int page_faults = 0;
    // Initializing frames
    //int* frames = (int*)malloc(sizeof(int) * frame_sz);
    //for (i = 0; i < frame_sz; i++) frames[i] = -1;
    // frame node is the header for frames
    Node* frame = (Node*)malloc(sizeof(Node));
    frame->data = -1;
    frame->prev = NULL;
    frame->next = NULL;
    // TODO
    // LRU using Stack
    showRefarr(ref_arr, ref_arr_sz);
    for (i = 0; i < ref_arr_sz; i++) {
        printf("+- %d :: ", ref_arr[i]);
        // if we can find the page, we just update the Top
        if (find(frame, ref_arr[i]) == 1) {
            search_pop(frame, ref_arr[i]);
            push(frame, ref_arr[i]);
            showFrame(frame, frame_sz);
        }
        // if we cannot find the page, page fault occur
        else {
            if (isFull(frame, frame_sz) == 0) {
                // free frame exists, so we just push
                push(frame, ref_arr[i]);
            }
            else if (isFull(frame, frame_sz) == 1) {
                // frame is full, so start replacement :: pop the lowest in stack, and push new one
                pop_bot(frame);
                push(frame, ref_arr[i]);
            }
            page_faults += 1;
            showFrame(frame, frame_sz);
            printf("(page fault)");
        }
        printf("\n");
    }
    return page_faults;
}