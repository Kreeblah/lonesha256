/*
lonesha256.h test by comparison with known hashes of strings, also serves as usage example

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

/*
Compile using "gcc test.c" or equivalent, then run from command line to see the results.
If you find an error in this test or a test pair which lonesha256 fails, please open an issue.
*/

/* includes */
#define LONESHA256_STATIC
#include "lonesha256.h"
#include <stdio.h>

/* structs */
struct pair {
    const unsigned char* in; /* input string */
    const unsigned char out[32]; /* expected output */
};

/* known input/output pairs */
struct pair pairs[] = {
    /* 0: empty string */
    {
        (unsigned char *)"",
        {0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14,
        0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24,
        0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C,
        0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55}
    },
    /* 1: foobar string */
    {
        (unsigned char *)"foobar",
        {0xC3, 0xAB, 0x8F, 0xF1, 0x37, 0x20, 0xE8, 0xAD,
        0x90, 0x47, 0xDD, 0x39, 0x46, 0x6B, 0x3C, 0x89,
        0x74, 0xE5, 0x92, 0xC2, 0xFA, 0x38, 0x3D, 0x4A,
        0x39, 0x60, 0x71, 0x4C, 0xAE, 0xF0, 0xC4, 0xF2}
    },
    /* 2: quick brown fox */
    {
        (unsigned char *)"The quick brown fox jumps over the lazy dog",
        {0xD7, 0xA8, 0xFB, 0xB3, 0x07, 0xD7, 0x80, 0x94,
        0x69, 0xCA, 0x9A, 0xBC, 0xB0, 0x08, 0x2E, 0x4F,
        0x8D, 0x56, 0x51, 0xE4, 0x6D, 0x3C, 0xDB, 0x76,
        0x2D, 0x02, 0xD0, 0xBF, 0x37, 0xC9, 0xE5, 0x92}
    },
    /* 3: random string */
    {
        (unsigned char *)"dThNFRWWhyG7pINI",
        {0x64, 0x13, 0x46, 0x80, 0xDC, 0x26, 0x6E, 0x5D,
        0xFF, 0xE3, 0x83, 0xBD, 0xC4, 0xD1, 0x98, 0xC3,
        0xA3, 0x59, 0x2C, 0x36, 0xB4, 0x78, 0xCE, 0xBE,
        0xD6, 0x6C, 0x15, 0x8F, 0x78, 0x82, 0x5A, 0x28}
    },
    /* 4: longer string */
    {
        (unsigned char *)"laSQWLJtfvdfSvH9MaJC32tk59jpm3ZOkfQX2E308aUqgyqY"
        "Tq2hOyoLJaujn4ULaFDEb65ISERxjSqovIiccU1q8TxsBwm6"
        "qodWbY3mUdPL0YuRrN9RggDzkvHQz91XbpoItjPB7mxtzILt"
        "lypZtfJY3A3xh0B4ZrsRM2FTtcircLWRfDOpGIGbQcJoXeZq"
        "vKdvzrGbs6oG8wKAYKn51j3aQDNmENXGlYR06eRXFyctEU3g"
        "HDaXB6shHCa621wAIPn3ZjtaPzkYjFIQv2NwAnmLCSxOlhqs"
        "pCoaNVwHfHc6bc3c0Ani6f8Gto3lfrEMwLaPkxOEXuh4SK5w"
        "0qWQdahu4LNqoqT7KI7uuwXiMMzw6Y7ySK3u37VJ8msDNI6f"
        "0nRHEfROfhK2Yf3FVGItOcNBQDUa4BeuAJgt",
        {0xCD, 0x6F, 0x3B, 0x5E, 0x08, 0xC3, 0x41, 0x62,
        0xCB, 0xD5, 0x19, 0x0D, 0xA8, 0xCD, 0x45, 0xF7,
        0x63, 0x7E, 0x15, 0x13, 0x8A, 0x39, 0x62, 0x90,
        0x03, 0x68, 0x97, 0xB3, 0x18, 0x84, 0x26, 0xFE}
    },
    /* 5: 56 spaces */
    {
        (unsigned char *)"                                                        ",
        {0xB6, 0x0C, 0x4C, 0x2D, 0x56, 0x5C, 0xD1, 0xF1,
        0x0E, 0xEB, 0x0D, 0xEF, 0xFD, 0xD7, 0xB6, 0x5F,
        0xDF, 0x9D, 0x4F, 0xD3, 0x96, 0x08, 0x68, 0xDA,
        0x68, 0xE3, 0x53, 0x31, 0x59, 0xB7, 0x0B, 0xD8}
    }
};

/* main function */
int main (void) {
    unsigned long i = 0;
    /* runs through all the hard-coded pairs, then returns */
    for (i = 0; i < sizeof(pairs)/sizeof(pairs[0]); i++) {
        unsigned char out[32];
        if (lonesha256(out, pairs[i].in, strlen((const char *)pairs[i].in)))
            printf("TEST %d: ERROR\n", (int)i);
        else if (memcmp(out, pairs[i].out, sizeof(out)))
            printf("TEST %d: FAILED\n", (int)i);
        else
            printf("TEST %d: PASSED\n", (int)i);
    }
    /* return */
    return 0;
}
