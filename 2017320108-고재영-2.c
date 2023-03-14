#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

typedef struct Node {
    int ref;
    struct Node* prev;
    struct Node* next;
}Node;

void push(Node* front, Node* rear) {
    // addition in rear
    Node* newnode = (Node*)malloc(sizeof(Node));
    //newnode->data = data;
    newnode->ref = 0;
    newnode->next = rear;
    newnode->prev = rear->prev;

    rear->prev->next = newnode;
    rear->prev = newnode;
}

int findID(int* frames, size_t frame_sz, int data) {
    // check if the data can be referred in frames, then return id
    for (int i = 0; i < frame_sz; i++) {
        if (frames[i] == data) {
            // gotcha!
            return i;
        }
    }
    // cannot find
    return -1;
}

void setOne(Node* front, Node* rear, int id) {
    Node* current = front;
    int steps = id + 1;
    for (int i = 0; i < steps; i++) {
        current = current->next;
    }
    current->ref = 1;
}

void clockTick(Node* front, Node* rear, Node* clockpoint, int clockid, size_t frame_sz) {
    clockpoint = clockpoint->next;
    clockid = (clockid + 1) % frame_sz;
    if (clockpoint == rear) {
        clockpoint = clockpoint->next->next;
    }
}

void showRefarr(int* ref, size_t refsize) {
    printf("+--<Ref array>--+\n");
    for (int i = 0; i < refsize; i++) {
        printf("%d ", ref[i]);
    }
    printf("\n");

}

void showFrame(int* frames, size_t frame_sz) {
    printf("Frames :: ");
    for (int i = 0; i < frame_sz; i++) {
        if (frames[i] == -1) {
            printf("- ");
        }
        else {
            printf("%d ", frames[i]);
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
    int page_faults = 0;

    // Initializing frames
    int* frames = (int*)malloc(sizeof(int) * frame_sz);
    for (int i = 0; i < frame_sz; i++) frames[i] = -1;
    Node* front = (Node*)malloc(sizeof(Node));
    Node* rear = (Node*)malloc(sizeof(Node));
    Node* clockpoint = (Node*)malloc(sizeof(Node));
    int clockid;

    front->ref = -1;
    front->prev = rear;
    front->next = rear;

    rear->ref = -1;
    rear->prev = front;
    rear->next = front;

    // INIT :: reference bits initialize : set bits zero-valued.
    for (int j = 0; j < frame_sz; j++) {
        push(front, rear);
    }
    // INIT :: clock points the first frame
    clockpoint = front->next;
    clockid = 0;

    // TODO
    // LRU using Clock Algorithm .. using reference bits circular queue

    showRefarr(ref_arr, ref_arr_sz);

    for (int i = 0; i < ref_arr_sz; i++) {
        //printf("+- %dth iteration\n", i + 1);
        // if we can find the page, we just update the Top
        printf("+- %d :: ", ref_arr[i]);
        // branch if page fault occur or not
        if (findID(frames, frame_sz, ref_arr[i]) != -1) {
            // no page fault, succeed to refer then set ref bit 1
            // 해당 인덱스의 ref bit을 1로 올려주세용~
            setOne(front, rear, findID(frames, frame_sz, ref_arr[i]));
            showFrame(frames, frame_sz);
        }
        else {
            // page fault occur, clockpointer continue ticking
            // if clock ref = 1이면,
            //          ref = 0으로 설정하고, 다음!
            //    clock ref = 0이면,
            //          그 자리에 page replace하고 ref = 1로 set
            while (1) {
                if (clockpoint->ref == 1) {
                    clockpoint->ref = 0;

                    clockpoint = clockpoint->next;
                    clockid = (clockid + 1) % frame_sz;
                    if (clockpoint == rear) {
                        clockpoint = clockpoint->next->next;
                    }

                }
                else if (clockpoint->ref == 0) {
                    frames[clockid] = ref_arr[i];
                    clockpoint->ref = 1;
                    break;
                }
            }
            page_faults += 1;
            showFrame(frames, frame_sz);
            printf("(page fault)");
        }
        printf("\n");
    }
    return page_faults;
}
