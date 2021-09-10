/* See LICENSE file for copyright and license details. */
#define LIBSIMPLY_CONFIG_MULTICALL_BINARY
#include "common.h"
#include <libsimple-arg.h>

USAGE("-l language [-o options ...] [cardinal ...]");


static enum libnumtext_language lang;
static uint32_t flags = 0;


static struct option swedish_options[] = {

	{"[type=]c[ard[inal]]",        "type=cardinal",          LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{NULL,                         "type=card",              LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{NULL,                         "type=c",                 LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{NULL,                         "cardinal",               LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{NULL,                         "card",                   LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{NULL,                         "c",                      LIBNUMTEXT_N2T_SWEDISH_CARDINAL,              UINT32_C(0x00000001)},
	{"[type=]o[rd[inal]]",         "type=ordinal",           LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},
	{NULL,                         "type=ord",               LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},
	{NULL,                         "type=o",                 LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},
	{NULL,                         "ordinal",                LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},
	{NULL,                         "ord",                    LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},
	{NULL,                         "o",                      LIBNUMTEXT_N2T_SWEDISH_ORDINAL,               UINT32_C(0x00000001)},

	{"numerator|denominator=n[o]", "denominator=no",         LIBNUMTEXT_N2T_SWEDISH_NUMERATOR,             UINT32_C(0x00000002)},
	{NULL,                         "denominator=n",          LIBNUMTEXT_N2T_SWEDISH_NUMERATOR,             UINT32_C(0x00000002)},
	{NULL,                         "numerator",              LIBNUMTEXT_N2T_SWEDISH_NUMERATOR,             UINT32_C(0x00000002)},
	{"denominator[=y[es]]",        "denominator=yes",        LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR,           UINT32_C(0x00000002)},
	{NULL,                         "denominator=y",          LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR,           UINT32_C(0x00000002)},
	{NULL,                         "denominator",            LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR,           UINT32_C(0x00000002)},

	{"singular|plural=n[o]",       "plural=no",              LIBNUMTEXT_N2T_SWEDISH_SINGULAR_FORM,         UINT32_C(0x00000004)},
	{NULL,                         "plural=n",               LIBNUMTEXT_N2T_SWEDISH_SINGULAR_FORM,         UINT32_C(0x00000004)},
	{NULL,                         "singular",               LIBNUMTEXT_N2T_SWEDISH_SINGULAR_FORM,         UINT32_C(0x00000004)},
	{"plural[=y[es]]",             "plural=yes",             LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM,           UINT32_C(0x00000004)},
	{NULL,                         "plural=y",               LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM,           UINT32_C(0x00000004)},
	{NULL,                         "plural",                 LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM,           UINT32_C(0x00000004)},

	{"indefinite|definite=n[o]",   "definite=no",            LIBNUMTEXT_N2T_SWEDISH_INDEFINITE_FORM,       UINT32_C(0x00000008)},
	{NULL,                         "definite=n",             LIBNUMTEXT_N2T_SWEDISH_INDEFINITE_FORM,       UINT32_C(0x00000008)},
	{NULL,                         "indefinite",             LIBNUMTEXT_N2T_SWEDISH_INDEFINITE_FORM,       UINT32_C(0x00000008)},
	{"definite[=y[es]]",           "definite=yes",           LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM,         UINT32_C(0x00000008)},
	{NULL,                         "definite=y",             LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM,         UINT32_C(0x00000008)},
	{NULL,                         "definite",               LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM,         UINT32_C(0x00000008)},

	{"gender=c[ommon]|u|t",        "gender=common",          LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER,         UINT32_C(0x00000030)},
	{NULL,                         "gender=c",               LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER,         UINT32_C(0x00000030)},
	{NULL,                         "gender=u",               LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER,         UINT32_C(0x00000030)},
	{NULL,                         "gender=t",               LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER,         UINT32_C(0x00000030)},
	{"gender=n[euter]",            "gender=neuter",          LIBNUMTEXT_N2T_SWEDISH_NEUTER_GENDER,         UINT32_C(0x00000030)},
	{NULL,                         "gender=n",               LIBNUMTEXT_N2T_SWEDISH_NEUTER_GENDER,         UINT32_C(0x00000030)},
	{"gender=m[asculine]",         "gender=masculine",       LIBNUMTEXT_N2T_SWEDISH_MASCULINE_GENDER,      UINT32_C(0x00000030)},
	{NULL,                         "gender=m",               LIBNUMTEXT_N2T_SWEDISH_MASCULINE_GENDER,      UINT32_C(0x00000030)},
	{"gender=f[eminine]",          "gender=feminine",        LIBNUMTEXT_N2T_SWEDISH_FEMININE_GENDER,       UINT32_C(0x00000030)},
	{NULL,                         "gender=f",               LIBNUMTEXT_N2T_SWEDISH_FEMININE_GENDER,       UINT32_C(0x00000030)},

	{"one=e[xplicit]",             "one=explicit",           LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_ONE,          UINT32_C(0x00000040)},
	{NULL,                         "one=e",                  LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_ONE,          UINT32_C(0x00000040)},
	{"one=i[mplicit]",             "one=implicit",           LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE,          UINT32_C(0x00000040)},
	{NULL,                         "one=i",                  LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE,          UINT32_C(0x00000040)},

	{"hyphenated=n[o]",            "hyphenated=no",          LIBNUMTEXT_N2T_SWEDISH_NOT_HYPHENATED,        UINT32_C(0x00000080)},
	{NULL,                         "hyphenated=n",           LIBNUMTEXT_N2T_SWEDISH_NOT_HYPHENATED,        UINT32_C(0x00000080)},
	{"hyphenated[=y[es]]",         "hyphenated=yes",         LIBNUMTEXT_N2T_SWEDISH_HYPHENATED,            UINT32_C(0x00000080)},
	{NULL,                         "hyphenated=y",           LIBNUMTEXT_N2T_SWEDISH_HYPHENATED,            UINT32_C(0x00000080)},
	{NULL,                         "hyphenated",             LIBNUMTEXT_N2T_SWEDISH_HYPHENATED,            UINT32_C(0x00000080)},

	{"case=l[ower]",               "case=lower",             LIBNUMTEXT_N2T_SWEDISH_LOWER_CASE,            UINT32_C(0x00000300)},
	{NULL,                         "case=l",                 LIBNUMTEXT_N2T_SWEDISH_LOWER_CASE,            UINT32_C(0x00000300)},
	{"case=u[pper]",               "case=upper",             LIBNUMTEXT_N2T_SWEDISH_UPPER_CASE,            UINT32_C(0x00000300)},
	{NULL,                         "case=u",                 LIBNUMTEXT_N2T_SWEDISH_UPPER_CASE,            UINT32_C(0x00000300)},
	{"case=m[oney]",               "case=money",             LIBNUMTEXT_N2T_SWEDISH_MONEY_CASE,            UINT32_C(0x00000300)},
	{NULL,                         "case=m",                 LIBNUMTEXT_N2T_SWEDISH_MONEY_CASE,            UINT32_C(0x00000300)},
	{"case=s[entence]",            "case=sentence",          LIBNUMTEXT_N2T_SWEDISH_SENTENCE_CASE,         UINT32_C(0x00000300)},
	{NULL,                         "case=s",                 LIBNUMTEXT_N2T_SWEDISH_SENTENCE_CASE,         UINT32_C(0x00000300)},

	{"hyphenation=n[o[ne]]",       "hyphenation=none",       LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION,        UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=no",         LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION,        UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=n",          LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION,        UINT32_C(0x00000C00)},
	{"hyphenation=c[omponent[s]]", "hyphenation=components", LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION, UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=component",  LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION, UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=c",          LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION, UINT32_C(0x00000C00)},
	{"hyphenation=sy[llable[s]]",  "hyphenation=syllables",  LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION,  UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=syllable",   LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION,  UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=sy",         LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION,  UINT32_C(0x00000C00)},
	{"hyphenation=se[condary]|2",  "hyphenation=secondary",  LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION, UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=se",         LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION, UINT32_C(0x00000C00)},
	{NULL,                         "hyphenation=2",          LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION, UINT32_C(0x00000C00)},

	{"triplets=r[educed]",         "triplets=reduced",       LIBNUMTEXT_N2T_SWEDISH_REDUCED_TRIPLETS,      UINT32_C(0x00003000)},
	{NULL,                         "triplets=r",             LIBNUMTEXT_N2T_SWEDISH_REDUCED_TRIPLETS,      UINT32_C(0x00003000)},
	{"triplets=l[atex]",           "triplets=latex",         LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS,        UINT32_C(0x00003000)},
	{NULL,                         "triplets=l",             LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS,        UINT32_C(0x00003000)},
	{"triplets=e[xplicit]",        "triplets=explicit",      LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_TRIPLETS,     UINT32_C(0x00003000)},
	{NULL,                         "triplets=e",             LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_TRIPLETS,     UINT32_C(0x00003000)},

	{NULL, NULL, 0, 0}
};

static struct option *options[] = {
	[LIBNUMTEXT_SWEDISH] = swedish_options
};


static ssize_t
process(char *outbuf, size_t outbuf_size, const char *num, size_t num_len)
{
	ssize_t ret;
	ret = libnumtext_num2text(outbuf, outbuf_size, num, num_len, lang, flags);
	if (ret < 0)
		fprintf(stderr, "%s: libnumtext_num2text %s: %s\n", argv0, num, strerror(errno));
	return ret;
}


int
num2text_main(int argc, char *argv[])
{
	int have_lang = 0;
	char **optionses;
	size_t n_optionses = 0;
	uint32_t flag_mask;

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

	process_options(optionses, n_optionses, options[lang], &flags, &flag_mask);
	free(optionses);

	if (lang == LIBNUMTEXT_SWEDISH) {
		if ((flag_mask & UINT32_C(0x0000000C)) && !(flags & UINT32_C(0x00000003))) {
			fprintf(stderr, "%s: singular/plural and indefinite/definite"
			                " cannot be used with numerator cardinals\n", argv0);
			return 1;
		}
	}

	return run(argc, argv, process);
}
