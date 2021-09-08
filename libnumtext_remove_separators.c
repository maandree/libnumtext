/* See LICENSE file for copyright and license details. */
#include "common.h"


ssize_t
libnumtext_remove_separators(char *outbuf, size_t outbuf_size, const char *num, size_t num_len, enum libnumtext_language lang)
{
	char *p = outbuf;
	ssize_t len = 0;

	switch (lang) {

	case LIBNUMTEXT_SWEDISH:
		for (; num_len--; num++) {
			if (*num != ' ' && *num != '\'' && *num != '.') {
				if (IS_UNICODE_NBSP(num, num_len)) {
					num = &num[sizeof(UNICODE_NBSP) - 2];
					num_len -= sizeof(UNICODE_NBSP) - 2;
					continue;
				}
				if (outbuf_size) {
					*p++ = *num;
					outbuf_size--;
				}
				len += 1;
			}
		}
		return len;

	default:
		errno = EINVAL;
		return -1;
	}
}
