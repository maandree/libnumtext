/* See LICENSE file for copyright and license details. */
#include "common.h"


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
	{NULL},
	{NULL},
	{"Tju¦go",   "Tju¦gon"},
	{"Tret|tio", "Tret|ti¦on"},
	{"Fyr|tio",  "Fyr|ti¦on"},
	{"Fem|tio",  "Fem|ti¦on"},
	{"Sex|tio",  "Sex|ti¦on"},
	{"Sju|tio",  "Sju|ti¦on"},
	{"Åt|tio",   "Åt|ti¦on"},
	{"Nit|tio",  "Nit|ti¦on"}
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
	{"||kvad¦r", "||kvat¦tour", "",   "ns",  "||kvad¦ra|gin¦ta",  "ns",  "||kvad¦rin|gen¦ti"},
	{"||kvin¦t", "||kvin",      "",   "ns",  "||kvin¦kva|gin¦ta", "ns",  "||kvin|gen¦ti"},
	{"||sex¦t",  "||se",        "sx", "n",   "||sex¦a|gin¦ta",    "n",   "||ses|cen¦ti"},
	{"||sep¦t",  "||sep¦te",    "mn", "n",   "||sep¦tua|gin¦ta",  "n",   "||sep¦tin|gen¦ti"},
	{"||ok¦t",   "||ok¦to",     "",   "mx*", "||ok¦to|gin¦ta",    "mx*", "||ok¦tin|gen|ti"},
	{"||no¦n",   "||no¦ve",     "mn", "",    "||no¦na|gin¦ta",    "",    "||non|gen¦ti"}
};


static void
append(char outbuf[], size_t outbuf_size, size_t *lenp, const char *appendage, uint32_t flags)
{
	char hyphen[sizeof("¦x")], *p;
	uint32_t hyphenation = flags & UINT32_C(0x00000C00);
	int shift;

	if (appendage[0] == '|' && appendage[1] == '|') {
		if (hyphenation == LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION)
			appendage = &appendage[2];
		else
			appendage = &appendage[1];
	} else if (appendage[0] == '|') {
		if (hyphenation == LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION ||
		    hyphenation == LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION) {
			appendage = &appendage[1];
		}
	} else if (!strncmp(&appendage[appendage[0] == '<'], "¦", sizeof("¦") - 1)) {
		shift = appendage[0] == '<';
		appendage = &appendage[shift];
		appendage = &appendage[sizeof("¦") - 1];
		if (hyphenation == LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION)
			p = stpcpy(hyphen, "¦");
		else if (hyphenation == LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION)
			p = stpcpy(hyphen, "|");
		else
			*hyphen = 0;
		if (*hyphen) {
			if (shift && *lenp <= outbuf_size) {
				*p++ = outbuf[--*lenp];
				*p = '\0';
			}
			for (p = hyphen; *p; p++) {
				if (*lenp < outbuf_size)
					outbuf[*lenp] = *p;
				*lenp += 1;
			}
		}
	}

	if (*lenp >= 2 && outbuf[*lenp - 2] == outbuf[*lenp - 1] && outbuf[*lenp - 1] == appendage[0]) {
		if ((flags & UINT32_C(0x00002000)) == LIBNUMTEXT_N2T_SWEDISH_REDUCED_TRIPLETS)
			*lenp -= 1;
		else if ((flags & UINT32_C(0x00002000)) == LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS)
			outbuf[*lenp - 2] = '"';
	}

	if (*lenp && (flags & (LIBNUMTEXT_N2T_SWEDISH_HYPHENATED | hyphenation))) {
		if (isupper(appendage[0]) || (appendage[0] == "Å"[0] && appendage[1] == "Å"[1])) {
			if (*lenp < outbuf_size)
				outbuf[*lenp] = '-';
			*lenp += 1;
		}
	}

	for (; *appendage; appendage++) {
		if (*lenp < outbuf_size)
			outbuf[*lenp] = *appendage;
		*lenp += 1;
	}
}


static void
suffix(char outbuf[], size_t outbuf_size, size_t *lenp, uint32_t flags)
{
	const char *appendage;

	if (flags & LIBNUMTEXT_N2T_SWEDISH_ORDINAL) {
		if (flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR) {
			appendage = "||delte";
		} else {
			return;
		}
	} else if (flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR) {
		if (flags & LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM) {
			if (flags & LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM) {
				appendage = "||del¦ar¦na";
			} else {
				appendage = "||del¦ar";
			}
		} else {
			if (flags & LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM) {
				appendage = "||del¦en";
			} else {
				appendage = "||del";
			}
		}
	} else {
		return;
	}

	append(outbuf, outbuf_size, lenp, appendage, flags);
}


ssize_t
libnumtext_num2text_swedish__(char outbuf_[], size_t outbuf_size, const char *num, size_t num_len, uint32_t flags)
{
	char *outbuf = outbuf_;
	size_t len = 0;
	int first = 1, last;
	int hundred_thousands, thousands, orig_thousands, hundreds, ones;
	int32_t small_num;
	const char *great_1, *great_1_suffix, *great_last;
	const char *great_10, *great_10_prefix, *gsuffix;
	const char *great_100, *great_100_prefix, *gprefix;
	char affix[2] = {[1] = 0};
	size_t great_order, small_order, great_order_suffix;
	size_t i, offset = 0;
	const char *append_for_ordinal = NULL;

	if ((flags & (uint32_t)~UINT32_C(0x00003FFF)) ||
	    ((flags & UINT32_C(0x00003000)) == UINT32_C(0x00003000)))
		goto einval;
	if (flags & (LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM | LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM))
		if ((flags & (LIBNUMTEXT_N2T_SWEDISH_ORDINAL | LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR)) == 0)
			goto einval;

	if (!isdigit(num[0])) {
		append(outbuf, outbuf_size, &len, num[0] == '+' ? "Plus " : "Min¦us ", flags);
		offset = len;
		if (offset > outbuf_size)
			offset = outbuf_size;
		outbuf += offset;
		outbuf_size -= offset;
		num++;
		num_len--;
	}

	while (num_len > 1 && num[0] == '0') {
		num++;
		num_len--;
	}

	if (num_len == 1) {
		if (num[0] == '0') {
			append(outbuf, outbuf_size, &len, digits[0].cardinal_common, flags);
			suffix(outbuf, outbuf_size, &len, flags);
			goto out;
		} else if (num[0] <= '2' && (flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR)) {
			if (flags & LIBNUMTEXT_N2T_SWEDISH_ORDINAL)
				i = 4;
			else
				i = (size_t)((flags / LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM) & 3);
			append(outbuf, outbuf_size, &len, wholes_and_halves[num[0] - 1][i], flags);
			goto out;
		}
	}

	while (num_len) {
		num_len -= 1;
		great_order = num_len / 6;
		small_order = num_len % 6;
		last = !num_len;

		great_order_suffix = 0;
		hundred_thousands = thousands = hundreds = ones = 0;
		small_num = 0;

		if (great_order && small_order >= 3) {
			small_order -= 3;
			great_order_suffix = 1;
		}

		switch (small_order) {
		case 5:
			hundred_thousands = *num++ - '0';
			small_num = (int32_t)hundred_thousands;
			num_len--;
			if (hundred_thousands) {
				if (first && hundred_thousands == 1 && (flags & LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE)) {
					append(outbuf, outbuf_size, &len, "Hun¦dra", flags);
				} else {
					append(outbuf, outbuf_size, &len, digits[hundred_thousands].cardinal_common, flags);
					append(outbuf, outbuf_size, &len, "||hun¦dra", flags);
				}
				append_for_ordinal = "¦de";
				first = 0;
			}
			/* fall through */

		case 4:
			thousands = *num++ - '0';
			orig_thousands = thousands;
			num_len--;
			if (tens[thousands].cardinal) {
				append(outbuf, outbuf_size, &len, tens[thousands].cardinal, flags);
				thousands = 0;
				first = 0;
			} else {
				thousands *= 10;
			}
			/* fall through */

		case 3:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			thousands += *num++ - '0';
			num_len--;
			if (thousands) {
				if (first && thousands == 1 && (flags & LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE)) {
					append(outbuf, outbuf_size, &len, "Tus¦en", flags);
				} else {
					append(outbuf, outbuf_size, &len, digits[thousands].cardinal_common, flags);
					append(outbuf, outbuf_size, &len, "||tus¦en", flags);
				}
				append_for_ordinal = "¦de";
				first = 0;
			} else if (hundred_thousands || orig_thousands) {
				append(outbuf, outbuf_size, &len, "||tus¦en", flags);
				append_for_ordinal = "¦de";
				first = 0;
			}
			/* fall through */

		case 2:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			hundreds = *num++ - '0';
			num_len--;
			if (hundreds) {
				if (first && hundred_thousands == 1 && (flags & LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE)) {
					append(outbuf, outbuf_size, &len, "Hun¦dra", flags);
				} else {
					append(outbuf, outbuf_size, &len, digits[hundreds].cardinal_common, flags);
					append(outbuf, outbuf_size, &len, "||hun¦dra", flags);
				}
				append_for_ordinal = "¦de";
				first = 0;
			}
			/* fall through */

		case 1:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			ones = *num++ - '0';
			num_len--;
			if (tens[ones].cardinal) {
				if (last && (flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR) && *num == '0') {
					append(outbuf, outbuf_size, &len, tens[ones].ordinal, flags);
				} else if (last && !great_order && *num == '0') {
					append(outbuf, outbuf_size, &len, tens[ones].ordinal, flags);
					append(outbuf, outbuf_size, &len, "¦de", flags);
				} else {
					append(outbuf, outbuf_size, &len, tens[ones].cardinal, flags);
				}
				append_for_ordinal = NULL;
				ones = 0;
				first = 0;
			} else {
				ones *= 10;
			}
			/* fall through */

		case 0:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			ones += *num++ - '0';
			if (ones) {
				append_for_ordinal = NULL;
				if (last && (flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR)) {
					if ((flags & UINT32_C(0x00000030)) == LIBNUMTEXT_N2T_SWEDISH_MASCULINE_GENDER)
						append(outbuf, outbuf_size, &len, digits[ones].ordinal_masculine, flags);
					else
						append(outbuf, outbuf_size, &len, digits[ones].ordinal_other, flags);
					append_for_ordinal = digits[ones].ordinal_suffix;
				} else if (!digits[ones].cardinal_other ||
				           (last && (flags & UINT32_C(0x00000030)) == LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER)) {
					append(outbuf, outbuf_size, &len, digits[ones].cardinal_common, flags);
				} else {
					append(outbuf, outbuf_size, &len, digits[ones].cardinal_other, flags);
				}
				first = 0;
			}
			break;
		}

		if (great_order) {
			if (great_order < 10) {
				append(outbuf, outbuf_size, &len, greats[great_order - 1][0], flags);
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
					append(outbuf, outbuf_size, &len, great_1, flags);
					great_last = great_1;
				}
				if (great_10) {
					if (great_1_suffix && great_10_prefix) {
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
							append(outbuf, outbuf_size, &len, affix, flags);
						}
					}
					append(outbuf, outbuf_size, &len, great_10, flags);
					great_last = great_10;
					great_1_suffix = NULL;
				}
				if (great_100) {
					if (great_1_suffix && great_100_prefix) {
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
							append(outbuf, outbuf_size, &len, affix, flags);
						}
					}
					append(outbuf, outbuf_size, &len, great_100, flags);
					great_last = great_100;
				}
				while (great_last[1])
					great_last = &great_last[1];
				if (*great_last == 'a' || *great_last == 'e' || *great_last == 'i' || *great_last == 'o')
					len -= 1;
			}
			append(outbuf, outbuf_size, &len, great_suffixes[great_order_suffix], flags);
			append_for_ordinal = great_order_suffix == 0 ? "¦te" : "<¦e";
			if (small_num != 1)
				if (!last || !(flags & (LIBNUMTEXT_N2T_SWEDISH_ORDINAL | LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR)))
					append(outbuf, outbuf_size, &len, "¦er", flags);
		}
	}

	if (flags & LIBNUMTEXT_N2T_SWEDISH_ORDINAL)
		if (!(flags & LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR))
			if (!append_for_ordinal)
				append(outbuf, outbuf_size, &len, append_for_ordinal, flags);
	suffix(outbuf, outbuf_size, &len, flags);

out:
	outbuf -= offset;
	outbuf_size += offset;

	if ((size_t)len < outbuf_size)
		outbuf[len] = '0';
	else if (outbuf_size)
		outbuf[outbuf_size - 1] = '0';
	len += 1;

	if (!outbuf_size)
		return (ssize_t)len;

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
	if ((flags & UINT32_C(0x00000300)) == LIBNUMTEXT_N2T_SWEDISH_SENTENCE_CASE) {
		i = 1;
		while ((outbuf[i] & 0xC0) == 0x80)
			i += 1;
		goto lower_case;

	} else if ((flags & UINT32_C(0x00000300)) == LIBNUMTEXT_N2T_SWEDISH_UPPER_CASE) {
		for (; outbuf[i]; i++)
			if (isupper(outbuf[i]) || outbuf[i] == '\xa5' || outbuf[i] == '\xa4' || outbuf[i] == '\xb6')
				outbuf[i] ^= 0x20;

	} else if ((flags & UINT32_C(0x00000300)) == LIBNUMTEXT_N2T_SWEDISH_LOWER_CASE) {
	lower_case:
		for (; outbuf[i]; i++)
			if (islower(outbuf[i]) || outbuf[i] == '\x85')
				outbuf[i] ^= 0x20;
	}

	return (ssize_t)len;

einval:
	errno = EINVAL;
	return -1;
}
