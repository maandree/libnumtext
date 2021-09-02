/* See LICENSE file for copyright and license details. */
#include "common.h"

#define UNICODE_MINUS "−"


ssize_t
libnumtext_num2text(char outbuf[], size_t outbuf_size, const char *num, size_t num_len,
                    enum libnumtext_language lang, uint32_t flags, ...)
{
	size_t i;

	i = 0;
	if (i < num_len) {
		if (num[i] == '+' || num[i] == '-')
			i += 1;
		else if (!strncmp(&num[0], UNICODE_MINUS, sizeof(UNICODE_MINUS) - 1))
			i += sizeof(UNICODE_MINUS) - 1;
	}
	if (i == num_len)
		goto einval;
	for (; i < num_len; i++)
		if (!isdigit(num[i]))
			goto einval;

	switch (lang) {

	case LIBNUMTEXT_SWEDISH:
		return libnumtext_num2text_swedish__(outbuf, outbuf_size, num, num_len, flags);

	default:
	einval:
		errno = EINVAL;
		return -1;
	}
}
