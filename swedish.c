/* See LICENSE file for copyright and license details. */
#include "common.h"


#define CARDINAL(F)              (!ORDINAL(F))
#define ORDINAL(F)               ((F) & LIBNUMTEXT_N2T_SWEDISH_ORDINAL)

#define NUMERATOR(F)             (!DENOMINATOR(F))
#define DENOMINATOR(F)           ((F) & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR)

#define SINGULAR_FORM(F)         (!PLURAL_FORM(F))
#define PLURAL_FORM(F)           ((F) & LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM)

#define INDEFINITE_FORM(F)       (!DEFINITE_FORM(F))
#define DEFINITE_FORM(F)         ((F) & LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM)

#define GENDER(F)                ((F) & UINT32_C(0x00000030))
#define COMMON_GENDER(F)         (GENDER(F) == LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER)
#define NEUTER_GENDER(F)         (GENDER(F) == LIBNUMTEXT_N2T_SWEDISH_NEUTER_GENDER)
#define MASCULINE_GENDER(F)      (GENDER(F) == LIBNUMTEXT_N2T_SWEDISH_MASCULINE_GENDER)
#define FEMININE_GENDER(F)       (GENDER(F) == LIBNUMTEXT_N2T_SWEDISH_FEMININE_GENDER)

#define EXPLICIT_ONE(F)          (!IMPLICIT_ONE(F))
#define IMPLICIT_ONE(F)          ((F) & LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE)

#define NOT_HYPHENATED(F)        (!HYPHENATED(F))
#define HYPHENATED(F)            ((F) & LIBNUMTEXT_N2T_SWEDISH_HYPHENATED)

#define CASE(F)                  ((F) & UINT32_C(0x00000300))
#define LOWER_CASE(F)            (CASE(F) == LIBNUMTEXT_N2T_SWEDISH_LOWER_CASE)
#define PASCAL_CASE(F)           (CASE(F) == LIBNUMTEXT_N2T_SWEDISH_PASCAL_CASE)
#define UPPER_CASE(F)            (CASE(F) == LIBNUMTEXT_N2T_SWEDISH_UPPER_CASE)
#define SENTENCE_CASE(F)         (CASE(F) == LIBNUMTEXT_N2T_SWEDISH_SENTENCE_CASE)

#define HYPHENATION(F)           ((F) & UINT32_C(0x00000C00))
#define NO_HYPHENATION(F)        (HYPHENATION(F) == LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION)
#define COMPONENT_HYPHENATION(F) (HYPHENATION(F) == LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION)
#define SYLLABLE_HYPHENATION(F)  (HYPHENATION(F) == LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION)
#define SECONDARY_HYPHENATION(F) (HYPHENATION(F) == LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION)

#define TRIPLETS(F)              ((F) & UINT32_C(0x00003000))
#define REDUCED_TRIPLETS(F)      (TRIPLETS(F) == LIBNUMTEXT_N2T_SWEDISH_REDUCED_TRIPLETS)
#define EXPLICIT_TRIPLETS(F)     (TRIPLETS(F) == LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_TRIPLETS)
#define LATEX_TRIPLETS(F)        (TRIPLETS(F) == LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS)
#define X_INVALID_TRIPLETS(F)    (TRIPLETS(F) > LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS)

#define INVALID_BITS(F)          ((F) & (uint32_t)~UINT32_C(0x00003FFF))


static struct digit {
	const char *cardinal_common;
	const char *cardinal_other;
	const char *ordinal_other;
	const char *ordinal_masculine;
	const char *ordinal_suffix;
} digits[] = {
	{"Noll",     NULL, "Noll¦te",  NULL,      NULL},
	{"Ett",      "En", "Förs¦ta",  "Förs¦te", NULL},
	{"Två",      NULL, "An¦dra",   "An¦dre",  NULL},
	{"Tre",      NULL, "Tre¦dje",  NULL,      NULL},
	{"Fy¦ra",    NULL, "Fjär¦de",  NULL,      NULL},
	{"Fem",      NULL, "Fem¦te",   NULL,      NULL},
	{"Sex",      NULL, "Sjät¦te",  NULL,      NULL},
	{"Sju",      NULL, "Sjun¦de",  NULL,      NULL},
	{"Åt¦ta",    NULL, "Åt¦ton",   NULL,      "¦de"},
	{"Nio",      NULL, "Ni¦on",    NULL,      "¦de"},
	{"Tio",      NULL, "Ti¦on",    NULL,      "¦de"},
	{"El¦va",    NULL, "Elf¦te",   NULL,      NULL},
	{"Tolv",     NULL, "Tolf¦te",  NULL,      NULL},
	{"Tret|ton", NULL, "Tret|ton", NULL,      "¦de"},
	{"Fjor|ton", NULL, "Fjor|ton", NULL,      "¦de"},
	{"Fem|ton",  NULL, "Fem|ton",  NULL,      "¦de"},
	{"Sex|ton",  NULL, "Sex|ton",  NULL,      "¦de"},
	{"Sjut|ton", NULL, "Sjut|ton", NULL,      "¦de"},
	{"Ar|ton",   NULL, "Ar|ton",   NULL,      "¦de"},
	{"Nit|ton",  NULL, "Nit|ton",  NULL,      "¦de"}
};

static struct ten {
	const char *cardinal;
	const char *ordinal;
} tens[] = {
	{NULL,       NULL},
	{NULL,       NULL},
	{"Tju¦go",   "Tju¦gon"},
	{"Tret¦tio", "Tret¦ti¦on"},
	{"Fyr¦tio",  "Fyr¦ti¦on"},
	{"Fem¦tio",  "Fem¦ti¦on"},
	{"Sex¦tio",  "Sex¦ti¦on"},
	{"Sjut¦tio", "Sjut¦ti¦on"},
	{"Åt¦tio",   "Åt¦ti¦on"},
	{"Nit¦tio",  "Nit¦ti¦on"}
};

static const char *wholes_and_halves[][5] = {
	{"Hel",  "He¦la",  "Hel¦an",  "Hel¦or¦na",  "Hel¦te"},
	{"Halv", "Hal¦va", "Halv¦an", "Halv¦or¦na", "Half¦te"}
};

static const char *great_suffixes[] = {
	"il¦jon",
	"il¦jard"
};

static const char *greats[][7] = {
	{NULL,       NULL,          NULL, NULL,  NULL,                NULL,  NULL},
	{"||m",      "||un",        "",   "n",   "||de¦ci",           "nx*", "||cen¦ti"},
	{"||b",      "||duo",       "",   "ms",  "||vi|gin¦ti",       "n",   "||du|cen¦ti"},
	{"||tr",     "||tre",       "s*", "ns",  "||tri|gin¦ta",      "ns",  "||tre|cen¦ti"},
	{"||kvad¦r", "||kvat¦tuor", "",   "ns",  "||kvad¦ra|gin¦ta",  "ns",  "||kvad¦rin|gen¦ti"},
	{"||kvin¦t", "||kvin",      "",   "ns",  "||kvin¦kva|gin¦ta", "ns",  "||kvin|gen¦ti"},
	{"||sex¦t",  "||se",        "sx", "n",   "||sex¦a|gin¦ta",    "n",   "||ses|cen¦ti"},
	{"||sep¦t",  "||sep¦te",    "mn", "n",   "||sep¦tua|gin¦ta",  "n",   "||sep¦tin|gen¦ti"},
	{"||ok¦t",   "||ok¦to",     "",   "mx*", "||ok¦to|gin¦ta",    "mx*", "||ok¦tin|gen|ti"},
	{"||no¦n",   "||no¦ve",     "mn", "",    "||no¦na|gin¦ta",    "",    "||non|gen¦ti"}
};


struct state {
	char *outbuf;
	size_t outbuf_size;
	size_t len;
	uint32_t flags;
	char double_char;
};


static void
append(struct state *state, const char *appendage)
{
	uint32_t flags = state->flags;
	char hyphen[sizeof("¦x")], *p, a = 0, b = 0;
	int shift;

	for (; *appendage; a = b, b = *appendage++) {
		if (appendage[0] == '|' && appendage[1] == '|') {
			if (NO_HYPHENATION(flags))
				appendage = &appendage[2];
			else
				appendage = &appendage[1];
		} else if (appendage[0] == '|') {
			if (NO_HYPHENATION(flags) || COMPONENT_HYPHENATION(flags)) {
				appendage = &appendage[1];
			}
		} else if (!strncmp(&appendage[appendage[0] == '<'], "¦", sizeof("¦") - 1)) {
			shift = appendage[0] == '<';
			appendage = &appendage[shift];
			appendage = &appendage[sizeof("¦") - 1];
			if (SECONDARY_HYPHENATION(flags)) {
				p = stpcpy(hyphen, "¦");
			} else if (SYLLABLE_HYPHENATION(flags)) {
				p = stpcpy(hyphen, "|");
			} else {
				*hyphen = 0;
				p = NULL; /* silence clang */
			}
			if (*hyphen) {
				if (shift && state->len <= state->outbuf_size) {
					*p++ = state->outbuf[--state->len];
					*p = '\0';
				}
				for (p = hyphen; *p; p++) {
					if (state->len < state->outbuf_size)
						state->outbuf[state->len] = *p;
					state->len += 1;
				}
			}
		}

		if (state->len >= 2 && state->double_char == appendage[appendage[0] == '|']) {
			if (appendage[0] == '|' && !EXPLICIT_TRIPLETS(flags))
				appendage = &appendage[1];
			if (REDUCED_TRIPLETS(flags)) {
				state->len -= 1;
			} else if (LATEX_TRIPLETS(flags)) {
				if (state->len - 2 < state->outbuf_size)
					state->outbuf[state->len - 2] = '"';
			}
		}

		if (state->len && (HYPHENATED(flags) | HYPHENATION(flags))) {
			if (isupper(appendage[0]) || (appendage[0] == "Å"[0] && appendage[1] == "Å"[1])) {
				if (state->len < state->outbuf_size)
					state->outbuf[state->len] = HYPHENATED(flags) ? '-' : '|';
				state->len += 1;
			}
		}

		if (state->len < state->outbuf_size)
			state->outbuf[state->len] = *appendage;
		state->len += 1;
	}

	state->double_char = a == b ? a : 0;
}


static void
suffix(struct state *state)
{
	uint32_t flags = state->flags;
	const char *appendage;

	if (ORDINAL(flags)) {
		if (DENOMINATOR(flags))
			appendage = "||delte";
		else
			return;
	} else if (DENOMINATOR(flags)) {
		if (PLURAL_FORM(flags))
			appendage = DEFINITE_FORM(flags) ? "||del¦ar¦na" : "||del¦ar";
		else
			appendage = DEFINITE_FORM(flags) ? "||del¦en" : "||del";
	} else {
		return;
	}

	append(state, appendage);
}


ssize_t
libnumtext_num2text_swedish__(char *outbuf, size_t outbuf_size, const char *num, size_t num_len, uint32_t flags)
{
	int first = 1;
	int hundred_thousands, thousands, orig_thousands, hundreds, ones;
	int32_t small_num;
	const char *great_1, *great_1_suffix, *great_last;
	const char *great_10, *great_10_prefix, *gsuffix;
	const char *great_100, *great_100_prefix, *gprefix;
	char affix[2] = {[1] = 0};
	size_t great_order, small_order, great_order_suffix;
	size_t i, offset;
	const char *append_for_ordinal = NULL;
	size_t trailing_zeroes;
	struct state state;

	if (INVALID_BITS(flags) || X_INVALID_TRIPLETS(flags))
		goto einval;
	if (PLURAL_FORM(flags) || DEFINITE_FORM(flags))
		if (!ORDINAL(flags) || !DENOMINATOR(flags))
			goto einval;

	state.outbuf = outbuf;
	state.outbuf_size = outbuf_size;
	state.len = 0;
	state.flags = flags;
	state.double_char = 0;

	if (!isdigit(num[0])) {
		if (ORDINAL(flags) || DENOMINATOR(flags))
			append(&state, num[0] == '+' ? "Plus-" : "Min¦us-");
		else
			append(&state, num[0] == '+' ? "Plus " : "Min¦us ");
		offset = state.len;
		if (offset > state.outbuf_size)
			offset = state.outbuf_size;
		state.outbuf += offset;
		state.outbuf_size -= offset;
		offset = state.len;
		state.len = 0;
		do {
			num++;
			num_len--;
		} while ((*num & 0xC0) == 0x80);
	} else {
		offset = 0;
	}

	while (num_len > 1 && num[0] == '0') {
		num++;
		num_len--;
	}

	if (num_len == 1) {
		if (num[0] == '0') {
			if (ORDINAL(flags) || DENOMINATOR(flags))
				append(&state, digits[0].ordinal_other);
			else
				append(&state, digits[0].cardinal_common);
			suffix(&state);
			goto out;
		} else if (num[0] <= '2' && DENOMINATOR(flags)) {
			if (ORDINAL(flags))
				i = 4;
			else
				i = (size_t)((flags / LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM) & 3);
			append(&state, wholes_and_halves[num[0] - 1][i]);
			goto out;
		}
	}

	trailing_zeroes = 0;
	for (i = 0; i < num_len; i += 1) {
		if (num[i] == '0')
			trailing_zeroes += 1;
		else
			trailing_zeroes = 0;
	}

	while (num_len) {
		num_len -= 1;
		great_order = num_len / 6;
		small_order = num_len % 6;

		great_order_suffix = 0;
		hundred_thousands = thousands = hundreds = ones = 0;
		small_num = 0;

		if (great_order && small_order >= 3) {
			small_order -= 3;
			great_order_suffix = 1;
		}

		orig_thousands = 0;
		switch (small_order) {
		case 5:
			hundred_thousands = *num++ - '0';
			small_num = (int32_t)hundred_thousands;
			num_len--;
			if (hundred_thousands) {
				if (first && hundred_thousands == 1 && IMPLICIT_ONE(flags)) {
					append(&state, "Hun¦dra");
				} else {
					append(&state, digits[hundred_thousands].cardinal_common);
					append(&state, "||hun¦dra");
				}
				append_for_ordinal = "¦de";
				first = 0;
			}
			FALL_THROUGH
			/* fall through */

		case 4:
			thousands = *num++ - '0';
			orig_thousands = thousands;
			num_len--;
			if (tens[thousands].cardinal) {
				append(&state, tens[thousands].cardinal);
				thousands = 0;
				first = 0;
			} else {
				thousands *= 10;
			}
			FALL_THROUGH
			/* fall through */

		case 3:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			thousands += *num++ - '0';
			num_len--;
			if (thousands) {
				if (first && thousands == 1 && IMPLICIT_ONE(flags)) {
					append(&state, "Tu¦sen");
				} else {
					append(&state, digits[thousands].cardinal_common);
					append(&state, "||tu¦sen");
				}
				append_for_ordinal = "¦de";
				first = 0;
			} else if (hundred_thousands || orig_thousands) {
				append(&state, "||tu¦sen");
				append_for_ordinal = "¦de";
				first = 0;
			}
			FALL_THROUGH
			/* fall through */

		case 2:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			hundreds = *num++ - '0';
			num_len--;
			if (hundreds) {
				if (first && hundreds == 1 && IMPLICIT_ONE(flags)) {
					append(&state, "Hun¦dra");
				} else {
					append(&state, digits[hundreds].cardinal_common);
					append(&state, "||hun¦dra");
				}
				append_for_ordinal = "¦de";
				first = 0;
			}
			FALL_THROUGH
			/* fall through */

		case 1:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			ones = *num++ - '0';
			num_len--;
			if (tens[ones].cardinal) {
				if (!great_order && (DENOMINATOR(flags) || ORDINAL(flags)) && *num == '0') {
					append(&state, tens[ones].ordinal);
					append_for_ordinal = "¦de";
				} else {
					append(&state, tens[ones].cardinal);
					append_for_ordinal = NULL;
				}
				ones = 0;
				first = 0;
			} else {
				ones *= 10;
			}
			FALL_THROUGH
			/* fall through */

		case 0:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			ones += *num++ - '0';
			if (ones) {
				append_for_ordinal = NULL;
				if (!great_order && (DENOMINATOR(flags) || ORDINAL(flags))) {
					if (MASCULINE_GENDER(flags) && digits[ones].ordinal_masculine)
						append(&state, digits[ones].ordinal_masculine);
					else
						append(&state, digits[ones].ordinal_other);
					append_for_ordinal = digits[ones].ordinal_suffix;
				} else if (!digits[ones].cardinal_other) {
					append(&state, digits[ones].cardinal_common);
				} else if (great_order) {
					append(&state, digits[ones].cardinal_other);
				} else if (COMMON_GENDER(flags)) {
					append(&state, digits[ones].cardinal_common);
				} else {
					append(&state, digits[ones].cardinal_other);
				}
				first = 0;
			}
			break;
		}

		if (great_order && small_num) {
			if (great_order < 10) {
				append(&state, greats[great_order][0]);
			} else if (great_order > 999) {
				errno = EDOM;
				return -1;
			} else {
				great_1          = greats[(great_order / 1) % 10][1];
				great_1_suffix   = greats[(great_order / 1) % 10][2];
				great_10_prefix  = greats[(great_order / 10) % 10][3];
				great_10         = greats[(great_order / 10) % 10][4];
				great_100_prefix = greats[(great_order / 100) % 10][5];
				great_100        = greats[(great_order / 100) % 10][6];
				great_last = NULL;
				if (great_1) {
					append(&state, great_1);
					great_last = great_1;
				}
				if (great_10) {
					if (great_1_suffix && great_10_prefix) {
						gsuffix = NULL; /* silence clang */
						for (gprefix = great_10_prefix; *gprefix; gprefix++) {
							for (gsuffix = great_1_suffix; *gsuffix; gsuffix++)
								if (*gprefix == *gsuffix)
									break;
							if (*gsuffix)
								break;
						}
						if (*gprefix && *gprefix == *gsuffix) {
							affix[0] = *gprefix;
							if (affix[0] == '*')
								affix[0] = 's';
							append(&state, affix);
						}
					}
					append(&state, great_10);
					great_last = great_10;
					great_1_suffix = NULL;
				}
				if (great_100) {
					if (great_1_suffix && great_100_prefix) {
						gsuffix = NULL; /* silence clang */
						for (gprefix = great_100_prefix; *gprefix; gprefix++) {
							for (gsuffix = great_1_suffix; *gsuffix; gsuffix++)
								if (*gprefix == *gsuffix)
									break;
							if (*gsuffix)
								break;
						}
						if (*gprefix && *gprefix == *gsuffix) {
							affix[0] = *gprefix;
							if (affix[0] == '*')
								affix[0] = 's';
							append(&state, affix);
						}
					}
					append(&state, great_100);
					great_last = great_100;
				}
				while (great_last[1])
					great_last = &great_last[1];
				if (*great_last == 'a' || *great_last == 'e' || *great_last == 'i' || *great_last == 'o')
					state.len -= 1;
			}
			append(&state, great_suffixes[great_order_suffix]);
			append_for_ordinal = great_order_suffix == 0 ? "¦te" : "<¦e";
			if (small_num != 1)
				if (num_len > trailing_zeroes || !(ORDINAL(flags) || DENOMINATOR(flags)))
					append(&state, "¦er");
		}
	}

	if (ORDINAL(flags) && !DENOMINATOR(flags) && append_for_ordinal)
		append(&state, append_for_ordinal);
	suffix(&state);

out:
	state.len += offset;

	if (state.len < outbuf_size)
		outbuf[state.len] = '\0';
	else if (outbuf_size)
		outbuf[outbuf_size - 1] = '\0';
	state.len += 1;

	if (!outbuf_size)
		return (ssize_t)state.len;

	/*
	 * Å   = \xc3\x85
	 * å   = \xc3\xa5
	 * Ä   = \xc3\x84
	 * ä   = \xc3\xa4
	 * Ö   = \xc3\x96
	 * ö   = \xc3\xb6
	 * A-Z = 0x41 - 0x5a
	 * a-z = 0x61 - 0x7a
	 * |   = 0x7c
	 * ¦   = \xc2\xa6
	 * -   = 0x2d
	 */
	i = 0;
	if (SENTENCE_CASE(flags)) {
		i = 1;
		while ((outbuf[i] & 0xC0) == 0x80)
			i += 1;
		goto lower_case;

	} else if (UPPER_CASE(flags)) {
		for (; outbuf[i]; i++)
			if (islower(outbuf[i]) || outbuf[i] == '\xa5' || outbuf[i] == '\xa4' || outbuf[i] == '\xb6')
				outbuf[i] ^= 0x20;

	} else if (LOWER_CASE(flags)) {
	lower_case:
		for (; outbuf[i]; i++)
			if (isupper(outbuf[i]) || outbuf[i] == '\x85')
				outbuf[i] ^= 0x20;
	}

	return (ssize_t)state.len;

einval:
	errno = EINVAL;
	return -1;
}
