/* See LICENSE file for copyright and license details. */
#define LIBSIMPLY_CONFIG_MULTICALL_BINARY
#include "common.h"
#include <libsimple-arg.h>

USAGE("-l language [-o options ...] [cardinal ...]");


static enum libnumtext_language lang;
static uint32_t flags = 0;


static struct option swedish_options[] = {
	{"gender=c[ommon]|u|t", "gender=common",    LIBNUMTEXT_C2O_SWEDISH_COMMON_GENDER,    UINT32_C(0x00000003)},
	{NULL,                  "gender=c",         LIBNUMTEXT_C2O_SWEDISH_COMMON_GENDER,    UINT32_C(0x00000003)},
	{NULL,                  "gender=u",         LIBNUMTEXT_C2O_SWEDISH_COMMON_GENDER,    UINT32_C(0x00000003)},
	{NULL,                  "gender=t",         LIBNUMTEXT_C2O_SWEDISH_COMMON_GENDER,    UINT32_C(0x00000003)},
	{"gender=n[euter]",     "gender=neuter",    LIBNUMTEXT_C2O_SWEDISH_NEUTER_GENDER,    UINT32_C(0x00000003)},
	{NULL,                  "gender=n",         LIBNUMTEXT_C2O_SWEDISH_NEUTER_GENDER,    UINT32_C(0x00000003)},
	{"gender=m[asculine]",  "gender=masculine", LIBNUMTEXT_C2O_SWEDISH_MASCULINE_GENDER, UINT32_C(0x00000003)},
	{NULL,                  "gender=m",         LIBNUMTEXT_C2O_SWEDISH_MASCULINE_GENDER, UINT32_C(0x00000003)},
	{"gender=f[eminine]",   "gender=feminine",  LIBNUMTEXT_C2O_SWEDISH_FEMININE_GENDER,  UINT32_C(0x00000003)},
	{NULL,                  "gender=f",         LIBNUMTEXT_C2O_SWEDISH_FEMININE_GENDER,  UINT32_C(0x00000003)},
	{"case=l[ower]",        "case=lower",       LIBNUMTEXT_C2O_SWEDISH_LOWER_CASE,       UINT32_C(0x00000004)},
	{NULL,                  "case=l",           LIBNUMTEXT_C2O_SWEDISH_LOWER_CASE,       UINT32_C(0x00000004)},
	{"case=u[pper]",        "case=upper",       LIBNUMTEXT_C2O_SWEDISH_UPPER_CASE,       UINT32_C(0x00000004)},
	{NULL,                  "case=u",           LIBNUMTEXT_C2O_SWEDISH_UPPER_CASE,       UINT32_C(0x00000004)},
	{NULL, NULL, 0, 0}
};

static struct option *options[] = {
	[LIBNUMTEXT_SWEDISH] = swedish_options
};


static ssize_t
process(char *outbuf, size_t outbuf_size, const char *num, size_t num_len)
{
	ssize_t ret;
	ret = libnumtext_card2ord(outbuf, outbuf_size, num, num_len, lang, flags);
	if (ret < 0)
		fprintf(stderr, "%s: libnumtext_card2ord %s: %s\n", argv0, num, strerror(errno));
	return ret;
}


int
card2ord_main(int argc, char *argv[])
{
	int have_lang = 0;
	char **optionses;
	size_t n_optionses = 0;

	optionses = calloc((size_t)argc, sizeof(optionses));
	if (!optionses) {
		fprintf(stderr, "%s: calloc %zu %zu: %s\n", argv0, (size_t)argc, sizeof(optionses), strerror(errno));
		return 1;
	}

	ARGBEGIN {
	case 'l':
		if (!get_language(ARG(), &lang, &have_lang))
			usage();
		break;
	case 'o':
		optionses[n_optionses++] = ARG();
		break;
	default:
		usage();
	} ARGEND;

	if (!have_lang)
		usage();

	process_options(optionses, n_optionses, options[lang], &flags, NULL);
	free(optionses);

	return run(argc, argv, process);
}
