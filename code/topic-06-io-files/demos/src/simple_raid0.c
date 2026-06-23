/*
 * simple_raid0.c — CST-315 · Topic 6 · demos · INSTRUCTOR ONLY
 * Purpose: Stripe a byte stream across two output files.
 *          The principle of RAID 0 in 40 lines.
 * Build:   make simple_raid0
 * Run:     ./simple_raid0
 * Notes:   Portable POSIX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    const char *data = "The quick brown fox jumps over the lazy dog.";
    int a = open("/tmp/raid0_a.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    int b = open("/tmp/raid0_b.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (a < 0 || b < 0) { perror("open"); return 1; }

    /* Stripe size = 4 bytes. */
    int stripe = 4;
    for (size_t i = 0; data[i]; i += stripe) {
        int n = 0;
        while (n < stripe && data[i + n]) n++;
        if ((i / stripe) % 2 == 0) write(a, data + i, n);
        else                       write(b, data + i, n);
    }
    close(a); close(b);

    printf("Original: %s\n", data);
    printf("Stripe A (/tmp/raid0_a.txt): %s\n",
           "(every other 4-byte stripe)");
    printf("Stripe B (/tmp/raid0_b.txt): %s\n",
           "(complementary stripes)");
    printf("\nTo reconstruct, interleave A and B in stripe order.\n");
    printf("Lose either file and you lose half the data — that's RAID 0.\n");
    return 0;
}
