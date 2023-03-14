#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Problem #3 :: LRU Algorithm using Additional Reference bits algorithm
int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max);

int find(int* ExistArr, int data) {
    // check if the page exists in frame, return
    if (ExistArr[data] == 1) {
        return 1;
    }
    return 0;
}

int isFull(int* ExistArr, size_t page_max, size_t frame_sz) {
    int cnt = 0;
    for (int i = 0; i < page_max; i++) {
        cnt += ExistArr[i];
    }
    if (cnt == frame_sz) {
        // 1 if full
        return 1;
    }
    else if (cnt < frame_sz) {
        // 0 if not full
        return 0;
    }
    // -1 if error
    return -1;
}

int showVictimID(unsigned char* page_ref_bits, size_t page_max, int* frames, size_t frame_sz) {
    // full frameÀÌ¶ó¼­ replace ÇØÁà¾ß ÇÑ´Ù·Õ
    unsigned char lowest_ref_bit = 255;
    int lowest_framesID = -1;
    for (int i = 0; i < frame_sz; i++) {
        if (lowest_ref_bit > page_ref_bits[frames[i]]) {
            lowest_ref_bit = page_ref_bits[frames[i]];
            lowest_framesID = frames[i];
        }
    }
    return lowest_framesID;
}


void updateFrames(int* frames, size_t frame_sz, int* ExistArr, size_t page_max) {
    // Frames has the frame_szed number of frames ... extracted from top page_ref_bits 
    int j = 0;
    for (int i = 0; i < page_max; i++) {
        if (ExistArr[i] == 1) {
            frames[j] = i;
            j++;
        }
    }
}

void showRefarr(int* ref, size_t refsize) {
    printf("+--<Ref array>--+\n");
    for (int i = 0; i < refsize; i++) {
        printf("%d ", ref[i]);
    }
    printf("\n");
}

void timerInterrupt(unsigned char* page_ref_bits, size_t page_max) {
    for (int i = 0; i < page_max; i++) {
        page_ref_bits[i] >>= 1;
    }
}

void showFrames(int* frames, size_t frame_sz) {
    printf("Frames ::");
    for (int i = 0; i < frame_sz; i++) {
        if (frames[i] == -1) {
            printf("- ");
        }
        else {
            printf("%d ", frames[i]);
        }
    }
}

void showRefBits(unsigned char* page_ref_bits, size_t page_max) {
    unsigned char tmp = 0x80;
    printf("  Ref Bits :: ");
    for (int i = 0; i < page_max; i++) {
        tmp = 0x80;
        // convert to binary
        //printf("%u ", page_ref_bits[i]);
        for (int j = 0; j < 8; j++) {
            if (page_ref_bits[i] & tmp) {
                printf("1");
            }
            else {
                printf("0");
            }
            tmp >>= 1;
        }
        printf(" ");
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
    int page_faults = lru(ref, ref_arr_sz, frame_sz, page_max);
    printf("%d\n", page_faults);
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

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max) {
    int i, j;
    int page_faults = 0;

    // Initializing frames
    int* frames = (int*)malloc(sizeof(int) * frame_sz);
    for (i = 0; i < frame_sz; i++) frames[i] = -1;

    // INIT :: reference bits - unsigned char 1byte = 8bits
    unsigned char* page_ref_bits = (unsigned char*)malloc(sizeof(unsigned char) * page_max);
    for (i = 0; i < page_max; i++) page_ref_bits[i] = 0;

    // INIT :: page exist in frame
    int* ExistArr = (int*)malloc(sizeof(int) * page_max);
    for (i = 0; i < page_max; i++) ExistArr[i] = 0;

    // TODO
    showRefarr(ref_arr, ref_arr_sz);

    for (i = 0; i < ref_arr_sz; i++) {
        printf("+- %d :: ", ref_arr[i]);
        // timer interrupt occurs
        timerInterrupt(page_ref_bits, page_max);

        if (find(ExistArr, ref_arr[i]) == 1) {
            // find desired page :: page refer
            // set reference bits 
            page_ref_bits[ref_arr[i]] += 128;
            updateFrames(frames, frame_sz, ExistArr, page_max);
            showFrames(frames, frame_sz);
            printf("       ");
        }
        else {
            // cannot find :: page fault occur
            if (isFull(ExistArr, page_max, frame_sz) == 0) {
                // not full frame -> just add
                ExistArr[ref_arr[i]] = 1;
            }
            else if (isFull(ExistArr, page_max, frame_sz) == 1) {
                // already full frame -> replace the victim
                ExistArr[showVictimID(page_ref_bits, page_max, frames, frame_sz)] = 0;
                ExistArr[ref_arr[i]] = 1;
            }
            page_ref_bits[ref_arr[i]] += 128;
            updateFrames(frames, frame_sz, ExistArr, page_max);
            showFrames(frames, frame_sz);
            printf("(fault)");
            page_faults += 1;
        }
        //showRefBits(page_ref_bits, page_max);
        printf("\n");
    }
    return page_faults;
}
