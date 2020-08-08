// Simulate LRU replacement of page frames

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// represent an entry in a simple inverted page table

typedef struct ipt_entry {
    int virtual_page;        // == -1 if physical page free
    int last_access_time;
} ipt_entry_t;


void lru(int n_physical_pages, int n_virtual_pages);
void access_page(int virtual_page, int access_time, int n_physical_pages, struct ipt_entry *ipt);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n-physical-pages> <n-virtual-pages>\n", argv[0]);
        return 1;
    }
    lru(atoi(argv[1]), atoi(argv[2]));
    return 0;
}


void lru(int n_physical_pages, int n_virtual_pages) {
    printf("Simulating %d pages of physical memory, %d pages of virtual memory\n",
          n_physical_pages, n_virtual_pages);
    struct ipt_entry *ipt = malloc(n_physical_pages * sizeof *ipt);
    assert(ipt);
    struct ipt_entry *inverted_page_table = malloc(n_physical_pages * sizeof *inverted_page_table);
    assert(inverted_page_table);
    for (int i = 0; i < n_virtual_pages; i++) {
        page_table[i].physical_page = -1;
        page_table[i].permissions = 0;
    }
    for (int i = 0; i < n_physical_pages; i++) {
        inverted_page_table[i].virtual_page = -1;
        inverted_page_table[i].last_access_time = 0;
    }
    int virtual_page;
    char action;
    for (int access_time = 0; scanf(" %c%d", &action, &virtual_page) == 2; access_time++) {
        // assert(virtual_page >= 0 && virtual_page < n_virtual_pages);
        //assert(strchr("RWrwU", action) != NULL);
        page_access(action, virtual_page, access_time, n_virtual_pages, n_physical_pages,
        page_table, inverted_page_table);
    }
}


// if virtual_page is not in ipt, the first free page is used
// if there is no free page, the least-recently-used page is evicted
//
// a single line of output describing the page access is always printed
// the last_access_time in ipt is always updated

void access_page(int virtual_page, int access_time, int n_physical_pages, struct ipt_entry *ipt) {

    // PUT YOUR CODE HERE TO HANDLE THE 3 cases
    //
    // 1) The virtual page is already in a physical page
    //
    // 2) The virtual page is not in a physical page,
    //    and there is free physical page
    //
    // 3) The virtual page is not in a physical page,
    //    and there is no free physical page
    //
    // don't forgot to update the last_access_time of the virtual_page


    int phy_using = 0;
    for (int temp_p = 0; temp_p < n_physical_pages; temp_p++) {
        if (ipt[temp_p].virtual_page == virtual_page) {
        phy_using = temp_p;
        break;
    }
    if (ipt[temp_p].last_access_time < ipt[phy_using].last_access_time) {
        phy_using = temp_p;
    }
    }
    int vp = ipt[phy_using].virtual_page;
    printf("Time %d: virtual page %d ", access_time, virtual_page);
    if (vp == virtual_page) {
        printf("->");
    } else if (vp == -1) {
        printf("loaded to");
    } else {
        printf(" - virtual page %d evicted - loaded to", vp);
    }
    printf(" physical page %d\n", phy_using);
    ipt[phy_using].virtual_page = virtual_page;
    ipt[phy_using].last_access_time = access_time;

}
