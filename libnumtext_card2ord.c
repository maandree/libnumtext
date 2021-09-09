/* See LICENSE file for copyright and license details. */
#include "common.h"


ssize_t
libnumtext_card2ord(char *outbuf, size_t outbuf_size, const char *num, size_t num_len,
                    enum libnumtext_language lang, uint32_t flags, ...)
{
	switch (lang) {

	case LIBNUMTEXT_SWEDISH:
		return libnumtext_card2ord_swedish__(outbuf, outbuf_size, num, num_len, flags);

	default:
		errno = EINVAL;
		return -1;
	}
}
