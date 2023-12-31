/*
 * Usage: test_huff_dec [-v] [-e]
 *
 *  -v  Verbose
 *  -e  Run expensive tests
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <getopt.h>
#else
#include <unistd.h>
#endif

#include "lsqpack.h"
#include "lsqpack-test.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int verbose;

struct test_huff_dec
{
    int             lineno;

    /* Input */
    unsigned char  *src;
    size_t          src_sz;

    /* Output */
    char           *dst;
    size_t          dst_sz;
};

const struct test_huff_dec tests[] =
{
    { __LINE__,
      (unsigned char *)
      "\xbf\x6c\x85\xfa\x53\xc4\xe5\xb0\xaa\x19\x0a\x53\xe9\x42\x0e\xcf\xf3",
      17,
      "Dude, where is my car?",
      22,
    },
    { __LINE__,
      (unsigned char *)
                        "\xd6\x32\x1a\xa9\x94\x8c\x64\xea\xa0\xea\x91\x49"
        "\x4f\x62\x67\x51\x33\x95\x4a\x0b\x54\xc9\x9d\x43\xca\xa2\x67\x2a"
        "\x93\x8d\x24\x9f\xc0\xfd\x21\x41\xd5\x22\x88\x1f\x71\xcc\x5a\x3d"
        "\x28\x49\xcb\xe0\xd5\x4d\xff\xff\xff\xe2\x67\x2a\x85\xaa\x45\x0f"
        "\x2a\x83\x50\x86\x98\x76\x7d\x29\xe0\x68\x51\x33\x95\x4a\x47\x52"
        "\x9e\x27\x3a\x0a\xa1\x06\x8b\x47\x42\x93\x50\x3a\x88\x55\x27\x1c"
        "\x8a\x58\x54\x12\xd2\xc8\xa5\x25\x52\x5b\x0f\x4f\xff\xff\xff\x89"
        "\x9c\xaa\x20\xa3\x5d\x45\x31\x99\x0a\x4e\x53\x4d\x3a\x54\xf0\x34"
        "\x29\x5d\x87\x8c\x71\xd1\xe9\x49\x4d\xdc\xaa\x4a\x52\xa5\x44\x96"
        "\xa5\x0d\x51\x27\x2a\x3e\x94\x3d\x8a\x24\xb5\x28\x3a\xa4\x50\x7f"
        "\xff\xff\xfe\x4e\x3a\x25\xfb\x52\x3b\x52\xa5\x3e\x94\x94\xd6\x21"
        "\x2a\x1a\x9a\xec\x2a\x10\x63\xd5\xf4\xa1\xec\x52\x52\x96\x85\x0f"
        "\x2a\x89\x9c\xaa\x52\x3a\xbe\x94\xf0\x34\x29\x51\xd2\xa1\xe5\x51"
        "\x33\x99\x1f\x4a\x47\x6a\x7f\xff\xff\xfc\x3c\xaa\x4e\x64\x28\x84"
        "\xd8\x5a\xa6\x4c\xeb\xa8\xa4\x35\x48\xa7\xa2\xa7\xe9\x4f\x13\x9a"
        "\x82\xa9\x39\x54\xf0\x34\x28\x79\x54\xa4\x68\x41\xbb\x95\x48\xed"
        "\x35\x12\x7d\x29\xe0\xc9\x9d\x48\xec\x38\xe4\xff\xff\xff\xf1\x13"
        "\x9e\xda\x24\x2d\x88\x50\x75\x48\xa4\x85\x2d\x6a\x12\x72\xa1\x3f"
        "\x4a\x64\x50\x87\xb6\x89\x14\xa8\xe9\x50\x93\x8e\xc1\x91\x25\xb0"
        "\xde\xca\xa4\xe6\x42\x88\x4d\x85\xaa\x64\xce\xa0\xd1\xff\xff\xff"
        "\xe5\x23\x42\x0d\xdc\xab\xa8\xa6\x44\xa9\xe0\x68\x53\xc4\xe3\x4a"
        "\x94\x9a\x69\xd2\xa4\x65\x51\x25\xb2\x92\xc8\xa0\xd4\x41\xaa\x2f"
        "\x8f\x5f\x4a\x75\xa8\xe9\x4f\xa5\x10\x9b\x0b\x54\xc9\x9f\xe9\x43"
        "\xca\xa2\x67\x2f\xff\xff\xff\x9d\x4d\x54\x8a\x78\x2a\x83\x41\x2c"
        "\x34\x65\x51\x27\x52\x82\x8e\xa5\x07\x54\x8a\x78\x35\x9e\x94\xa4"
        "\xb5\x7d\x29\x1d\xa9\x53\xc4\xe6\x24\xff\x4a\x1a\xa5\x27\x35\x3f"
        "\x4a\x46\x52\x0e\xd4\x15\x43\xca\xa4\xe6\x47\xff\xff\xff\x93\x94"
        "\x7d\xfd\x29\x1d\xa6\xa2\x4f\xa5\x2b\x1d\x89\x39\xfa\xa9\x49\xec"
        "\x2a\x87\x95\x44\xce\x55\x0b\x55\x03\xb2\x62\xc8\xa4\xc7\xb0\xd4"
        "\x50\x35\x0f\x64\x85\xb1\x75\x14\xd2\x74\xa8\x99\xc6\x95\x0d\x57"
        "\xff\xff\xff\x83\xae\xb2\x8e\xc1\xd4\x42\xa9\x39\x54\x41\x4b\x49"
        "\x64\x50\xd5\x28\x99\xca\xa5\x05\x1a\x12\xa4\xc7\xb0\xd4\x50\x35"
        "\xa8\x37\x52\xae\xa2\x9c\xa7\x1a\x54\xc8\xa0\xe9\x51\x09\xb0\xdd"
        "\xcd\x54\xca\x24\xff\xff\xff\xfe\x17\xcd\x76\x15\x08\x50\xc8\x51"
        "\x33\x99\x0a\x21\x36\x16\xa9\x93\x3a\x86\x4a\x0b\x44\xbf\x4a\x52"
        "\x7b\x0a\xa0\xd0\xa0\xd4\x4c\xe6\xaa\x65\x07\x58\xec\x4a\x92\xd8"
        "\x7a\x54\x9c\xc8\x52\xb9\xfd\x20\xc4\x1d\x1f\xff\xff\xfe\x20\xb4"
        "\xc7\x40\xea\x21\x57\x51\x4c\x89\x53\xc0\xd0\xa0\xd4\x42\x6c\x2d"
        "\x53\x26\x75\x3c\x15\x41\xd8\x55\x3c\x1e\xa4\xa8\x93\xa8\x34\x20"
        "\xe4\x30\xd2\x55\x3c\x19\x33\xa8\x99\xcd\x54\xc8\xff\xff\xff\xf2"
        "\xbb\x0d\x49\x92\x6e\xe5\xfa\x53\xc1\x93\x3a\x9e\x0d\x44\x8a\x0e"
        "\xa3\x52\x3a\x11\xf4\xa0\xea\x91\x44\xce\x55\x09\x61\x46\x9b\x6c"
        "\x2a\x14\xf0\x55\x0d\x48\xe6\x35\x45\x50\xf6\xd9\xff\xff\xff\xe2"
        "\x6c\x29\x56\x93\xc1\x68\xa0\xd5\x4c\xa5\x76\x1d\x27\x4f\xaa\xca"
        "\x85\x12\x75\x27\x1f\x72\xa9\x19\x4b\x57\xff\xf6\xff\xff\x37\xff"
        "\xff\x70\xd4\x42\x6c\x2d\x53\x26\x75\x10\x3e\xe3\x98\xbf\x4a\x4a"
        "\x5b\x0e\x43\x1e\xd4\x7d\x28\x3a\x0d\xdc\xbf\xff\xff\xfe\x1a\xa5"
        "\x0c\x94\x16\x89\x7e\x94\x4c\xe5\x50\xa8\x41\x6a\x21\x54\x3c\xaa"
        "\x50\x69\x4a\xa1\xaa\x51\x33\x8d\x2a\x19\x2a\x19\x0a\x26\x72\xa9"
        "\x59\xd2\x5a\x89\xe9\x43\xca\xa5\x27\x49\x8f\x57\xd2\x89\x9c\xbf"
        "\xff\xff\xfe\x16\x82\xd2\x5a\x92\x3a\x14\x42\x6d\x96\x55\x0c\x94"
        "\x16\x89\x54\x3d\xa9\x50\xf2\xa9\xe2\x73\x12\x75\x13\x39\x54\xa4"
        "\x75\x7a\x52\x53\xd9\x4a\x14\x3c\xaa\x50\x69\x4a\xa4\xe3\xee\x55"
        "\x23\x29\x6a\xff\xff\xff\xf2\x93\xdb\x44\x85\x93\xed\x43\x54\xa2"
        "\x27\x3d\x89\xfa\x51\x33\x8d\x2a\x78\x9c\xc4\x9d\x4f\x16\x19\x33"
        "\x95\x0a\x1a\xa5\x13\x39\x54\x8c\xf2\x7a\x50\xf2\xa8\x35\x11\x50"
        "\xfa\x95\x4f\x13\x96\xa5\x13\x39\x7f\xff\xff\xfc\x9c\xa3\x91\x43"
        "\x21\x42\x5a\x95\x0f\x2c\xbf\x4a\x0e\xa9\x14\x4c\xe5\x51\x15\x0f"
        "\xa9\x7e\x94\x1a\x14\x1a\x88\xa8\x7d\x4b\xf4\xa1\x90\xa4\x85\x1c"
        "\x9f\x4a\x1e\xc5\x3c\x4e\x62\x4e\xa5\x06\xaa\x62\xd8\x85\x0d\x57"
        "\xff\xff\xff\x89\x9c\xaa\x22\x71\xd6\x5a\x0a\x84\x29\x45\xb4\xd6"
        "\xa1\xe5\x51\x36\xd8\x9a\x0a\xb5\x25\x1a\x54\x1d\x52\x29\x61\x48"
        "\x73\x50\x85\x07\x54\x8a\x76\xb4\xdd\xcb\x62\x14\x96\xc3\xd2\xa2"
        "\x67\x2a\x95\xd8\x79\x3f\xff\xff\xfc\x3c\xaa\x0d\x49\x4d\x54\xc5"
        "\xb1\x7f\xff\xff\xff\x3f\xff\xff\xfc\xdd\x69\x27\x53\xc0\xd0\xa2"
        "\x67\x2a\x86\xa6\xbb\x0a\x84\x18\xf5\x28\x79\x54\x42\x6c\x2d\x53"
        "\x26\x75\x32\x29\x30\xd3\x39\x6c\x2c\x8a\x4b\x61\xe9\x51\x33\x99"
        "\x0a\x52\x3a\x94\xf1\x39\xd4\xac\x64\x2c\x8a\x5b\x5f\xff\xff\xff"
        "\x83\xaa\x45\x24\x3f\x8a\x97\x51\x4c\x65\x53\xc0\xd0\xa4\xa6\xb2"
        "\x9a\x3d\x29\x5d\x03\xa9\x25\x91\x43\xd4\xa4\xe6\x42\x94\x16\x64"
        "\x7d\xa9\x39\x90\xa4\xa5\x2a\x54\x42\x6c\xb4\x9d\x54\x4c\xe5\x52"
        "\x42\x93\xaf\xff\xff\xff\x88\xed\x68\xec\x2d\x1e\x94\x1d\x52\x29"
        "\xe0\xc9\x9d\x44\x5b\x61\x53\xeb\xed\x42\xf7\x2d\x9e\x94\xa4\xfd"
        "\xcb\x49\x6a\x4a\x87\x95\x41\xa9\x4d\xa8\x25\x05\xfa\x52\x5b\x0f"
        "\x4a\x89\x9c\xaa\x4e\x51\xf7\x2a\x87\x97\xff\xff\xff\xc4\xce\x55"
        "\x11\x39\xed\xa2\x42\xd8\x85\x12\x75\x13\x39\x54\x49\xa6\x9d\x25"
        "\xa8\xd5\x4c\xa1\xe5\x51\x33\x95\x4a\x0d\x5a\x14\x1c\x67\xb5\x2a"
        "\x26\x72\xa8\x43\x4c\x3b\x3e\x94\xf0\x34\x7f\xff\xff\xf9\x21\x48"
        "\x64\x1b\xb9\x7e\x94\x1d\x52\x28\x82\x96\x92\xc8\xa2\x4e\xa1\x0f"
        "\x49\x54\x3d\xa9\x50\xf2\xa8\x35\x10\x9b\x0b\x54\xc9\x9d\x44\xce"
        "\x34\xa9\xe0\x68\x50\xbe\x48\x54\x20\xdd\xca\xa0\xea\x93\xff\xff"
        "\xff\xc3\xf7\x2d\x9e\x27\x2d\x14\x9a\xa9\x97\x51\x4c\x95\x29\x28"
        "\xc8\x9f\xa5\x13\x39\xed\x9a\x75\x13\x39\x90\xa2\x13\x61\x6a\x99"
        "\x33\xa9\x59\x6c\xee\x39\x0b\x22\x85\xee\x5b\x3d\x28\x32\x24\xc7"
        "\xa9\x43\xcb\xff\xff\xff\xe4\xe6\x47\xd2\x86\x4a\x88\x29\x69\x2c"
        "\x8a\x47\x6a\x54\x4c\xe5\x50\x72\x77\x2d\x89\x32\x0b\x49\x6a\x4a"
        "\x87\x95\x41\xa9\x35\x85\x1a\x4b\x62\x88\x4d\x85\xaa\x64\xce\xa2"
        "\x67\x1a\x54\xa2\xdb\x3a\x96\x4f\xff\xff\xff\x3c\x19\x33\x9a\xaf"
        "\xa5\x13\x38\xd2\xa5\x03\xf4\xa4\x87\xb2\x91\xd4\x95\x07\x54\x8a"
        "\x54\x75\x29\x3d\x85\x51\x33\x8e\xa5\x10\x93\x59\x61\x64\x52\x5b"
        "\x0f\x4a\x89\x35\x25\x51\x27\x51\x26\xa4\xbf\x4a\x47\x6a\x7f\xff"
        "\xff\xfc\xf1\x39\x89\x3a\x94\x9a\x69\xd2\xa0\xec\x3d\xa8\x2f\xd2"
        "\x83\x4a\x83\xab\xd2\x94\x9e\x92\xd4\x9f\xa5\x12\x5b\x2c\x34\x74"
        "\x15\x41\xd5\x22\x84\x3d\x35\x96\x8a\x0d\x54\xdf\x4a\x50\x6e\xa5"
        "\x51\x33\x95\x4b\x07\x31\x7f\xff\xff\xfc\x3c\xaa\x0d\x4a\x0c\x7a"
        "\x94\x3d\x8a\x26\x72\xa9\xe2\xc1\xa6\x75\x0f\x2a\x83\x51\x09\x3d"
        "\x94\xaf\xff\xff\xff\xe7\xff\xff\xff\x9b\xe7\x2a\x84\x39\xfa\xa8"
        "\x84\xda\x4e\xaa\x4e\x64\x29\x39\x47\x22\x87\xb5\x2a\x1e\x55\x13"
        "\x39\x54\x98\x74\x50\x5f\x4a\x48\x73\xd8\xa0\xea\x91\x49\xac\x35"
        "\x55\x16\x45\x0b\x51\x0f\x6d\x83\x98\xd5\x4d\x47\xaf\xff\xff\xff"
        "\x06\x95\x29\x2f\xd2\x83\x4a\x89\x9c\xaa\x20\x74\x95\x44\x9a\x92"
        "\xa9\xd0\xb6\x75\x35\x53\x29\x39\x90\xa2\x67\xb6\x98\xd4\x35\x4a"
        "\x26\x72\xa9\x21\xac\x29\x12\x63\xd4\xa1\xe5\x51\x33\x95\x4a\x47"
        "\x52\x9e\x27\x3f\xff\xff\xff\x95\x8c\x85\x91\x4b\x6b\x50\x75\x48"
        "\xa4\x87\xf1\x52\x91\xfa\x51\x33\x95\x49\xc6\x92\x4f\xe0\x7e\x97"
        "\x51\x4d\xf3\xda\x85\x32\x29\xe0\x68\x52\x63\x77\x2d\x4a\x24\xea"
        "\x5b\x54\x85\xb1\x09\x1d\x52\x28\x99\xc6\x9f\xff\xff\xff\x27\x2a"
        "\x9e\x06\x85\x13\x39\x54\x20\xeb\x48\xcd\x57\xd2\x89\x9c\xaa\x7f"
        "\xff\x6f\xff\xf3\x7f\xff\xb3\xaa\x89\x14\xd0\x3a\xbe\xbf\xff\xb7"
        "\xff\xf9\xbf\xff\xf3\x28\x6a\x94\x4c\xe5\x50\x87\x3f\x5f\xff\xed"
        "\xff\xfe\x6f\xff\xee\x21\x4e\xe5\xb2\xa1\x92\xda\x07\x67\xd2\x89"
        "\x9c\xbf\xff\xff\xfe\x1a\xa9\x0d\xdc\xd2\x5a\x3a\x14\xf1\x39\xe9"
        "\x53\x22\x94\xda\x84\xa8\x6a\x92\x5b\x3b\x98\xbe\x14\x1d\x52\x29"
        "\x5d\xa9\x51\x27\x51\x33\x95\x44\xd8\x7b\x63\xa0\xaa\x1e\x55\x10"
        "\x7a\x4b\x39\xfc\x7f\xff\xff\xf9\x31\x52\x93\x55\x32\x94\x95\x41"
        "\xa2\x73\xd8\x55\xd4\x53\x22\x93\x8e\x45\x27\x1d\x12\xa8\x84\x8e"
        "\xc4\x96\x45\x25\x3d\x9e\x07\x64\x9f\x4a\x24\xea\x4c\x54\xa8\x7e"
        "\xe5\xb1\x4f\x06\x4c\xea\x78\x9c\x69\x53\x27\xff\xff\xff\x9e\x06"
        "\x85\x08\x5b\x12\x33\x54\xa7\x83\xdb\x44\x8a\x46\x55\x06\xa2\x12"
        "\x7b\x29\xf4\xa4\xa6\xee\x55\x29\x35\x56\xa4\xa8\xfa\x53\xc4\xe5"
        "\xa9\x41\xa9\x49\xec\x2a\x9d\xcc\x7a\x0b\x52\x7f\xff\xff\xfc\x45"
        "\xb2\xca\x72\x0d\x26\xaa\x65\x2b\x1d\x87\xf3\xe9\x14\xa8\x82\x9b"
        "\xd9\x64\x51\x33\x95\x4b\x6a\x94\xf6\x26\xda\x86\x92\xa9\x59\x6c"
        "\x41\xea\x53\xc4\xe7\x53\xc0\xd0\xa5\x1e\x8d\x54\xca\x1e\xa5\x27"
        "\x32\x3f\xff\xff\xfc\x8c\x64\xea\xba\x8a\x63\x2a\x9e\x2c\x2d\x44"
        "\x9c\xb2\x28\x3a\xa4\x53\xc5\x86\x4c\xe5\x91\x41\xc6\x7b\x52\xa1"
        "\x0f\x57\x7b\x68\x41\xbb\x96\x8f\x4b\xa8\xa6\xf9\xca\xa1\x27\x35"
        "\x5f\x4a\x78\x32\x67\x51\x33\x97\xff\xff\xff\xc9\x07\x4d\x6a\x47"
        "\x40\xc9\xd5\x48\xca\x3b\x24\xfa\x52\xb3\x9a\xa4\x96\x45\x27\x34"
        "\xd3\x96\xc5\x0d\x52\x89\x9c\xaa\x0c\xd6\x28\x34\x28\x99\xca\xa4"
        "\x63\x27\x55\x29\xb5\x04\xa0\xa8\x51\x09\x34\xb2\x96\xa2\xc9\xff"
        "\xff\xff\xe0\xea\x91\x44\xce\x55\x09\x39\x50\x95\x11\xe0\xb4\x50"
        "\x59\x14\x35\x4a\x0e\xa5\x2d\xa8\x87\xa9\x04\x31\xed\x42\x83\xaa"
        "\x45\x0d\x52\x12\x6a\x88\x93\x77\x2a\x85\x96\x53\xd8\x95\x12\x75"
        "\x26\x2a\x7f\xff\xff\xfc\xa4\xf6\x15\x41\x9a\xc5\xd4\x53\x85\x52"
        "\x16\xc5\x13\x39\x54\xf1\x39\x91\xd4\xb6\x23\xe9\x41\xd5\x22\x83"
        "\xa2\x85\x2d\xa9\x05\x2d\x5f\x4a\x64\x53\xad\x45\xf0\xa2\x67\x1a"
        "\x54\x4c\xe5\x51\x1d\x4d\x52\x9e\x06\x8f\xff\xff\xff\x12\x3e\xa6"
        "\xaa\x65\x0f\x52\x83\x52\xbb\x6c\xae\x83\x22\x75\x27\xb4\xab\xff"
        "\xff\xff\xf9\xff\xff\xff\xe6\xf9\xca\xa1\x07\x5a\x46\x6a\xbe\x94"
        "\x3d\x8a\x72\x3d\x12\xa9\x9c\x76\x20\xb5\x7d\x28\x34\x29\x49\x6a"
        "\x50\x83\xa2\x82\xc8\xa4\xe6\xa7\xe9\x42\x14\x68\x2c\x8a\x56\x32"
        "\x1a\xa9\x94\x1d\x52\x29\x30\xfd\x96\x4f\xff\xff\xff\x24\x3f\x8a"
        "\x94\x1a\x54\x4c\xe5\x52\x4f\x46\xaa\x65\x29\x1d\x4b\xa8\xa6\xe3"
        "\xa9\x29\x8b\x61\x0a\xa4\xe3\x91\x44\xce\x64\x29\x29\xaa\x1d\x0a"
        "\x21\x36\x5b\x34\xd4\x15\x48\xca\x43\xd2\x55\x13\x38\xd2\xa2\x67"
        "\x2f\xff\xff\xff\x88\x19\xa8\x3d\x8a\x56\x3b\x50\x59\x14\x35\x4a"
        "\x26\x72\xa8\x32\x25\x43\xca\xa4\xb4\x1a\xa9\x8d\x54\xca\x52\x7b"
        "\x0a\xa7\x81\xa4\xb6\x28\x7e\xe5\xb1\x49\xcd\x4a\x83\xaa\x45\x10"
        "\x91\xd8\x59\x3f\xff\xff\xfc\x25\xb6\x18\xf6\xa2\x8f\x5f\x4a\x26"
        "\x72\xa8\x41\xd5\xdc\x68\x52\x3b\x49\xd4\xa9\x52\xb1\xd8\x9a\x3d"
        "\x28\x93\x50\x92\xc8\xa0\xea\x91\x49\x2c\x35\x75\x9a\xa9\x94\x32"
        "\x50\xa1\x0f\x52\x4b\x52\x50\xa2\x4f\xff\xff\xff\xe2\x67\x2a\x92"
        "\x14\x9d\x57\x51\x4d\xf3\x95\x49\x3d\x1a\xa9\x94\xa4\x75\x29\x19"
        "\x46\x95\x06\xa2\x46\x94\x9c\xea\x1e\xa5\x13\x39\x54\x9c\x69\x24"
        "\xea\x78\x32\x67\x52\x73\x21\x49\xca\x5a\x11\xf5\xff\xff\xff\xe2"
        "\x13\x60\xcd\x34\xe9\x2d\x45\x91\x43\xda\x95\x27\x32\x14\xa0\xb3"
        "\x23\xe9\x41\xd5\x22\x88\x49\xa5\x94\xb5\x16\x45\x0d\x52\x87\xa8"
        "\xaa\x4d\x61\x46\x95\x12\x5a\x90\x55\x0b\x2c\xa7\xb1\x3f\x4a\x0e"
        "\xa9\x3f\xff\xff\xfc\xb0\xf4\x50\x59\x14\x9c\xc8\x52\x72\x8e\x45"
        "\x25\xb0\xf4\xa8\x83\x48\x55\x12\x75\x10\x69\x0a\xba\x8a\x6f\x9c"
        "\xb5\x28\x99\xca\xa5\x36\xa0\x94\x15\x0a\x58\x5a\x07\xe4\xb2\x7d"
        "\x28\x99\xca\xa4\xe5\x1c\x9f\xff\xff\xfe\x21\x27\xae\xb2\xc8\xa5"
        "\x87\xa2\x83\x55\x37\xd2\x83\xaa\x45\x06\xa2\x0d\x34\xff\x4a\x0d"
        "\x0a\x1e\x55\x2b\xb0\xf2\x9e\xda\xa4\x52\xc2\xd0\x62\xcb\xf4\xa4"
        "\xb4\x1c\x69\x2c\x8a\x5b\x5f\x81\xd9\x22\x92\xd8\x7a\x7f\xff\xff"
        "\xfc\x9c\xc8\x52\x83\x56\x85\xd4\x53\x7c\xe5\x53\xa0\xfc\x29\x25"
        "\x87\xae\xb2\xc9\xf4\xa2\x67\x2a\x96\xd7\x59\x6c\x52\x83\x56\xa5"
        "\x06\x95\x25\x93\xe9\x41\xd5\x22\x89\xf0\x75\x2c\x3f\x84\x28\x79"
        "\x7f\xff\xff\xfc\x49\xe3\x19\x08\x75\xa4\x32\x08\x7a\x0f\x6d\x85"
        "\x91\x44\x94\xa9\x9d\x41\xd7\x59\x47\x61\x64\x5d\x45\x32\x25\x44"
        "\x14\xb4\x96\x45\x06\x85\x13\x39\xed\x9a\x75\x27\x32\x14\x94\xa3"
        "\x4d\xb6\x15\x0a\x4e\x39\x3f\xff\xff\xfc\x96\xc3\xfb\x2d\x4a\x1a"
        "\xa4\x9d\x41\xa9\x21\x87\x19\xe8\x31\x07\x42\x93\x58\x6a\x94\x1a"
        "\x54\x4c\xe5\x53\xc1\xec\xa2\x45\x27\x2a\x93\x8e\x45\x28\x2c\xa9"
        "\x50\x75\x48\xa1\xed\x4f\x83\x25\x25\x91\x7f",
        12 + 16 * 174 + 11,
        "Pacing back and forth the length of the hatchways and savagely chewing\n"
        "the end of a cigar, was the man whose casual glance had rescued me from\n"
        "the sea.  His height was probably five feet ten inches, or ten and a\n"
        "half; but my first impression, or feel of the man, was not of this, but\n"
        "of his strength.  And yet, while he was of massive build, with broad\n"
        "shoulders and deep chest, I could not characterize his strength as\n"
        "massive.  It was what might be termed a sinewy, knotty strength, of the\n"
        "kind we ascribe to lean and wiry men, but which, in him, because of his\n"
        "heavy build, partook more of the enlarged gorilla order.  Not that in\n"
        "appearance he seemed in the least gorilla-like.  What I am striving to\n"
        "express is this strength itself, more as a thing apart from his physical\n"
        "semblance.  It was a strength we are wont to associate with things\n"
        "primitive, with wild animals, and the creatures we imagine our\n"
        "tree-dwelling prototypes to have been—a strength savage, ferocious, alive\n"
        "in itself, the essence of life in that it is the potency of motion, the\n"
        "elemental stuff itself out of which the many forms of life have been\n"
        "moulded; in short, that which writhes in the body of a snake when the\n"
        "head is cut off, and the snake, as a snake, is dead, or which lingers in\n"
        "the shapeless lump of turtle-meat and recoils and quivers from the prod\n"
        "of a finger.\n"
        "\n"
        "Such was the impression of strength I gathered from this man who paced up\n"
        "and down.  He was firmly planted on his legs; his feet struck the deck\n"
        "squarely and with surety; every movement of a muscle, from the heave of\n"
        "the shoulders to the tightening of the lips about the cigar, was\n"
        "decisive, and seemed to come out of a strength that was excessive and\n"
        "overwhelming.  In fact, though this strength pervaded every action of\n"
        "his, it seemed but the advertisement of a greater strength that lurked\n"
        "within, that lay dormant and no more than stirred from time to time, but\n"
        "which might arouse, at any moment, terrible and compelling, like the rage\n"
        "of a lion or the wrath of a storm.\n"
        "\n"
        "The cook stuck his head out of the galley door and grinned encouragingly\n"
        "at me, at the same time jerking his thumb in the direction of the man who\n"
        "paced up and down by the hatchway.  Thus I was given to understand that\n"
        "he was the captain, the “Old Man,” in the cook’s vernacular, the\n"
        "individual whom I must interview and put to the trouble of somehow\n"
        "getting me ashore.  I had half started forward, to get over with what I\n"
        "was certain would be a stormy five minutes, when a more violent\n"
        "suffocating paroxysm seized the unfortunate person who was lying on his\n"
        "back.  He wrenched and writhed about convulsively.  The chin, with the\n"
        "damp black beard, pointed higher in the air as the back muscles stiffened\n"
        "and the chest swelled in an unconscious and instinctive effort to get\n"
        "more air.  Under the whiskers, and all unseen, I knew that the skin was\n"
        "taking on a purplish hue.\n"
        "\n"
        "The captain, or Wolf Larsen, as men called him, ceased pacing and gazed\n"
        "down at the dying man.  So fierce had this final struggle become that the\n"
        "sailor paused in the act of flinging more water over him and stared\n"
        "curiously, the canvas bucket partly tilted and dripping its contents to\n"
        "the deck.  The dying man beat a tattoo on the hatch with his heels,\n"
        "straightened out his legs, and stiffened in one great tense effort, and\n"
        "rolled his head from side to side.  Then the muscles relaxed, the head\n"
        "stopped rolling, and a sigh, as of profound relief, floated upward from\n"
        "his lips.  The jaw dropped, the upper lip lifted, and two rows of\n"
        "tobacco-discoloured teeth appeared.  It seemed as though his features had\n"
        "frozen into a diabolical grin at the world he had left and outwitted.",
        3687,
    },
    {   __LINE__,
        (unsigned char *)
                        "\xe5\x39\x6a\x50\xd5\x28\x99\xca\xa5\xe3\xdb\x62"
        "\x0a\xa1\xe5\x52\x7b\x69\x1d\x4a\x17\xb9\x6a\x4a\x14\x32\x54\x8c"
        "\xa4\x3d\x25\x42\x95\x14\x85\x42\x07\x67\xa5\x25\x3d\x8a\x1e\xa2"
        "\xa9\x59\x4f\x5d\x05\x51\x27\x52\x43\x21\x07\xa3\xb9\x54\x4c\xe5"
        "\x52\xb3\xd0\x64\x98\x83\xa1\x48\xc7\x54\x88\x53\xc4\xe6\x24\xea"
        "\x4e\x3e\xe5\x50\x87\xaa\xa2\x91\x25\x91\x44\xce\x5a\x54\xf0\x64"
        "\xce\xa0\xea\x3a\x67\x2d\x8a\x0e\xa9\x14\x49\xd4\x1a\x11\x6d\x25"
        "\x50\x74\x9e\xa9\x94\x4c\xe5\x52\xb3\xf8\x2d\x88\x50\xf2\xa8\x99"
        "\xca\xa1\x47\x62\x67\xfa\x51\x33\x95\x44\x16\xb1\xd8\x34\x95\x41"
        "\xd5\x22\x85\xed\x68\xe8\x51\x09\x1a\x4c\x7a\x94\x49\xd4\xf1\x39"
        "\x89\x3a\x89\x9c\xaa\x67\x1f\x84\x28\x79\x54\xd2\x34\xdb\x61\x54"
        "\x1d\x52\x28\x79\x54\xd2\x34\xdb\x61\x7f\xd2\x14\xc4\x79\x14\x2d"
        "\x49\x32\x68\x2a\x89\x9c\xb4\xfd\x28\x35\x24\x29\x0b\x52\x54\xb0"
        "\xa8\xac\xa4\x4a\x89\x3a\x89\x9c\xaa\x1e\xb3\x54\x63\xd4\x85\x0f"
        "\x2a\x94\x8e\xae\xa6\xaa\x45\x2c\x2f\x6b\x4d\x61\x50\xa2\x67\x1a"
        "\x54\x4c\xe5\xf4\xa2\x27\x3d\xb4\x48\xa4\x85\x25\x03\xb0\xaa\x26"
        "\x72\xa8\x41\xda\x82\xa1\x4f\x13\x98\x93\xa8\x6a\x6b\x2d\x0a\x26"
        "\x72\xd2\xa2\x4e\xa2\x67\x2a\x88\x2d\x63\xb0\x69\x31\xea\x5f",
        12 + 16 * 16 + 15,
        "When in the Course of human events it becomes necessary for one "
        "people to dissolve the political bands which have connected them "
        "with another and to assume among the powers of the earth, the "
        "separate and equal station to which the Laws of Nature and of "
        "Nature's God entitle them, a decent respect to the opinions of "
        "mankind requires that they should declare the causes which impel "
        "them to the separation.",
        404,
    },
};

void
run_test (const struct test_huff_dec *test)
{
    struct huff_decode_retval retval;
    struct lsqpack_huff_decode_state state;
    unsigned in_chunk_sz, out_chunk_sz, in_off, out_off, n_to_read, n_to_write;
    char output[0x1000];

    if (verbose)
        fprintf(stderr, "Run test on line %u\n", test->lineno);

    for (in_chunk_sz = 1; in_chunk_sz <= test->src_sz; ++in_chunk_sz)
    {
        if (verbose)
            fprintf(stderr, "chunk %u out of %zu\n", in_chunk_sz, test->src_sz);
        for (out_chunk_sz = 1; out_chunk_sz <= test->dst_sz; ++out_chunk_sz)
        {
            in_off = 0;
            out_off = 0;
            state.resume = 0;
            n_to_read = MIN(test->src_sz, in_chunk_sz);
            n_to_write = MIN(test->dst_sz, out_chunk_sz);

            do
            {
                assert(in_off + n_to_read <= test->src_sz); /* self-test */
                retval = lsqpack_huff_decode_full(test->src + in_off, n_to_read,
                        (unsigned char *) output + out_off, n_to_write, &state,
                        test->src_sz == in_off + n_to_read);
                switch (retval.status)
                {
                case HUFF_DEC_OK:
                    in_off += retval.n_src;
                    out_off += retval.n_dst;
                    assert(test->dst_sz == out_off);
                    assert(test->src_sz == in_off);
                    assert(0 == memcmp(test->dst, output, out_off));
                    break;
                case HUFF_DEC_END_SRC:
                case HUFF_DEC_END_DST:
                    in_off += retval.n_src;
                    out_off += retval.n_dst;
                    n_to_write = MIN(sizeof(output) - out_off, out_chunk_sz);
                    n_to_read = MIN(test->src_sz - in_off, in_chunk_sz);
                    break;
                default:
                    assert(retval.status == HUFF_DEC_ERROR);
                    assert(0);
                }
            }
            while (in_off < test->src_sz);
            assert(retval.status == HUFF_DEC_OK);
        }
    }
}

int
main (int argc, char **argv)
{
    const struct test_huff_dec *test;
    int opt, run_expensive = 0;

    while (-1 != (opt = getopt(argc, argv, "ve")))
    {
        switch (opt)
        {
        case 'v':
            verbose = 1;
            break;
        case 'e':
            run_expensive = 1;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    for (test = tests; test < tests + sizeof(tests) / sizeof(tests[0]); ++test)
        if (run_expensive || test->src_sz * test->dst_sz < 150000)
            run_test(test);

    return 0;
}
