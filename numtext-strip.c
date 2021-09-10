/* See LICENSE file for copyright and license details. */
#define LIBSIMPLY_CONFIG_MULTICALL_BINARY
#include "common.h"
#include <libsimple-arg.h>

USAGE("-l language");


static enum libnumtext_language lang;


static ssize_t
process(char *outbuf, size_t outbuf_size, const char *num, size_t num_len)
{
	ssize_t ret;
	ret = libnumtext_remove_separators(outbuf, outbuf_size, num, num_len, lang);
	if (ret < 0) {
		fprintf(stderr, "%s: libnumtext_remove_separators %s: %s\n", argv0, num, strerror(errno));
	} else {
		if ((size_t)ret < outbuf_size)
			outbuf[ret] = '\0';
		ret += 1;
	}
	return ret;
}


int
numtext_strip_main(int argc, char *argv[])
{
	int have_lang = 0;

	ARGBEGIN {
	case 'l':
		if (!get_language(ARG(), &lang, &have_lang))
			usage();
		break;
	default:
		usage();
	} ARGEND;

	if (!have_lang)
		usage();

	return run(argc, argv, process);
}
