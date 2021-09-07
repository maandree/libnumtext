/* See LICENSE file for copyright and license details. */
#include "common.h"

#define F(FLAG) LIBNUMTEXT_N2T_SWEDISH_##FLAG
#define ORD F(ORDINAL)
#define DENUM F(DENOMINATOR)


#define ORDS(N, Z, E, FLAGS, ERR)\
	{N, Z, E, (FLAGS) | ORD, ERR},\
	{N, Z, E, (FLAGS) | ORD | F(PLURAL_FORM), ERR},\
	{N, Z, E, (FLAGS) | ORD | F(DEFINITE_FORM), ERR},\
	{N, Z, E, (FLAGS) | ORD | F(PLURAL_FORM) | F(DEFINITE_FORM), ERR}

#define DENUMS_(N, Z, E, FLAGS, ERR)\
	{N, Z, E"del", (FLAGS) | DENUM, ERR},\
	{N, Z, E"delar", (FLAGS) | DENUM | F(PLURAL_FORM), ERR},\
	{N, Z, E"delen", (FLAGS) | DENUM | F(DEFINITE_FORM), ERR},\
	{N, Z, E"delarna", (FLAGS) | DENUM | F(PLURAL_FORM) | F(DEFINITE_FORM), ERR}

#define DENUMS(N, Z, E, FLAGS, ERR)\
	DENUMS_(N, Z, E, (FLAGS) | F(COMMON_GENDER), ERR),\
	DENUMS_(N, Z, E, (FLAGS) | F(NEUTER_GENDER), ERR),\
	DENUMS_(N, Z, E, (FLAGS) | F(MASCULINE_GENDER), ERR),\
	DENUMS_(N, Z, E, (FLAGS) | F(FEMININE_GENDER), ERR)

#define DENUMS_AE(N, Z, E, FLAGS, ERR)\
	DENUMS_(N, Z, E"a", (FLAGS) | F(COMMON_GENDER), ERR),\
	DENUMS_(N, Z, E"a", (FLAGS) | F(NEUTER_GENDER), ERR),\
	DENUMS_(N, Z, E"e", (FLAGS) | F(MASCULINE_GENDER), ERR),\
	DENUMS_(N, Z, E"a", (FLAGS) | F(FEMININE_GENDER), ERR)

#define ORDS_DENUMS(N, Z, E, FLAGS, ERR)\
	ORDS(N, Z, E, FLAGS, ERR),\
	DENUMS(N, Z, E, FLAGS, ERR)

#define ORDS_DENUMS_AE(N, Z, E, FLAGS, ERR)\
	ORDS(N, Z, E"a", FLAGS, ERR),\
	DENUMS_AE(N, Z, E, FLAGS, ERR)

#define CARD_DENUMS(N, Z, E, FLAGS, ERR)\
	{N, Z, E, FLAGS, ERR},\
	DENUMS(N, Z, E, FLAGS, ERR)

#define ANY_GENDER(N, Z, E, FLAGS, ERR)\
	{N, Z, E, (FLAGS) | F(COMMON_GENDER), ERR},\
	{N, Z, E, (FLAGS) | F(NEUTER_GENDER), ERR},\
	{N, Z, E, (FLAGS) | F(MASCULINE_GENDER), ERR},\
	{N, Z, E, (FLAGS) | F(FEMININE_GENDER), ERR}

static struct test {
	const char *num;
	size_t zeroes;
	const char *expect;
	uint32_t flags;
	int errnum;
} tests[] = {
	{"0", 0, "noll", 0, 0},
	{"00", 0, "noll", 0, 0},
	{"00000000", 0, "noll", 0, 0},
	{"+0", 0, "plus noll", 0, 0},
	{"-0", 0, "minus noll", 0, 0},
	{UNICODE_MINUS"0", 0, "minus noll", 0, 0},
	ORDS("0", 0, "nollte", 0, 0),
	ORDS("+0", 0, "plus-nollte", 0, 0),
	ORDS("-0", 0, "minus-nollte", 0, 0),
	ORDS(UNICODE_MINUS"0", 0, "minus-nollte", 0, 0),
	DENUMS("0", 0, "nollte", 0, 0),
	DENUMS("+0", 0, "plus-nollte", 0, 0),
	DENUMS("-0", 0, "minus-nollte", 0, 0),
	DENUMS(UNICODE_MINUS"0", 0, "minus-nollte", 0, 0),
	{"1", 0, "ett", 0, 0},
	{"+1", 0, "plus ett", 0, 0},
	{"-1", 0, "minus ett", 0, 0},
	{UNICODE_MINUS"1", 0, "minus ett", 0, 0},
	ORDS("1", 0, "första", 0, 0),
	ORDS("+1", 0, "plus-första", 0, 0),
	ORDS("-1", 0, "minus-första", 0, 0),
	ORDS(UNICODE_MINUS"1", 0, "minus-första", 0, 0),
	ANY_GENDER("1", 0, "hel", DENUM, 0),
	ANY_GENDER("1", 0, "hela", DENUM | F(PLURAL_FORM), 0),
	ANY_GENDER("1", 0, "helan", DENUM | F(DEFINITE_FORM), 0),
	ANY_GENDER("1", 0, "helorna", DENUM | F(PLURAL_FORM) | F(DEFINITE_FORM), 0),
	{"2", 0, "två", 0, 0},
	{"+2", 0, "plus två", 0, 0},
	{"-2", 0, "minus två", 0, 0},
	{UNICODE_MINUS"2", 0, "minus två", 0, 0},
	ORDS("2", 0, "andra", 0, 0),
	ORDS("+2", 0, "plus-andra", 0, 0),
	ORDS("-2", 0, "minus-andra", 0, 0),
	ORDS(UNICODE_MINUS"2", 0, "minus-andra", 0, 0),
	ANY_GENDER("2", 0, "halv", DENUM, 0),
	ANY_GENDER("2", 0, "halvor", DENUM | F(PLURAL_FORM), 0),
	ANY_GENDER("2", 0, "halvan", DENUM | F(DEFINITE_FORM), 0),
	ANY_GENDER("2", 0, "halvorna", DENUM | F(PLURAL_FORM) | F(DEFINITE_FORM), 0),
	{"3", 0, "tre", 0, 0},
	{"+3", 0, "plus tre", 0, 0},
	{"-3", 0, "minus tre", 0, 0},
	{UNICODE_MINUS"3", 0, "minus tre", 0, 0},
	ORDS_DENUMS("3", 0, "tredje", 0, 0),
	ORDS_DENUMS("+3", 0, "plus-tredje", 0, 0),
	ORDS_DENUMS("-3", 0, "minus-tredje", 0, 0),
	ORDS_DENUMS(UNICODE_MINUS"3", 0, "minus-tredje", 0, 0),
	{"4", 0, "fyra", 0, 0},
	ORDS_DENUMS("4", 0, "fjärde", 0, 0),
	{"5", 0, "fem", 0, 0},
	ORDS_DENUMS("5", 0, "femte", 0, 0),
	{"6", 0, "sex", 0, 0},
	ORDS_DENUMS("6", 0, "sjätte", 0, 0),
	{"7", 0, "sju", 0, 0},
	ORDS_DENUMS("7", 0, "sjunde", 0, 0),
	{"8", 0, "åtta", 0, 0},
	ORDS("8", 0, "åttonde", 0, 0),
	DENUMS("8", 0, "åtton", 0, 0),
	{"9", 0, "nio", 0, 0},
	ORDS("9", 0, "nionde", 0, 0),
	DENUMS("9", 0, "nion", 0, 0),
	{"00009", 0, "nio", 0, 0},
	{"10", 0, "tio", 0, 0},
	ORDS("10", 0, "tionde", 0, 0),
	DENUMS("10", 0, "tion", 0, 0),
	{"11", 0, "elva", 0, 0},
	ORDS_DENUMS("11", 0, "elfte", 0, 0),
	{"12", 0, "tolv", 0, 0},
	ORDS_DENUMS("12", 0, "tolfte", 0, 0),
	CARD_DENUMS("13", 0, "tretton", 0, 0),
	ORDS("13", 0, "trettonde", 0, 0),
	CARD_DENUMS("14", 0, "fjorton", 0, 0),
	ORDS("14", 0, "fjortonde", 0, 0),
	CARD_DENUMS("15", 0, "femton", 0, 0),
	ORDS("15", 0, "femtonde", 0, 0),
	CARD_DENUMS("16", 0, "sexton", 0, 0),
	ORDS("16", 0, "sextonde", 0, 0),
	CARD_DENUMS("17", 0, "sjutton", 0, 0),
	ORDS("17", 0, "sjuttonde", 0, 0),
	CARD_DENUMS("18", 0, "arton", 0, 0),
	ORDS("18", 0, "artonde", 0, 0),
	CARD_DENUMS("19", 0, "nitton", 0, 0),
	ORDS("19", 0, "nittonde", 0, 0),
	{"20", 0, "tjugo", 0, 0},
	ORDS("20", 0, "tjugonde", 0, 0),
	DENUMS("20", 0, "tjugon", 0, 0),
	{"21", 0, "tjugoett", 0, 0},
	ORDS_DENUMS_AE("21", 0, "tjugoförst", 0, 0),
	{"22", 0, "tjugotvå", 0, 0},
	ORDS_DENUMS_AE("22", 0, "tjugoandr", 0, 0),
	{"23", 0, "tjugotre", 0, 0},
	ORDS_DENUMS("23", 0, "tjugotredje", 0, 0),
	{"28", 0, "tjugoåtta", 0, 0},
	ORDS("28", 0, "tjugoåttonde", 0, 0),
	DENUMS("28", 0, "tjugoåtton", 0, 0),
	{"29", 0, "tjugonio", 0, 0},
	ORDS("29", 0, "tjugonionde", 0, 0),
	DENUMS("29", 0, "tjugonion", 0, 0),
	{"30", 0, "trettio", 0, 0},
	ORDS("30", 0, "trettionde", 0, 0),
	DENUMS("30", 0, "trettion", 0, 0),
	{"31", 0, "trettioett", 0, 0},
	ORDS_DENUMS_AE("31", 0, "trettioförst", 0, 0),
	{"32", 0, "trettiotvå", 0, 0},
	ORDS_DENUMS_AE("32", 0, "trettioandr", 0, 0),
	{"40", 0, "fyrtio", 0, 0},
	ORDS("40", 0, "fyrtionde", 0, 0),
	DENUMS("40", 0, "fyrtion", 0, 0),
	{"43", 0, "fyrtiotre", 0, 0},
	ORDS_DENUMS("43", 0, "fyrtiotredje", 0, 0),
	{"50", 0, "femtio", 0, 0},
	ORDS("50", 0, "femtionde", 0, 0),
	DENUMS("50", 0, "femtion", 0, 0),
	{"54", 0, "femtiofyra", 0, 0},
	ORDS_DENUMS("54", 0, "femtiofjärde", 0, 0),
	{"60", 0, "sextio", 0, 0},
	ORDS("60", 0, "sextionde", 0, 0),
	DENUMS("60", 0, "sextion", 0, 0),
	{"65", 0, "sextiofem", 0, 0},
	ORDS_DENUMS("65", 0, "sextiofemte", 0, 0),
	{"70", 0, "sjuttio", 0, 0},
	ORDS("70", 0, "sjuttionde", 0, 0),
	DENUMS("70", 0, "sjuttion", 0, 0),
	{"76", 0, "sjuttiosex", 0, 0},
	ORDS_DENUMS("76", 0, "sjuttiosjätte", 0, 0),
	{"80", 0, "åttio", 0, 0},
	ORDS("80", 0, "åttionde", 0, 0),
	DENUMS("80", 0, "åttion", 0, 0),
	{"87", 0, "åttiosju", 0, 0},
	ORDS_DENUMS("87", 0, "åttiosjunde", 0, 0),
	{"90", 0, "nittio", 0, 0},
	ORDS("90", 0, "nittionde", 0, 0),
	DENUMS("90", 0, "nittion", 0, 0),
	{"98", 0, "nittioåtta", 0, 0},
	ORDS("98", 0, "nittioåttonde", 0, 0),
	DENUMS("98", 0, "nittioåtton", 0, 0),
	{"99", 0, "nittionio", 0, 0},
	ORDS("99", 0, "nittionionde", 0, 0),
	DENUMS("99", 0, "nittionion", 0, 0),
	CARD_DENUMS("100", 0, "etthundra", 0, 0), /* Note: IMPLICIT_ONES is not set so DENUM will be "etthundradel" */
	ORDS("100", 0, "etthundrade", 0, 0),
	{"101", 0, "etthundraett", 0, 0},
	ORDS_DENUMS_AE("101", 0, "etthundraförst", 0, 0),
	{"110", 0, "etthundratio", 0, 0},
	ORDS("110", 0, "etthundrationde", 0, 0),
	DENUMS("110", 0, "etthundration", 0, 0),
	{"111", 0, "etthundraelva", 0, 0},
	ORDS("111", 0, "etthundraelfte", 0, 0),
	{"120", 0, "etthundratjugo", 0, 0},
	ORDS("120", 0, "etthundratjugonde", 0, 0),
	DENUMS("120", 0, "etthundratjugon", 0, 0),
	{"121", 0, "etthundratjugoett", 0, 0},
	ORDS_DENUMS_AE("121", 0, "etthundratjugoförst", 0, 0),
	{"999", 0, "niohundranittionio", 0, 0},
	ORDS("999", 0, "niohundranittionionde", 0, 0),
	CARD_DENUMS("1000", 0, "ettusen", 0, 0),
	ORDS("1000", 0, "ettusende", 0, 0),
	{"1001", 0, "ettusenett", 0, 0},
	ORDS_DENUMS_AE("1001", 0, "ettusenförst", 0, 0),
	{"1002", 0, "ettusentvå", 0, 0},
	ORDS_DENUMS_AE("1002", 0, "ettusenandr", 0, 0),
	{"1100", 0, "ettusenetthundra", 0, 0},
	ORDS("1100", 0, "ettusenetthundrade", 0, 0),
	{"1101", 0, "ettusenetthundraett", 0, 0},
	ORDS("1101", 0, "ettusenetthundraförsta", 0, 0),
	{"1123", 0, "ettusenetthundratjugotre", 0, 0},
	ORDS("1123", 0, "ettusenetthundratjugotredje", 0, 0),
	CARD_DENUMS("2000", 0, "tvåtusen", 0, 0),
	ORDS("2000", 0, "tvåtusende", 0, 0),
	{"3000", 0, "tretusen", 0, 0},
	ORDS("3000", 0, "tretusende", 0, 0),
	{"4000", 0, "fyratusen", 0, 0},
	ORDS("4000", 0, "fyratusende", 0, 0),
	{"5000", 0, "femtusen", 0, 0},
	ORDS("5000", 0, "femtusende", 0, 0),
	{"8000", 0, "åttatusen", 0, 0},
	ORDS("8000", 0, "åttatusende", 0, 0),
	{"9000", 0, "niotusen", 0, 0},
	ORDS("9000", 0, "niotusende", 0, 0),
	{"10000", 0, "tiotusen", 0, 0},
	ORDS("10000", 0, "tiotusende", 0, 0),
	{"11000", 0, "elvatusen", 0, 0},
	ORDS("11000", 0, "elvatusende", 0, 0),
	{"20000", 0, "tjugotusen", 0, 0},
	ORDS("20000", 0, "tjugotusende", 0, 0),
	{"21000", 0, "tjugoettusen", 0, 0}, /* "tjugoentusen" is actually incorrect (it's colloquial) */
	{"30000", 0, "trettiotusen", 0, 0},
	{"31000", 0, "trettioettusen", 0, 0},
	{"32000", 0, "trettiotvåtusen", 0, 0},
	{"40001", 0, "fyrtiotusenett", 0, 0},
	{"40001", 0, "fyrtiotusenförsta", ORD, 0},
	{"40010", 0, "fyrtiotusentio", 0, 0},
	{"40010", 0, "fyrtiotusentionde", ORD, 0},
	{"40012", 0, "fyrtiotusentolv", 0, 0},
	{"40012", 0, "fyrtiotusentolfte", ORD, 0},
	{"50100", 0, "femtiotusenetthundra", 0, 0},
	{"50100", 0, "femtiotusenetthundrade", ORD, 0},
	{"50200", 0, "femtiotusentvåhundra", 0, 0},
	{"50200", 0, "femtiotusentvåhundrade", ORD, 0},
	{"50210", 0, "femtiotusentvåhundratio", 0, 0},
	{"50240", 0, "femtiotusentvåhundrafyrtio", 0, 0},
	{"50240", 0, "femtiotusentvåhundrafyrtionde", ORD, 0},
	{"50248", 0, "femtiotusentvåhundrafyrtioåtta", 0, 0},
	{"50248", 0, "femtiotusentvåhundrafyrtioåttonde", ORD, 0},
	{"100000", 0, "etthundratusen", 0, 0},
	{"100000", 0, "etthundratusende", ORD, 0},
	{"100001", 0, "etthundratusenett", 0, 0},
	{"100001", 0, "etthundratusenförsta", ORD, 0},
	{"100014", 0, "etthundratusenfjorton", 0, 0},
	{"100014", 0, "etthundratusenfjortonde", ORD, 0},
	{"100020", 0, "etthundratusentjugo", 0, 0},
	{"100020", 0, "etthundratusentjugonde", ORD, 0},
	{"100026", 0, "etthundratusentjugosex", 0, 0},
	{"100026", 0, "etthundratusentjugosjätte", ORD, 0},
	{"100100", 0, "etthundratusenetthundra", 0, 0},
	{"100100", 0, "etthundratusenetthundrade", ORD, 0},
	{"100200", 0, "etthundratusentvåhundra", 0, 0},
	{"100200", 0, "etthundratusentvåhundrade", ORD, 0},
	{"100210", 0, "etthundratusentvåhundratio", 0, 0},
	{"100247", 0, "etthundratusentvåhundrafyrtiosju", 0, 0},
	{"101000", 0, "etthundraettusen", 0, 0},
	{"101000", 0, "etthundraettusende", ORD, 0},
	{"102000", 0, "etthundratvåtusen", 0, 0},
	{"110000", 0, "etthundratiotusen", 0, 0},
	{"118000", 0, "etthundraartontusen", 0, 0},
	{"130000", 0, "etthundratrettiotusen", 0, 0},
	{"182000", 0, "etthundraåttiotvåtusen", 0, 0},
	{"182000", 0, "etthundraåttiotvåtusende", ORD, 0},
	{"300000", 0, "trehundratusen", 0, 0},
	{"300000", 0, "trehundratusende", ORD, 0},
	CARD_DENUMS("1""000000", 0, "enmiljon", 0, 0),
	ORDS("1""000000", 0, "enmiljonte", 0, 0),
	{"1""200000", 0, "enmiljontvåhundratusen", 0, 0},
	ORDS("1""200000", 0, "enmiljontvåhundratusende", 0, 0),
	{"2""000000", 0, "tvåmiljoner", 0, 0},
	ORDS("2""000000", 0, "tvåmiljonte", 0, 0),
	{"3""000000", 0, "tremiljoner", 0, 0},
	{"5""000000", 0, "femmiljoner", 0, 0},
	ORDS("5""000000", 0, "femmiljonte", 0, 0),
	{"5""010000", 0, "femmiljonertiotusen", 0, 0},
	ORDS("5""010000", 0, "femmiljonertiotusende", 0, 0),
	{"5""010100", 0, "femmiljonertiotusenetthundra", 0, 0},
	ORDS("5""010100", 0, "femmiljonertiotusenetthundrade", 0, 0),
	{"5""010111", 0, "femmiljonertiotusenetthundraelva", 0, 0},
	ORDS("5""010111", 0, "femmiljonertiotusenetthundraelfte", 0, 0),
	{"9""999999", 0, "niomiljonerniohundranittioniotusenniohundranittionio", 0, 0},
	ORDS("9""999999", 0, "niomiljonerniohundranittioniotusenniohundranittionionde", 0, 0),
	{"10""000000", 0, "tiomiljoner", 0, 0},
	DENUMS("10""000000", 0, "tiomiljon", 0, 0),
	ORDS("10""000000", 0, "tiomiljonte", 0, 0),
	{"11""000000", 0, "elvamiljoner", 0, 0},
	ORDS("11""000000", 0, "elvamiljonte", 0, 0),
	{"20""000000", 0, "tjugomiljoner", 0, 0},
	ORDS("20""000000", 0, "tjugomiljonte", 0, 0),
	{"100""000000", 0, "etthundramiljoner", 0, 0},
	ORDS("100""000000", 0, "etthundramiljonte", 0, 0),
	{"101""000000", 0, "etthundraenmiljoner", 0, 0},
	{"102""000000", 0, "etthundratvåmiljoner", 0, 0},
	{"112""000000", 0, "etthundratolvmiljoner", 0, 0},
	{"160""000000", 0, "etthundrasextiomiljoner", 0, 0},
	{"169""000000", 0, "etthundrasextioniomiljoner", 0, 0},
	ORDS("169""000000", 0, "etthundrasextioniomiljonte", 0, 0),
	{"200""000000", 0, "tvåhundramiljoner", 0, 0},
	ORDS("200""000000", 0, "tvåhundramiljonte", 0, 0),
	CARD_DENUMS("1000""000000", 0, "enmiljard", 0, 0),
	ORDS("1000""000000", 0, "enmiljarde", 0, 0),
	{"2000""000000", 0, "tvåmiljarder", 0, 0},
	DENUMS("2000""000000", 0, "tvåmiljard", 0, 0),
	ORDS("2000""000000", 0, "tvåmiljarde", 0, 0),
	{"20000""000000", 0, "tjugomiljarder", 0, 0},
	{"21000""000000", 0, "tjugoenmiljarder", 0, 0},
	{"100000""000000", 0, "etthundramiljarder", 0, 0},
	{"101000""000000", 0, "etthundraenmiljarder", 0, 0},
	ORDS("101000""000000", 0, "etthundraenmiljarde", 0, 0),
	{"101001""000000", 0, "etthundraenmiljarderenmiljon", 0, 0},
	ORDS("101001""000000", 0, "etthundraenmiljarderenmiljonte", 0, 0),
	{"101100""000000", 0, "etthundraenmiljarderetthundramiljoner", 0, 0},
	ORDS("101100""000000", 0, "etthundraenmiljarderetthundramiljonte", 0, 0),
	{"1""000000""000000", 0, "enbiljon", 0, 0},
	{"1""000000""000000", 0, "enbiljonte", ORD, 0},
	{"1""001000""000000", 0, "enbiljonenmiljard", 0, 0},
	{"1""001000""000000", 0, "enbiljonenmiljarde", ORD, 0},
	{"1""001001""000000", 0, "enbiljonenmiljardenmiljon", 0, 0},
	{"1""001001""000000", 0, "enbiljonenmiljardenmiljonte", ORD, 0},
	{"1""001001""001000", 0, "enbiljonenmiljardenmiljonettusen", 0, 0},
	{"1""001001""001000", 0, "enbiljonenmiljardenmiljonettusende", ORD, 0},
	{"1""001001""001001", 0, "enbiljonenmiljardenmiljonettusenett", 0, 0},
	{"1""001001""001001", 0, "enbiljonenmiljardenmiljonettusenförsta", ORD, 0},
	{"2""000000""000000", 0, "tvåbiljoner", 0, 0},
	{"2""000000""000000", 0, "tvåbiljonte", ORD, 0},
	{"1000""000000""000000", 0, "enbiljard", 0, 0},
	{"1000""000000""000000", 0, "enbiljarde", ORD, 0},
	{"1", 3 * 6, "entriljon", 0, 0},
	ORDS("1", 3 * 6, "entriljonte", 0, 0),
	{"1000", 3 * 6, "entriljard", 0, 0},
	{"1", 4 * 6, "enkvadriljon", 0, 0},
	{"1", 5 * 6, "enkvintiljon", 0, 0},
	{"1", 6 * 6, "ensextiljon", 0, 0},
	{"1", 7 * 6, "enseptiljon", 0, 0},
	{"1", 8 * 6, "enoktiljon", 0, 0},
	ORDS("1", 8 * 6, "enoktiljonte", 0, 0),
	{"1", 9 * 6, "ennoniljon", 0, 0},
	{"1", 10 * 6, "endeciljon", 0, 0},
	{"1", 11 * 6, "enundeciljon", 0, 0},
	{"1", 12 * 6, "enduodeciljon", 0, 0},
	{"1", 13 * 6, "entredeciljon", 0, 0},
	CARD_DENUMS("1", 14 * 6, "enkvattuordeciljon", 0, 0),
	{"1", 15 * 6, "enkvindeciljon", 0, 0},
	{"1", 16 * 6, "ensedeciljon", 0, 0},
	{"1", 17 * 6, "enseptendeciljon", 0, 0},
	{"1", 18 * 6, "enoktodeciljon", 0, 0},
	{"1", 19 * 6, "ennovendeciljon", 0, 0},
	{"1", 20 * 6, "envigintiljon", 0, 0},
	{"1", 21 * 6, "enunvigintiljon", 0, 0},
	{"1", 22 * 6, "enduovigintiljon", 0, 0},
	{"1", 23 * 6, "entresvigintiljon", 0, 0},
	{"1", 24 * 6, "enkvattuorvigintiljon", 0, 0},
	{"1", 25 * 6, "enkvinvigintiljon", 0, 0},
	{"1", 26 * 6, "ensesvigintiljon", 0, 0},
	{"1", 27 * 6, "enseptemvigintiljon", 0, 0},
	{"1", 28 * 6, "enoktovigintiljon", 0, 0},
	{"1", 29 * 6, "ennovemvigintiljon", 0, 0},
	{"1", 30 * 6, "entrigintiljon", 0, 0},
	{"1", 31 * 6, "enuntrigintiljon", 0, 0},
	{"1", 32 * 6, "enduotrigintiljon", 0, 0},
	{"1", 33 * 6, "entrestrigintiljon", 0, 0},
	{"1", 34 * 6, "enkvattuortrigintiljon", 0, 0},
	{"1", 35 * 6, "enkvintrigintiljon", 0, 0},
	{"1", 36 * 6, "ensestrigintiljon", 0, 0},
	{"1", 37 * 6, "enseptentrigintiljon", 0, 0},
	{"1", 38 * 6, "enoktotrigintiljon", 0, 0},
	{"1", 39 * 6, "ennoventrigintiljon", 0, 0},
	{"1", 40 * 6, "enkvadragintiljon", 0, 0},
	{"1", 41 * 6, "enunkvadragintiljon", 0, 0},
	{"1", 42 * 6, "enduokvadragintiljon", 0, 0},
	{"1", 43 * 6, "entreskvadragintiljon", 0, 0},
	{"1", 44 * 6, "enkvattuorkvadragintiljon", 0, 0},
	{"1", 45 * 6, "enkvinkvadragintiljon", 0, 0},
	{"1", 46 * 6, "enseskvadragintiljon", 0, 0},
	{"1", 47 * 6, "enseptenkvadragintiljon", 0, 0},
	{"1", 48 * 6, "enoktokvadragintiljon", 0, 0},
	{"1", 49 * 6, "ennovenkvadragintiljon", 0, 0},
	{"1", 50 * 6, "enkvinkvagintiljon", 0, 0},
	{"1", 51 * 6, "enunkvinkvagintiljon", 0, 0},
	{"1", 52 * 6, "enduokvinkvagintiljon", 0, 0},
	{"1", 53 * 6, "entreskvinkvagintiljon", 0, 0},
	{"1", 54 * 6, "enkvattuorkvinkvagintiljon", 0, 0},
	{"1", 55 * 6, "enkvinkvinkvagintiljon", 0, 0},
	{"1", 56 * 6, "enseskvinkvagintiljon", 0, 0},
	{"1", 57 * 6, "enseptenkvinkvagintiljon", 0, 0},
	{"1", 58 * 6, "enoktokvinkvagintiljon", 0, 0},
	{"1", 59 * 6, "ennovenkvinkvagintiljon", 0, 0},
	{"1", 60 * 6, "ensexagintiljon", 0, 0},
	{"1", 61 * 6, "enunsexagintiljon", 0, 0},
	{"1", 62 * 6, "enduosexagintiljon", 0, 0},
	{"1", 63 * 6, "entresexagintiljon", 0, 0},
	{"1", 64 * 6, "enkvattuorsexagintiljon", 0, 0},
	{"1", 65 * 6, "enkvinsexagintiljon", 0, 0},
	{"1", 66 * 6, "ensesexagintiljon", 0, 0},
	{"1", 67 * 6, "enseptensexagintiljon", 0, 0},
	{"1", 68 * 6, "enoktosexagintiljon", 0, 0},
	{"1", 69 * 6, "ennovensexagintiljon", 0, 0},
	{"1", 70 * 6, "enseptuagintiljon", 0, 0},
	{"1", 71 * 6, "enunseptuagintiljon", 0, 0},
	{"1", 72 * 6, "enduoseptuagintiljon", 0, 0},
	{"1", 73 * 6, "entreseptuagintiljon", 0, 0},
	{"1", 74 * 6, "enkvattuorseptuagintiljon", 0, 0},
	{"1", 75 * 6, "enkvinseptuagintiljon", 0, 0},
	{"1", 76 * 6, "enseseptuagintiljon", 0, 0},
	{"1", 77 * 6, "enseptenseptuagintiljon", 0, 0},
	{"1", 78 * 6, "enoktoseptuagintiljon", 0, 0},
	{"1", 79 * 6, "ennovenseptuagintiljon", 0, 0},
	{"1", 80 * 6, "enoktogintiljon", 0, 0},
	{"1", 81 * 6, "enunoktogintiljon", 0, 0},
	{"1", 82 * 6, "enduooktogintiljon", 0, 0},
	{"1", 83 * 6, "entresoktogintiljon", 0, 0},
	{"1", 84 * 6, "enkvattuoroktogintiljon", 0, 0},
	{"1", 85 * 6, "enkvinoktogintiljon", 0, 0},
	{"1", 86 * 6, "ensexoktogintiljon", 0, 0},
	{"1", 87 * 6, "enseptemoktogintiljon", 0, 0},
	{"1", 88 * 6, "enoktooktogintiljon", 0, 0},
	{"1", 89 * 6, "ennovemoktogintiljon", 0, 0},
	{"1", 90 * 6, "ennonagintiljon", 0, 0},
	{"1", 91 * 6, "enunnonagintiljon", 0, 0},
	{"1", 92 * 6, "enduononagintiljon", 0, 0},
	{"1", 93 * 6, "entrenonagintiljon", 0, 0},
	{"1", 94 * 6, "enkvattuornonagintiljon", 0, 0},
	{"1", 95 * 6, "enkvinnonagintiljon", 0, 0},
	{"1", 96 * 6, "ensenonagintiljon", 0, 0},
	{"1", 97 * 6, "enseptenonagintiljon", 0, 0},
	{"1", 98 * 6, "enoktononagintiljon", 0, 0},
	{"1", 99 * 6, "ennovenonagintiljon", 0, 0},
	{"1", 100 * 6, "encentiljon", 0, 0},
	{"1", 101 * 6, "enuncentiljon", 0, 0},
	{"1", 102 * 6, "enduocentiljon", 0, 0},
	{"1", 103 * 6, "entrescentiljon", 0, 0},
	{"1", 104 * 6, "enkvattuorcentiljon", 0, 0},
	{"1", 105 * 6, "enkvincentiljon", 0, 0},
	{"1", 106 * 6, "ensexcentiljon", 0, 0},
	{"1", 107 * 6, "enseptencentiljon", 0, 0},
	{"1", 108 * 6, "enoktocentiljon", 0, 0},
	{"1", 109 * 6, "ennovencentiljon", 0, 0},
	{"1", 110 * 6, "endecicentiljon", 0, 0},
	{"1", 120 * 6, "enviginticentiljon", 0, 0},
	{"1", 130 * 6, "entrigintacentiljon", 0, 0},
	{"1", 140 * 6, "enkvadragintacentiljon", 0, 0},
	{"1", 150 * 6, "enkvinkvagintacentiljon", 0, 0},
	{"1", 160 * 6, "ensexagintacentiljon", 0, 0},
	{"1", 170 * 6, "enseptuagintacentiljon", 0, 0},
	{"1", 177 * 6, "enseptenseptuagintacentiljon", 0, 0},
	{"1", 180 * 6, "enoktogintacentiljon", 0, 0},
	{"1", 190 * 6, "ennonagintacentiljon", 0, 0},
	{"1", 199 * 6, "ennovenonagintacentiljon", 0, 0},
	{"1", 200 * 6, "enducentiljon", 0, 0},
	{"1", 201 * 6, "enunducentiljon", 0, 0},
	{"1", 202 * 6, "enduoducentiljon", 0, 0},
	{"1", 203 * 6, "entreducentiljon", 0, 0},
	{"1", 204 * 6, "enkvattuorducentiljon", 0, 0},
	{"1", 205 * 6, "enkvinducentiljon", 0, 0},
	{"1", 206 * 6, "enseducentiljon", 0, 0},
	{"1", 207 * 6, "enseptenducentiljon", 0, 0},
	{"1", 208 * 6, "enoktoducentiljon", 0, 0},
	{"1", 209 * 6, "ennovenducentiljon", 0, 0},
	{"1", 300 * 6, "entrecentiljon", 0, 0},
	{"1", 301 * 6, "enuntrecentiljon", 0, 0},
	{"1", 302 * 6, "enduotrecentiljon", 0, 0},
	{"1", 303 * 6, "entrestrecentiljon", 0, 0},
	{"1", 304 * 6, "enkvattuortrecentiljon", 0, 0},
	{"1", 305 * 6, "enkvintrecentiljon", 0, 0},
	{"1", 306 * 6, "ensestrecentiljon", 0, 0},
	{"1", 307 * 6, "enseptentrecentiljon", 0, 0},
	{"1", 308 * 6, "enoktotrecentiljon", 0, 0},
	{"1", 309 * 6, "ennoventrecentiljon", 0, 0},
	{"1", 400 * 6, "enkvadringentiljon", 0, 0},
	{"1", 401 * 6, "enunkvadringentiljon", 0, 0},
	{"1", 402 * 6, "enduokvadringentiljon", 0, 0},
	{"1", 403 * 6, "entreskvadringentiljon", 0, 0},
	{"1", 404 * 6, "enkvattuorkvadringentiljon", 0, 0},
	{"1", 405 * 6, "enkvinkvadringentiljon", 0, 0},
	{"1", 406 * 6, "enseskvadringentiljon", 0, 0},
	{"1", 407 * 6, "enseptenkvadringentiljon", 0, 0},
	{"1", 408 * 6, "enoktokvadringentiljon", 0, 0},
	{"1", 409 * 6, "ennovenkvadringentiljon", 0, 0},
	{"1", 500 * 6, "enkvingentiljon", 0, 0},
	{"1", 501 * 6, "enunkvingentiljon", 0, 0},
	{"1", 502 * 6, "enduokvingentiljon", 0, 0},
	{"1", 503 * 6, "entreskvingentiljon", 0, 0},
	{"1", 504 * 6, "enkvattuorkvingentiljon", 0, 0},
	{"1", 505 * 6, "enkvinkvingentiljon", 0, 0},
	{"1", 506 * 6, "enseskvingentiljon", 0, 0},
	{"1", 507 * 6, "enseptenkvingentiljon", 0, 0},
	{"1", 508 * 6, "enoktokvingentiljon", 0, 0},
	{"1", 509 * 6, "ennovenkvingentiljon", 0, 0},
	{"1", 600 * 6, "ensescentiljon", 0, 0},
	{"1", 601 * 6, "enunsescentiljon", 0, 0},
	{"1", 602 * 6, "enduosescentiljon", 0, 0},
	{"1", 603 * 6, "entresescentiljon", 0, 0},
	{"1", 604 * 6, "enkvattuorsescentiljon", 0, 0},
	{"1", 605 * 6, "enkvinsescentiljon", 0, 0},
	{"1", 606 * 6, "ensesescentiljon", 0, 0},
	{"1", 607 * 6, "enseptensescentiljon", 0, 0},
	{"1", 608 * 6, "enoktosescentiljon", 0, 0},
	{"1", 609 * 6, "ennovensescentiljon", 0, 0},
	{"1", 700 * 6, "enseptingentiljon", 0, 0},
	{"1", 701 * 6, "enunseptingentiljon", 0, 0},
	{"1", 702 * 6, "enduoseptingentiljon", 0, 0},
	{"1", 703 * 6, "entreseptingentiljon", 0, 0},
	{"1", 704 * 6, "enkvattuorseptingentiljon", 0, 0},
	{"1", 705 * 6, "enkvinseptingentiljon", 0, 0},
	{"1", 706 * 6, "enseseptingentiljon", 0, 0},
	{"1", 707 * 6, "enseptenseptingentiljon", 0, 0},
	{"1", 708 * 6, "enoktoseptingentiljon", 0, 0},
	{"1", 709 * 6, "ennovenseptingentiljon", 0, 0},
	{"1", 800 * 6, "enoktingentiljon", 0, 0},
	{"1", 801 * 6, "enunoktingentiljon", 0, 0},
	{"1", 802 * 6, "enduooktingentiljon", 0, 0},
	{"1", 803 * 6, "entresoktingentiljon", 0, 0},
	{"1", 804 * 6, "enkvattuoroktingentiljon", 0, 0},
	{"1", 805 * 6, "enkvinoktingentiljon", 0, 0},
	{"1", 806 * 6, "ensexoktingentiljon", 0, 0},
	{"1", 807 * 6, "enseptemoktingentiljon", 0, 0},
	{"1", 808 * 6, "enoktooktingentiljon", 0, 0},
	{"1", 809 * 6, "ennovemoktingentiljon", 0, 0},
	{"1", 900 * 6, "ennongentiljon", 0, 0},
	{"1", 901 * 6, "enunnongentiljon", 0, 0},
	{"1", 902 * 6, "enduonongentiljon", 0, 0},
	{"1", 903 * 6, "entrenongentiljon", 0, 0},
	{"1", 904 * 6, "enkvattuornongentiljon", 0, 0},
	{"1", 905 * 6, "enkvinnongentiljon", 0, 0},
	{"1", 906 * 6, "ensenongentiljon", 0, 0},
	{"1", 907 * 6, "enseptenongentiljon", 0, 0},
	{"1", 908 * 6, "enoktonongentiljon", 0, 0},
	{"1", 909 * 6, "ennovenongentiljon", 0, 0},
	{"1", 990 * 6, "ennonagintanongentiljon", 0, 0},
	{"1", 999 * 6, "ennovenonagintanongentiljon", 0, 0},
	ORDS("1", 999 * 6, "ennovenonagintanongentiljonte", 0, 0),
	CARD_DENUMS("1000", 999 * 6, "ennovenonagintanongentiljard", 0, 0),
	ORDS("1000", 999 * 6, "ennovenonagintanongentiljarde", 0, 0),
	{"0", 0, "noll", F(IMPLICIT_ONE), 0},
	{"0", 0, "nollte", F(IMPLICIT_ONE) | ORD, 0},
	{"1", 0, "ett", F(IMPLICIT_ONE), 0},
	{"1", 0, "första", F(IMPLICIT_ONE) | ORD, 0},
	{"2", 0, "två", F(IMPLICIT_ONE), 0},
	{"2", 0, "andra", F(IMPLICIT_ONE) | ORD, 0},
	{"3", 0, "tre", F(IMPLICIT_ONE), 0},
	{"3", 0, "tredje", F(IMPLICIT_ONE) | ORD, 0},
	{"4", 0, "fyra", F(IMPLICIT_ONE), 0},
	{"20", 0, "tjugo", F(IMPLICIT_ONE), 0},
	{"30", 0, "trettio", F(IMPLICIT_ONE), 0},
	CARD_DENUMS("100", 0, "hundra", F(IMPLICIT_ONE), 0),
	{"100", 0, "hundrade", F(IMPLICIT_ONE) | ORD, 0},
	{"101", 0, "hundraett", F(IMPLICIT_ONE), 0},
	{"101", 0, "hundraförsta", F(IMPLICIT_ONE) | ORD, 0},
	{"110", 0, "hundratio", F(IMPLICIT_ONE), 0},
	{"110", 0, "hundrationde", F(IMPLICIT_ONE) | ORD, 0},
	{"999", 0, "niohundranittionio", F(IMPLICIT_ONE), 0},
	{"999", 0, "niohundranittionionde", F(IMPLICIT_ONE) | ORD, 0},
	{"1000", 0, "tusen", F(IMPLICIT_ONE), 0},
	{"1000", 0, "tusende", F(IMPLICIT_ONE) | ORD, 0},
	{"1001", 0, "tusenett", F(IMPLICIT_ONE), 0},
	{"1001", 0, "tusenförsta", F(IMPLICIT_ONE) | ORD, 0},
	{"2000", 0, "tvåtusen", F(IMPLICIT_ONE), 0},
	{"2000", 0, "tvåtusende", F(IMPLICIT_ONE) | ORD, 0},
	{"100000", 0, "hundratusen", F(IMPLICIT_ONE), 0},
	{"100000", 0, "hundratusende", F(IMPLICIT_ONE) | ORD, 0},
	{"101000", 0, "hundraettusen", F(IMPLICIT_ONE), 0},
	{"101000", 0, "hundraettusende", F(IMPLICIT_ONE) | ORD, 0},
	{"300000", 0, "trehundratusen", F(IMPLICIT_ONE), 0},
	{"1""000000", 0, "enmiljon", F(IMPLICIT_ONE), 0},
	{"1""000000", 0, "enmiljonte", F(IMPLICIT_ONE) | ORD, 0},
	{"1""200000", 0, "enmiljontvåhundratusen", F(IMPLICIT_ONE), 0},
	{"2""000000", 0, "tvåmiljoner", F(IMPLICIT_ONE), 0},
	{"2""000000", 0, "tvåmiljonte", F(IMPLICIT_ONE) | ORD, 0},
	{"100""000000", 0, "hundramiljoner", F(IMPLICIT_ONE), 0},
	DENUMS("100""000000", 0, "hundramiljon", F(IMPLICIT_ONE), 0),
	CARD_DENUMS("1000""000000", 0, "enmiljard", F(IMPLICIT_ONE), 0),
	CARD_DENUMS("1""000000""000000", 0, "enbiljon", F(IMPLICIT_ONE), 0),
	{"1", 4 * 6, "enkvadriljon", F(IMPLICIT_ONE), 0},
	{"1", 100 * 6, "encentiljon", F(IMPLICIT_ONE), 0},
	{"+0", 0, "Plus Noll", F(PASCAL_CASE), 0},
	{"+0", 0, "PLUS NOLL", F(UPPER_CASE), 0},
	{"+0", 0, "Plus noll", F(SENTENCE_CASE), 0},
	{"-0", 0, "Minus Noll", F(PASCAL_CASE), 0},
	{"-0", 0, "MINUS NOLL", F(UPPER_CASE), 0},
	{"-0", 0, "Minus noll", F(SENTENCE_CASE), 0},
	{"54248", 0, "FemtioFyratusenTvåhundraFyrtioÅtta", F(PASCAL_CASE), 0},
	{"54248", 0, "FEMTIOFYRATUSENTVÅHUNDRAFYRTIOÅTTA", F(UPPER_CASE), 0},
	{"54248", 0, "Femtiofyratusentvåhundrafyrtioåtta", F(SENTENCE_CASE), 0},
	{"54248", 0, "femtio-fyratusen-tvåhundra-fyrtio-åtta", F(HYPHENATED), 0},
	{"54248", 0, "Femtio-Fyratusen-Tvåhundra-Fyrtio-Åtta", F(HYPHENATED) | F(PASCAL_CASE), 0},
	{"54248", 0, "FEMTIO-FYRATUSEN-TVÅHUNDRA-FYRTIO-ÅTTA", F(HYPHENATED) | F(UPPER_CASE), 0},
	{"54248", 0, "Femtio-fyratusen-tvåhundra-fyrtio-åtta", F(HYPHENATED) | F(SENTENCE_CASE), 0},
	{"100", 0, "etthundra", F(EXPLICIT_TRIPLETS), 0},
	{"100", 0, "etthundra", F(LATEX_TRIPLETS), 0},
	{"1000", 0, "etttusen", F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "e\"ttusen", F(LATEX_TRIPLETS), 0},
	CARD_DENUMS("1000", 0, "tusen", F(IMPLICIT_ONE) | F(EXPLICIT_TRIPLETS), 0),
	{"1000", 0, "tusen", F(IMPLICIT_ONE) | F(LATEX_TRIPLETS), 0},
	{"0", 0, "noll", F(NEUTER_GENDER), 0},
	{"0", 0, "nollte", F(NEUTER_GENDER) | ORD, 0},
	{"1", 0, "en", F(NEUTER_GENDER), 0},
	{"1", 0, "första", F(NEUTER_GENDER) | ORD, 0},
	{"2", 0, "två", F(NEUTER_GENDER), 0},
	{"2", 0, "andra", F(NEUTER_GENDER) | ORD, 0},
	{"3", 0, "tre", F(NEUTER_GENDER), 0},
	{"3", 0, "tredje", F(NEUTER_GENDER) | ORD, 0},
	{"21", 0, "tjugoen", F(NEUTER_GENDER), 0},
	{"21", 0, "tjugoförsta", F(NEUTER_GENDER) | ORD, 0},
	{"22", 0, "tjugotvå", F(NEUTER_GENDER), 0},
	{"22", 0, "tjugoandra", F(NEUTER_GENDER) | ORD, 0},
	{"23", 0, "tjugotre", F(NEUTER_GENDER), 0},
	{"23", 0, "tjugotredje", F(NEUTER_GENDER) | ORD, 0},
	{"100", 0, "etthundra", F(NEUTER_GENDER), 0},
	{"100", 0, "etthundrade", F(NEUTER_GENDER) | ORD, 0},
	{"101", 0, "etthundraen", F(NEUTER_GENDER), 0},
	{"101", 0, "etthundraförsta", F(NEUTER_GENDER) | ORD, 0},
	{"1000", 0, "ettusen", F(NEUTER_GENDER), 0},
	{"1000", 0, "ettusende", F(NEUTER_GENDER) | ORD, 0},
	{"1001", 0, "ettusenen", F(NEUTER_GENDER), 0},
	{"1001", 0, "ettusenförsta", F(NEUTER_GENDER) | ORD, 0},
	{"1002", 0, "ettusentvå", F(NEUTER_GENDER), 0},
	{"1002", 0, "ettusenandra", F(NEUTER_GENDER) | ORD, 0},
	{"1000000", 0, "enmiljon", F(NEUTER_GENDER), 0},
	{"1000000", 0, "enmiljonte", F(NEUTER_GENDER) | ORD, 0},
	{"1000001", 0, "enmiljonen", F(NEUTER_GENDER), 0},
	{"1000001", 0, "enmiljonförsta", F(NEUTER_GENDER) | ORD, 0},
	{"1000002", 0, "enmiljontvå", F(NEUTER_GENDER), 0},
	{"1000002", 0, "enmiljonandra", F(NEUTER_GENDER) | ORD, 0},
	{"1000003", 0, "enmiljontre", F(NEUTER_GENDER), 0},
	{"1000003", 0, "enmiljontredje", F(NEUTER_GENDER) | ORD, 0},
	{"0", 0, "noll", F(MASCULINE_GENDER), 0},
	{"0", 0, "nollte", F(MASCULINE_GENDER) | ORD, 0},
	{"1", 0, "en", F(MASCULINE_GENDER), 0},
	{"1", 0, "förste", F(MASCULINE_GENDER) | ORD, 0},
	{"2", 0, "två", F(MASCULINE_GENDER), 0},
	{"2", 0, "andre", F(MASCULINE_GENDER) | ORD, 0},
	{"3", 0, "tre", F(MASCULINE_GENDER), 0},
	{"3", 0, "tredje", F(MASCULINE_GENDER) | ORD, 0},
	{"21", 0, "tjugoen", F(MASCULINE_GENDER), 0},
	{"21", 0, "tjugoförste", F(MASCULINE_GENDER) | ORD, 0},
	{"22", 0, "tjugotvå", F(MASCULINE_GENDER), 0},
	{"22", 0, "tjugoandre", F(MASCULINE_GENDER) | ORD, 0},
	{"23", 0, "tjugotre", F(MASCULINE_GENDER), 0},
	{"23", 0, "tjugotredje", F(MASCULINE_GENDER) | ORD, 0},
	{"100", 0, "etthundra", F(MASCULINE_GENDER), 0},
	{"100", 0, "etthundrade", F(MASCULINE_GENDER) | ORD, 0},
	{"101", 0, "etthundraen", F(MASCULINE_GENDER), 0},
	{"101", 0, "etthundraförste", F(MASCULINE_GENDER) | ORD, 0},
	{"1000", 0, "ettusen", F(MASCULINE_GENDER), 0},
	{"1000", 0, "ettusende", F(MASCULINE_GENDER) | ORD, 0},
	{"1001", 0, "ettusenen", F(MASCULINE_GENDER), 0},
	{"1001", 0, "ettusenförste", F(MASCULINE_GENDER) | ORD, 0},
	{"1002", 0, "ettusentvå", F(MASCULINE_GENDER), 0},
	{"1002", 0, "ettusenandre", F(MASCULINE_GENDER) | ORD, 0},
	{"1000000", 0, "enmiljon", F(MASCULINE_GENDER), 0},
	{"1000000", 0, "enmiljonte", F(MASCULINE_GENDER) | ORD, 0},
	{"1000001", 0, "enmiljonen", F(MASCULINE_GENDER), 0},
	{"1000001", 0, "enmiljonförste", F(MASCULINE_GENDER) | ORD, 0},
	{"1000002", 0, "enmiljontvå", F(MASCULINE_GENDER), 0},
	{"1000002", 0, "enmiljonandre", F(MASCULINE_GENDER) | ORD, 0},
	{"1000003", 0, "enmiljontre", F(MASCULINE_GENDER), 0},
	{"1000003", 0, "enmiljontredje", F(MASCULINE_GENDER) | ORD, 0},
	{"0", 0, "noll", F(FEMININE_GENDER), 0},
	{"0", 0, "nollte", F(FEMININE_GENDER) | ORD, 0},
	{"1", 0, "en", F(FEMININE_GENDER), 0},
	{"1", 0, "första", F(FEMININE_GENDER) | ORD, 0},
	{"2", 0, "två", F(FEMININE_GENDER), 0},
	{"2", 0, "andra", F(FEMININE_GENDER) | ORD, 0},
	{"3", 0, "tre", F(FEMININE_GENDER), 0},
	{"3", 0, "tredje", F(FEMININE_GENDER) | ORD, 0},
	{"21", 0, "tjugoen", F(FEMININE_GENDER), 0},
	{"21", 0, "tjugoförsta", F(FEMININE_GENDER) | ORD, 0},
	{"22", 0, "tjugotvå", F(FEMININE_GENDER), 0},
	{"22", 0, "tjugoandra", F(FEMININE_GENDER) | ORD, 0},
	{"23", 0, "tjugotre", F(FEMININE_GENDER), 0},
	{"23", 0, "tjugotredje", F(FEMININE_GENDER) | ORD, 0},
	{"100", 0, "etthundra", F(FEMININE_GENDER), 0},
	{"100", 0, "etthundrade", F(FEMININE_GENDER) | ORD, 0},
	{"101", 0, "etthundraen", F(FEMININE_GENDER), 0},
	{"101", 0, "etthundraförsta", F(FEMININE_GENDER) | ORD, 0},
	{"1000", 0, "ettusen", F(FEMININE_GENDER), 0},
	{"1000", 0, "ettusende", F(FEMININE_GENDER) | ORD, 0},
	{"1001", 0, "ettusenen", F(FEMININE_GENDER), 0},
	{"1001", 0, "ettusenförsta", F(FEMININE_GENDER) | ORD, 0},
	{"1002", 0, "ettusentvå", F(FEMININE_GENDER), 0},
	{"1002", 0, "ettusenandra", F(FEMININE_GENDER) | ORD, 0},
	{"1000000", 0, "enmiljon", F(FEMININE_GENDER), 0},
	{"1000000", 0, "enmiljonte", F(FEMININE_GENDER) | ORD, 0},
	{"1000001", 0, "enmiljonen", F(FEMININE_GENDER), 0},
	{"1000001", 0, "enmiljonförsta", F(FEMININE_GENDER) | ORD, 0},
	{"1000002", 0, "enmiljontvå", F(FEMININE_GENDER), 0},
	{"1000002", 0, "enmiljonandra", F(FEMININE_GENDER) | ORD, 0},
	{"1000003", 0, "enmiljontre", F(FEMININE_GENDER), 0},
	{"1000003", 0, "enmiljontredje", F(FEMININE_GENDER) | ORD, 0},
	{"100", 0, "ett|hundra", F(COMPONENT_HYPHENATION), 0},
	{"100", 0, "ett|hun|dra", F(SYLLABLE_HYPHENATION), 0},
	{"100", 0, "ett|hun¦dra", F(SECONDARY_HYPHENATION), 0},
	{"100", 0, "ett|hundra", F(HYPHENATED) | F(COMPONENT_HYPHENATION), 0},
	{"100", 0, "ett|hun|dra", F(HYPHENATED) | F(SYLLABLE_HYPHENATION), 0},
	{"100", 0, "ett|hun¦dra", F(HYPHENATED) | F(SECONDARY_HYPHENATION), 0},
	{"1000", 0, "ettusen", F(COMPONENT_HYPHENATION), 0},
	{"1000", 0, "ettu|sen", F(SYLLABLE_HYPHENATION), 0},
	{"1000", 0, "ettu¦sen", F(SECONDARY_HYPHENATION), 0},
	{"1000", 0, "ettusen", F(HYPHENATED) | F(COMPONENT_HYPHENATION), 0},
	{"1000", 0, "ettu|sen", F(HYPHENATED) | F(SYLLABLE_HYPHENATION), 0},
	{"1000", 0, "ettu¦sen", F(HYPHENATED) | F(SECONDARY_HYPHENATION), 0},
	{"1000", 0, "e\"ttusen", F(COMPONENT_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "e\"ttu|sen", F(SYLLABLE_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "e\"ttu¦sen", F(SECONDARY_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "e\"ttusen", F(HYPHENATED) | F(COMPONENT_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "e\"ttu|sen", F(HYPHENATED) | F(SYLLABLE_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "e\"ttu¦sen", F(HYPHENATED) | F(SECONDARY_HYPHENATION) | F(LATEX_TRIPLETS), 0},
	{"1000", 0, "ett|tusen", F(COMPONENT_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "ett|tu|sen", F(SYLLABLE_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "ett|tu¦sen", F(SECONDARY_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "ett|tusen", F(HYPHENATED) | F(COMPONENT_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "ett|tu|sen", F(HYPHENATED) | F(SYLLABLE_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"1000", 0, "ett|tu¦sen", F(HYPHENATED) | F(SECONDARY_HYPHENATION) | F(EXPLICIT_TRIPLETS), 0},
	{"54248", 0, "femtio|fyra|tusen|två|hundra|fyrtio|åtta", F(COMPONENT_HYPHENATION), 0},
	{"54248", 0, "fem|tio|fy|ra|tu|sen|två|hun|dra|fyr|tio|åt|ta", F(SYLLABLE_HYPHENATION), 0},
	{"54248", 0, "fem¦tio|fy¦ra|tu¦sen|två|hun¦dra|fyr¦tio|åt¦ta", F(SECONDARY_HYPHENATION), 0},
	{"54248", 0, "femtio-fyra|tusen-två|hundra-fyrtio-åtta", F(HYPHENATED) | F(COMPONENT_HYPHENATION), 0},
	{"54248", 0, "fem|tio-fy|ra|tu|sen-två|hun|dra-fyr|tio-åt|ta", F(HYPHENATED) | F(SYLLABLE_HYPHENATION), 0},
	{"54248", 0, "fem¦tio-fy¦ra|tu¦sen-två|hun¦dra-fyr¦tio-åt¦ta", F(HYPHENATED) | F(SECONDARY_HYPHENATION), 0},
	{"1", 1000 * 6, NULL, 0, EDOM},
	{"1", 1000 * 6, NULL, ORD, EDOM},
	{"0", 0, NULL, 0x3000, EINVAL},
	{"0", 0, NULL, 0x10000, EINVAL},
	{"++1", 0, NULL, 0, EINVAL},
	{"--0", 0, NULL, 0, EINVAL},
	{"-+0", 0, NULL, 0, EINVAL},
	{"+-0", 0, NULL, 0, EINVAL},
	{"x0", 0, NULL, 0, EINVAL},
	{"0x", 0, NULL, 0, EINVAL},
	{"0x0", 0, NULL, 0, EINVAL},
};


int
main(void)
{
	char numbuf[8 << 10];
	char buf[2 << 10];
	ssize_t len, r;
	size_t i, j, z, numlen, failures = 0;
	struct test *t;

	for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
		j = i + 1;
		t = &tests[i];
		numlen = strlen(t->num);
		memcpy(numbuf, t->num, numlen);
		for (z = 0; z < t->zeroes; z++)
			numbuf[numlen++] = '0';
		numbuf[numlen] = '\0';
		len = libnumtext_num2text(buf, sizeof(buf), numbuf, numlen, LIBNUMTEXT_SWEDISH, t->flags);
		if (!t->expect) {
			if (len >= 0)
				fprintf(stderr, "Test %zu was successful but error was expected\n", j);
			else if (errno != t->errnum)
				fprintf(stderr, "Test %zu failed but with wrong error: %s\n", j, strerror(errno));
			else
				continue;
		} else if (len < 0) {
			fprintf(stderr, "Error at test %zu: %s\n", j, strerror(errno));
		} else if ((size_t)len > sizeof(buf)) {
			fprintf(stderr, "Test %zu returned result larger (%zi) than buffer (%zu)\n", j, len, sizeof(buf));
		} else if (!len) {
			fprintf(stderr, "Test %zu returned zero-sized result\n", j);
		} else if (buf[len - 1]) {
			fprintf(stderr, "Test %zu returned non-NUL-terminated result\n", j);
		} else if (strcmp(buf, t->expect)) {
			fprintf(stderr, "Test %zu returned incorrect result: %s\n", j, buf);
		} else if ((r = libnumtext_num2text(buf, 2, numbuf, numlen, LIBNUMTEXT_SWEDISH, t->flags)) != len) {
			fprintf(stderr, "Test %zu has invalid return size when buffer is undersized: %zi\n", j, r);
		} else if (buf[1]) {
			fprintf(stderr, "Test %zu returned non-NUL-terminated result when buffer is undersized\n", j);
		} else if (buf[0] != t->expect[0]) {
			fprintf(stderr, "Test %zu returned incorrect result when buffer is undersized\n", j);
		} else if ((r = libnumtext_num2text(NULL, 0, numbuf, numlen, LIBNUMTEXT_SWEDISH, t->flags)) != len) {
			fprintf(stderr, "Test %zu has invalid return size when buffer is NULL: %zi\n", j, r);
		} else {
			numbuf[numlen + 0] = 'x';
			numbuf[numlen + 1] = '\0';
			libnumtext_num2text(buf, sizeof(buf), numbuf, numlen, LIBNUMTEXT_SWEDISH, t->flags);
			if (r != len) {
				fprintf(stderr, "Test %zu has invalid return size when buffer is not NUL-terminated: %zi\n", j, r);
			} else if (buf[len - 1]) {
				fprintf(stderr, "Test %zu returned non-NUL-terminated result when buffer is not NUL-terminated\n", j);
			} else if (strcmp(buf, t->expect)) {
				fprintf(stderr, "Test %zu has incorrect result when buffer is not NUL-terminated: %s\n", j, buf);
			} else {
				continue;
			}
		}
		if (t->zeroes)
			fprintf(stderr, "\tInput:    \"%s\"e%zu\n", t->num, t->zeroes);
		else
			fprintf(stderr, "\tInput:    %s\n", t->num);
		fprintf(stderr, "\tExpected: %s\n", t->expect ? t->expect : strerror(t->errnum));
		fprintf(stderr, "\tFlags:    0x%08x\n", t->flags);
		failures += 1;
	}

	/* TODO test for separator removal */
	/* TODO test …DENOMINATOR | …ORDINAL */

	if (failures > 0) {
		fprintf(stderr, "Failure count: %zu\n", failures);
		return 1;
	}
	return 0;
}
