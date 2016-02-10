

#include <algorithm>
#include <iterator>
#include <cstring>
#include <cstdlib>
#include <cstdio>
 
// Radix sort comparator for 32-bit two's complement integers
class radix_test
{
    const int bit; // bit position [0..31] to examine
public:
    radix_test(int offset) : bit(offset) {} // constructor
 
    bool operator()(int value) const // function call operator
    {
        if (bit == 31) // sign bit
            return value < 0; // negative int to left partition
        else
            return !(value & (1 << bit)); // 0 bit to left partition
    }
};
 
// Least significant digit radix sort
void lsd_radix_sort(int *first, int *last)
{
    for (int lsb = 0; lsb < 32; ++lsb) // least-significant-bit
    {
        std::stable_partition(first, last, radix_test(lsb));
    }
}
 
// Most significant digit radix sort (recursive)
void msd_radix_sort(int *first, int *last, int msb = 31)
{
    if (first != last && msb >= 0)
    {
        int *mid = std::partition(first, last, radix_test(msb));
        msb--; // decrement most-significant-bit
        msd_radix_sort(first, mid, msb); // sort left partition
        msd_radix_sort(mid, last, msb); // sort right partition
    }
}


int main()
{
    char *line = NULL;
    size_t size;
    int read;
    int p;
    int *nums = (int*)malloc(10000000*sizeof(int));
    int c = 0;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            p = atoi(&(line[2]));
            //printf("%d\n", p);
        } else if (*line == 'q') {
            char *tok = strtok(line, " ");
            tok = strtok(NULL, " ");
            while (tok != NULL) {
                nums[c++] = atoi(tok);
                tok = strtok(NULL, " ");
            }
        }
    } 

    //lsd_radix_sort(nums, nums + c);
    msd_radix_sort(nums, nums + 8);

    printf("s ");
    int j;
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");
    free(nums);
    return 0;
}
