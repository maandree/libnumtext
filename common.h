/* See LICENSE file for copyright and license details. */
#include "libnumtext.h"
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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

#if defined(__GNUC__)
# define PURE __attribute__((pure))
#else
# define PURE
#endif


#define UNICODE_MINUS "−" /* \xe2\x88\x92 */
#define UNICODE_NBSP  " " /* \xc2\xa0 */

#define IS_CHAR__(S, LEN, C) ((LEN) >= sizeof(C) - 1 && !memcmp((S), (C), sizeof(C) - 1))

#define IS_UNICODE_MINUS(S, LEN) IS_CHAR__(S, LEN, UNICODE_MINUS)
#define IS_UNICODE_NBSP(S, LEN)  IS_CHAR__(S, LEN, UNICODE_NBSP)

#define IS_UTF8_CHAR_CONTINUATION(B) (((B) & 0xC0) == 0x80)


struct common_num2text_params {
	char *outbuf;
	size_t outbuf_size;
	size_t sign_length;
	size_t number_offset;
	size_t trailing_zeroes;
};


int run(int argc, char *argv[], ssize_t (*callback)(char *outbuf, size_t outbuf_size, const char *num, size_t num_len));
int get_language(const char *arg, enum libnumtext_language *langp, int *have_langp);

int numtext_strip_main(int argc, char *argv[]);


ssize_t libnumtext_num2text_swedish__(struct common_num2text_params *params, const char *num, size_t num_len, uint32_t flags);
ssize_t libnumtext_card2ord_swedish__(char *outbuf, size_t outbuf_size, const char *num, size_t num_len, uint32_t flags);
