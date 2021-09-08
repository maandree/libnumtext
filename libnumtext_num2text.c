/* See LICENSE file for copyright and license details. */
#include "common.h"


ssize_t
libnumtext_num2text(char *outbuf, size_t outbuf_size, const char *num, size_t num_len,
                    enum libnumtext_language lang, uint32_t flags, ...)
{
	struct common_num2text_params params;
	size_t i;

	params.outbuf = outbuf;
	params.outbuf_size = outbuf_size;
	params.trailing_zeroes = 0;

	i = 0;
	if (num_len) {
		if (num[0] == '+' || num[0] == '-')
			i += 1;
		else if (IS_UNICODE_MINUS(num, num_len))
			i += sizeof(UNICODE_MINUS) - 1;
	}
	params.sign_length = i;
	if (i == num_len)
		goto einval;

	while (i + 1 < num_len && num[i] == '0')
		i += 1;
	params.number_offset = i;

	for (; i < num_len; i++) {
		if (!isdigit(num[i]))
			goto einval;
		else if (num[i] == '0')
			params.trailing_zeroes += 1;
		else
			params.trailing_zeroes = 0;
	}

	switch (lang) {

	case LIBNUMTEXT_SWEDISH:
		return libnumtext_num2text_swedish__(&params, num, num_len, flags);

	default:
	einval:
		errno = EINVAL;
		return -1;
	}
}
