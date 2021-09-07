/* See LICENSE file for copyright and license details. */
#include "libnumtext.h"
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#if defined(__clang__)
# pragma clang diagnostic ignored "-Wcovered-switch-default"
# pragma clang diagnostic ignored "-Wpadded"
#endif


#if defined(__clang__)
# define FALL_THROUGH __attribute__((fallthrough));
#else
# define FALL_THROUGH
#endif


#define UNICODE_MINUS "−"

#define IS_UTF8_CHAR_CONTINUATION(B) (((B) & 0xC0) == 0x80)


ssize_t libnumtext_num2text_swedish__(char *outbuf, size_t outbuf_size, const char *num, size_t num_len, uint32_t flags);
