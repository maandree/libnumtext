/* See LICENSE file for copyright and license details. */
#include "common.h"


#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-macros"
#endif

#define TYPE_INDEX(F)            (ORDINAL(F) | DENOMINATOR(F))
#define FORM_INDEX(F)            ((PLURAL_FORM(F) | DEFINITE_FORM(F)) / LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM)

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
#define MONEY_CASE(F)            (CASE(F) == LIBNUMTEXT_N2T_SWEDISH_MONEY_CASE)
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

#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif


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
} tens[10] = {
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

static struct special_denominator {
	const char *cardinal[4 /* form */];
	const char *ordinal;
} wholes_and_halves[] = {
	{{"Hel",  "He¦la",  "Hel¦an",  "Hel¦or¦na"},  "Hel¦te"},
	{{"Halv", "Hal¦vor", "Halv¦an", "Halv¦or¦na"}, "Half¦te"}
};

static struct denominator_suffix {
	const char *cardinal[4 /* form */];
	const char *ordinal;
} denominator_suffixes = {{"||del",  "||del¦ar",  "||del¦en",  "||del¦ar¦na"},  "||del¦te"};

static const char *signs[2 /* whether + */][4 /* type */] = {
	{"Mi¦nus ", "Mi¦nus-", "Mi¦nus-", "Mi¦nus-"},
	{"Plus ",   "Plus-",   "Plus-",   "Plus-"}
};

static struct great_suffix {
	const char *suffix[2 /* plural? */];
	const char *ordinal_suffix;
} great_suffixes[2 /* times 1000? */] = {
	{{"il¦jon",  "il¦jon¦er"},  "¦te"},
	{{"il¦jard", "il¦jard¦er"}, "<¦e"},
};

static struct great {
	const char *single_digit;
	const char *ones;
	const char *ones_suffixes;
	const char *tens_prefixes;
	const char *tens;
	const char *hundreds_prefixes;
	const char *hundreds;
} greats[] = {
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
	char first;
	const char *append_for_ordinal;
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
			if (SECONDARY_HYPHENATION(flags))
				p = stpcpy(hyphen, "¦");
			else if (SYLLABLE_HYPHENATION(flags))
				p = stpcpy(hyphen, "|");
			else
				p = NULL;
			if (p) {
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

		if (state->len && (HYPHENATED(flags) || HYPHENATION(flags))) {
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
append_final_digit(struct state *state, int digit)
{
	uint32_t f = state->flags;
	struct digit *d = &digits[digit];

	if (ORDINAL(f) || DENOMINATOR(f)) {
		if (MASCULINE_GENDER(f) && d->ordinal_masculine)
			append(state, d->ordinal_masculine);
		else
			append(state, d->ordinal_other);
		state->append_for_ordinal = d->ordinal_suffix;
	} else {
		if (COMMON_GENDER(f) || !d->cardinal_other)
			append(state, d->cardinal_common);
		else
			append(state, d->cardinal_other);
		state->append_for_ordinal = NULL;
	}
}


static void
append_hundreds(struct state *state, int hundreds)
{
	if (hundreds) {
		if (state->first && hundreds == 1 && IMPLICIT_ONE(state->flags)) {
			append(state, "Hun¦dra");
		} else {
			append(state, digits[hundreds].cardinal_common);
			append(state, "||hun¦dra");
		}
		state->append_for_ordinal = "¦de";
		state->first = 0;
	}
}


static void
append_thousands(struct state *state, int ten_thousands, int one_thousands)
{
	if (state->first && ten_thousands == 0 && one_thousands == 1 && IMPLICIT_ONE(state->flags)) {
		append(state, "Tu¦sen");
	} else {
		if (tens[ten_thousands].cardinal)
			append(state, tens[ten_thousands].cardinal);
		else
			one_thousands += ten_thousands * 10;
		if (one_thousands)
			append(state, digits[one_thousands].cardinal_common);
		append(state, "||tu¦sen");
	}
	state->append_for_ordinal = "¦de";
	state->first = 0;
}


PURE static char
get_common_affix(const char *suffixes, const char *prefixes)
{
	const char *s, *p;
	if (suffixes && prefixes)
		for (p = prefixes; *p; p++)
			for (s = suffixes; *s; s++)
				if (*p == *s)
					return *p == '*' ? 's' : *p;
	return '\0';
}


static int
append_great(struct state *state, size_t great_order, size_t great_order_suffix, int plurality)
{
	const char *great_1, *great_1_suffixes, *great_last;
	const char *great_10, *great_10_prefixes;
	const char *great_100, *great_100_prefixes;
	char affix[2] = {[1] = 0};

	if (great_order < 10) {
		append(state, greats[great_order].single_digit);
	} else if (great_order > 999) {
		errno = EDOM;
		return -1;
	} else {
		great_1            = greats[(great_order /   1) % 10].ones;
		great_1_suffixes   = greats[(great_order /   1) % 10].ones_suffixes;
		great_10_prefixes  = greats[(great_order /  10) % 10].tens_prefixes;
		great_10           = greats[(great_order /  10) % 10].tens;
		great_100_prefixes = greats[(great_order / 100) % 10].hundreds_prefixes;
		great_100          = greats[(great_order / 100) % 10].hundreds;
		great_last = NULL;
		if (great_1) {
			append(state, great_1);
			great_last = great_1;
		}
		if (great_10) {
			if ((affix[0] = get_common_affix(great_1_suffixes, great_10_prefixes)))
				append(state, affix);
			append(state, great_10);
			great_last = great_10;
			great_1_suffixes = NULL;
		}
		if (great_100) {
			if ((affix[0] = get_common_affix(great_1_suffixes, great_100_prefixes)))
				append(state, affix);
			append(state, great_100);
			great_last = great_100;
		}
		while (great_last[1])
			great_last = &great_last[1];
		if (*great_last == 'a' || *great_last == 'e' || *great_last == 'i' || *great_last == 'o')
			state->len -= 1;
	}

	append(state, great_suffixes[great_order_suffix].suffix[plurality]);
	state->append_for_ordinal = great_suffixes[great_order_suffix].ordinal_suffix;

	return 0;
}


static void
nul_terminate(struct state *state)
{
	if (state->len < state->outbuf_size)
		state->outbuf[state->len] = '\0';
	else if (state->outbuf_size)
		state->outbuf[state->outbuf_size - 1] = '\0';
	state->len += 1;
}


static void
adjust_case(struct state *state)
{
	char *p = state->outbuf;

	if (!state->outbuf_size)
		return;

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
	 *     = 0x20 (space)
	 */
	if (SENTENCE_CASE(state->flags)) {
		p++;
		while (IS_UTF8_CHAR_CONTINUATION(*p))
			p++;
		goto lower_case;

	} else if (UPPER_CASE(state->flags)) {
		for (; *p; p++)
			if (islower(*p) || *p == '\xa5' || *p == '\xa4' || *p == '\xb6')
				*p ^= 0x20;

	} else if (LOWER_CASE(state->flags)) {
	lower_case:
		for (; *p; p++)
			if (isupper(*p) || *p == '\x85')
				*p ^= 0x20;
	}
}


ssize_t
libnumtext_num2text_swedish__(struct common_num2text_params *params, const char *num, size_t num_len, uint32_t flags)
{
	int hundred_thousands, ten_thousands, one_thousands, hundreds, ones;
	int great_plurality;
	size_t great_order, great_order_suffix, small_order;
	int32_t small_num;
	struct state state;

	if (INVALID_BITS(flags) || X_INVALID_TRIPLETS(flags))
		goto einval;
	if (PLURAL_FORM(flags) || DEFINITE_FORM(flags))
		if (!ORDINAL(flags) && !DENOMINATOR(flags))
			goto einval;

	state.outbuf = params->outbuf;
	state.outbuf_size = params->outbuf_size;
	state.len = 0;
	state.flags = flags;
	state.double_char = 0;
	state.first = 1;
	state.append_for_ordinal = NULL;

	if (params->sign_length)
		append(&state, signs[num[0] == '+'][TYPE_INDEX(flags)]);

	num += params->number_offset;
	num_len -= params->number_offset;

	if (num_len == 1 && '1' <= num[0] && num[0] <= '2' && DENOMINATOR(flags)) {
		if (ORDINAL(flags))
			append(&state, wholes_and_halves[num[0] - '1'].ordinal);
		else
			append(&state, wholes_and_halves[num[0] - '1'].cardinal[FORM_INDEX(flags)]);
		goto out;
	}

	while (num_len) {
		num_len -= 1;
		great_order = num_len / 6;
		small_order = num_len % 6;

		great_order_suffix = 0;
		if (great_order && small_order >= 3) {
			small_order -= 3;
			great_order_suffix = 1;
		}

		small_num = 0;
		hundred_thousands = ten_thousands = one_thousands = hundreds = ones = 0;

		switch (small_order) {
		case 5:
			hundred_thousands = *num++ - '0';
			small_num = (int32_t)hundred_thousands;
			num_len--;
			append_hundreds(&state, hundred_thousands);
			FALL_THROUGH
			/* fall through */

		case 4:
			ten_thousands = *num++ - '0';
			small_num *= 10;
			small_num += (int32_t)ten_thousands;
			num_len--;
			FALL_THROUGH
			/* fall through */

		case 3:
			one_thousands = *num++ - '0';
			small_num *= 10;
			small_num += (int32_t)one_thousands;
			num_len--;
			if (small_num)
				append_thousands(&state, ten_thousands, one_thousands);
			FALL_THROUGH
			/* fall through */

		case 2:
			hundreds = *num++ - '0';
			small_num *= 10;
			small_num += (int32_t)hundreds;
			num_len--;
			append_hundreds(&state, hundreds);
			FALL_THROUGH
			/* fall through */

		case 1:
			ones = *num++ - '0';
			small_num *= 10;
			small_num += (int32_t)ones;
			num_len--;
			if (tens[ones].cardinal) {
				if (!great_order && (DENOMINATOR(flags) || ORDINAL(flags)) && *num == '0') {
					append(&state, tens[ones].ordinal);
					state.append_for_ordinal = "¦de";
				} else {
					append(&state, tens[ones].cardinal);
					state.append_for_ordinal = NULL;
				}
				ones = 0;
				state.first = 0;
			} else {
				ones *= 10;
			}
			FALL_THROUGH
			/* fall through */

		case 0:
			small_num *= 10;
			small_num += (int32_t)(*num - '0');
			ones += *num++ - '0';
			if (ones || (state.first && !great_order)) {
				state.append_for_ordinal = NULL;
				if (!great_order)
					append_final_digit(&state, ones);
				else if (digits[ones].cardinal_other)
					append(&state, digits[ones].cardinal_other);
				else
					append(&state, digits[ones].cardinal_common);
				state.first = 0;
			}
			break;
		}

		if (great_order && small_num) {
			great_plurality = (num_len > params->trailing_zeroes || (CARDINAL(flags) && NUMERATOR(flags)));
			great_plurality = (great_plurality && small_num != 1);
			if (append_great(&state, great_order, great_order_suffix, great_plurality))
				return -1;
		}
	}

	if (DENOMINATOR(flags))
		append(&state, ORDINAL(flags) ? denominator_suffixes.ordinal : denominator_suffixes.cardinal[FORM_INDEX(flags)]);
	else if (ORDINAL(flags) && state.append_for_ordinal)
		append(&state, state.append_for_ordinal);

out:
	nul_terminate(&state);
	adjust_case(&state);
	return (ssize_t)state.len;

einval:
	errno = EINVAL;
	return -1;
}

#undef TYPE_INDEX
#undef FORM_INDEX
#undef CARDINAL
#undef ORDINAL
#undef NUMERATOR
#undef DENOMINATOR
#undef SINGULAR_FORM
#undef PLURAL_FORM
#undef INDEFINITE_FORM
#undef DEFINITE_FORM
#undef GENDER
#undef COMMON_GENDER
#undef NEUTER_GENDER
#undef MASCULINE_GENDER
#undef FEMININE_GENDER
#undef EXPLICIT_ONE
#undef IMPLICIT_ONE
#undef NOT_HYPHENATED
#undef HYPHENATED
#undef CASE
#undef LOWER_CASE
#undef MONEY_CASE
#undef UPPER_CASE
#undef SENTENCE_CASE
#undef HYPHENATION
#undef NO_HYPHENATION
#undef COMPONENT_HYPHENATION
#undef SYLLABLE_HYPHENATION
#undef SECONDARY_HYPHENATION
#undef TRIPLETS
#undef REDUCED_TRIPLETS
#undef EXPLICIT_TRIPLETS
#undef LATEX_TRIPLETS
#undef X_INVALID_TRIPLETS
#undef INVALID_BITS



#define GENDER(F)                ((F) & UINT32_C(0x00000003))
#define MASCULINE_GENDER(F)      (GENDER(F) == LIBNUMTEXT_C2O_SWEDISH_MASCULINE_GENDER)
#define UPPER_CASE(F)            ((F) & LIBNUMTEXT_C2O_SWEDISH_UPPER_CASE)
#define INVALID_BITS(F)          ((F) & (uint32_t)~UINT32_C(0x00000007))

ssize_t
libnumtext_card2ord_swedish__(char *outbuf, size_t outbuf_size, const char *num, size_t num_len, uint32_t flags)
{
	size_t i = 0, length = 0;
	char last_digit;

	if (INVALID_BITS(flags))
		goto einval;

	if (num_len) {
		if (num[0] == '+' || num[0] == '-')
			i += 1;
		else if (IS_UNICODE_MINUS(num, num_len))
			i += sizeof(UNICODE_MINUS) - 1;
	}

	while (i < num_len) {
		if (isdigit(num[i]))
			length = i += 1;
		else if (IS_UNICODE_NBSP(&num[i], num_len - i))
			i += sizeof(UNICODE_NBSP) - 1;
		else if (num[i] != ' ' && num[i] != '\'' && num[i] != '.')
			goto einval;
	}

	if (!length)
		goto einval;

	last_digit = num[length - 1];

	memcpy(outbuf, num, length < outbuf_size ? length : outbuf_size);
	if (length < outbuf_size)
		outbuf[length] = ':';
	length += 1;

	if (length < outbuf_size) {
		if (MASCULINE_GENDER(flags) || last_digit == '0' || last_digit > '2')
			outbuf[length] = UPPER_CASE(flags) ? 'E' : 'e';
		else
			outbuf[length] = UPPER_CASE(flags) ? 'A' : 'a';
	}
	length += 1;

	if (outbuf_size)
		outbuf[length < outbuf_size ? length : outbuf_size] = '\0';
	length += 1;

	return (ssize_t)length;

einval:
	errno = EINVAL;
	return -1;
}

#undef GENDER
#undef MASCULINE_GENDER
#undef UPPER_CASE
#undef INVALID_BITS
