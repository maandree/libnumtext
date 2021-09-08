/* See LICENSE file for copyright and license details. */
#ifndef LIBNUMTEXT_H
#define LIBNUMTEXT_H

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


enum libnumtext_language {
	LIBNUMTEXT_SWEDISH
#define LIBNUMTEXT_SWEDISH LIBNUMTEXT_SWEDISH
};


#define LIBNUMTEXT_N2T_SWEDISH_CARDINAL              UINT32_C(0x00000000) /* ett/en, två, tre, fyra, fem, … */
#define LIBNUMTEXT_N2T_SWEDISH_ORDINAL               UINT32_C(0x00000001) /* första/-e, andra/-e, tredje, … */

#define LIBNUMTEXT_N2T_SWEDISH_NUMERATOR             UINT32_C(0)
#define LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR           UINT32_C(0x00000002) /* hel, halv, tredjedel, … [1] */

#define LIBNUMTEXT_N2T_SWEDISH_SINGULAR_FORM         UINT32_C(0)
#define LIBNUMTEXT_N2T_SWEDISH_PLURAL_FORM           UINT32_C(0x00000004) /* hela, halvor, tredjedelar, … [2] */

#define LIBNUMTEXT_N2T_SWEDISH_INDEFINITE_FORM       UINT32_C(0)
#define LIBNUMTEXT_N2T_SWEDISH_DEFINITE_FORM         UINT32_C(0x00000008) /* helan, halvan, tredjelen, … [2] */

#define LIBNUMTEXT_N2T_SWEDISH_COMMON_GENDER         UINT32_C(0x00000000) /* ett/första, två/andra, … */
#define LIBNUMTEXT_N2T_SWEDISH_NEUTER_GENDER         UINT32_C(0x00000010) /* en/första, två/andra, … */
#define LIBNUMTEXT_N2T_SWEDISH_MASCULINE_GENDER      UINT32_C(0x00000020) /* en/förste, två/andre, … */
#define LIBNUMTEXT_N2T_SWEDISH_FEMININE_GENDER       UINT32_C(0x00000030) /* en/första, två/andra, … */

#define LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_ONE          UINT32_C(0x00000000) /* …, ettusen, … */
#define LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE          UINT32_C(0x00000040) /* …, tusen, … (informal unless used with DENOMINATOR) */

#define LIBNUMTEXT_N2T_SWEDISH_NOT_HYPHENATED        UINT32_C(0)
#define LIBNUMTEXT_N2T_SWEDISH_HYPHENATED            UINT32_C(0x00000080) /* …, femhundra-trettio-två, … (avoid) */

#define LIBNUMTEXT_N2T_SWEDISH_LOWER_CASE            UINT32_C(0)          /* …, femhundratrettiotvå, … */
#define LIBNUMTEXT_N2T_SWEDISH_MONEY_CASE            UINT32_C(0x00000100) /* …, FemhundraTrettioTvå, … */
#define LIBNUMTEXT_N2T_SWEDISH_UPPER_CASE            UINT32_C(0x00000200) /* …, FEMHUNDRATRETTIOTVÅ, … */
#define LIBNUMTEXT_N2T_SWEDISH_SENTENCE_CASE         UINT32_C(0x00000300) /* …, Femhundratrettiotvå, … */

#define LIBNUMTEXT_N2T_SWEDISH_NO_HYPHENATION        UINT32_C(0)          /* …, femhundratrettiotvå, … */
#define LIBNUMTEXT_N2T_SWEDISH_COMPONENT_HYPHENATION UINT32_C(0x00000400) /* …, fem|hundra|trettio|två, … */
#define LIBNUMTEXT_N2T_SWEDISH_SYLLABLE_HYPHENATION  UINT32_C(0x00000800) /* …, fem|hun|dra|tret|tio|två, … */
#define LIBNUMTEXT_N2T_SWEDISH_SECONDARY_HYPHENATION UINT32_C(0x00000C00) /* …, fem|hun¦dra|tret¦tio|två, … */

#define LIBNUMTEXT_N2T_SWEDISH_REDUCED_TRIPLETS      UINT32_C(0)          /* …, ettusen, … */
#define LIBNUMTEXT_N2T_SWEDISH_EXPLICIT_TRIPLETS     UINT32_C(0x00001000) /* …, etttusen, … (not acceptable Swedish) */
#define LIBNUMTEXT_N2T_SWEDISH_LATEX_TRIPLETS        UINT32_C(0x00002000) /* …, e"ttusen, … (for use in LaTeX) */

/* [1] If used, also use LIBNUMTEXT_N2T_SWEDISH_IMPLICIT_ONE, otherwise the Swedish becomes odd, and arguably incorrect */
/* [2] Requires LIBNUMTEXT_N2T_SWEDISH_ORDINAL (no effect) or LIBNUMTEXT_N2T_SWEDISH_DENOMINATOR */


/* input to libnumtext_num2text may not contain separators */
ssize_t libnumtext_remove_separators(char *outbuf, size_t outbuf_size, const char *num, size_t num_len,
                                     enum libnumtext_language lang);

ssize_t libnumtext_num2text(char *outbuf, size_t outbuf_size, const char *num, size_t num_len,
                            enum libnumtext_language lang, uint32_t flags, ...);


#endif
