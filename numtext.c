/* See LICENSE file for copyright and license details. */
#include "common.h"


char *argv0;


int
main(int argc, char *argv[])
{
	if (!argc) {
		fprintf(stderr, "numtext: no command lines arguments specified, don't know what to execute\n");
		return 1;
	}

	argv0 = strrchr(argv[0], '/');
	argv0 = argv0 ? &argv0[1] : argv[0];

	if (!strcmp(argv0, "numtext-strip"))
		return numtext_strip_main(argc, argv);
	else if (!strcmp(argv0, "card2ord"))
		return card2ord_main(argc, argv);
	else if (!strcmp(argv0, "num2text"))
		return num2text_main(argc, argv);

	fprintf(stderr, "%s: not a recognised command for numtext multicall binary\n", argv[0]);
	return 1;
}


int
run(int argc, char *argv[], ssize_t (*callback)(char *outbuf, size_t outbuf_size, const char *num, size_t num_len))
{
	char *line = NULL;
	size_t size = 0;
	ssize_t len;
	int ret = 0;
	char *outbuf = NULL;
	size_t outbuf_size = 0;
	size_t num_len;

	if (argc) {
		for (; *argv; argv++) {
			num_len = strlen(*argv);
			len = callback(outbuf, outbuf_size, *argv, num_len);
			if (len < 0) {
				ret = 1;
				continue;
			}
			if ((size_t)len > outbuf_size) {
				outbuf_size = (size_t)len;
				outbuf = realloc(outbuf, outbuf_size);
				if (!outbuf) {
					fprintf(stderr, "%s: realloc %zu: %s\n", argv0, outbuf_size, strerror(errno));
					exit(1);
				}
			}
			len = callback(outbuf, outbuf_size, *argv, num_len);
			if (len < 0) {
				ret = 1;
				continue;
			}
			printf("%s\n", outbuf);
		}
	} else {
		for (;;) {
			len = getline(&line, &size, stdin);
			if (len == -1)
				break;
			if (len && line[len - 1] == '\n')
				line[--len] = '\0';
			if (!len)
				continue;
			num_len = (size_t)len;
			len = callback(outbuf, outbuf_size, line, num_len);
			if (len < 0) {
				ret = 1;
				continue;
			}
			if ((size_t)len > outbuf_size) {
				outbuf_size = (size_t)len;
				outbuf = realloc(outbuf, outbuf_size);
				if (!outbuf) {
					fprintf(stderr, "%s: realloc %zu: %s\n", argv0, outbuf_size, strerror(errno));
					exit(1);
				}
			}
			len = callback(outbuf, outbuf_size, line, num_len);
			if (len < 0) {
				ret = 1;
				continue;
			}
			printf("%s\n", outbuf);
		}
		if (ferror(stdin)) {
			fprintf(stderr, "%s: getline <stdin>: %s\n", argv0, strerror(errno));
			ret = 1;
		}
		free(line);
	}

	if (fflush(stdout) || fclose(stdout)) {
		fprintf(stderr, "%s: printf: %s\n", argv0, strerror(errno));
		ret = 1;
	}

	free(outbuf);
	return ret;
}


int
get_language(const char *arg, enum libnumtext_language *langp, int *have_langp)
{
	static const struct language {
		enum libnumtext_language value;
		const char *code;
		const char *name;
	} languages[] = {
		{LIBNUMTEXT_SWEDISH, "sv", "swedish"}
	};

	size_t i;

	if (*have_langp)
		return 0;
	*have_langp = 1;

	if (!strcmp(arg, "?")) {
		printf("Languages:\n");
		for (i = 0; i < sizeof(languages) / sizeof(*languages); i++)
			printf("\t%s %s\n", languages[i].code, languages[i].name);
		exit(0);
	} else {
		for (i = 0; i < sizeof(languages) / sizeof(*languages); i++) {
			if (!strcasecmp(arg, languages[i].code) || !strcasecmp(arg, languages[i].name)) {
				*langp = languages[i].value;
				return 1;
			}
		}
		fprintf(stderr, "%s: unrecognised language, use ? to list available languages: %s\n", argv0, arg);
		exit(1);
	}
}


static char *
process_option(char *opt, struct option *options, uint32_t *flagsp, uint32_t *maskedp)
{
	uint32_t flag, mask;
	size_t len;

	if (opt[0] == '?' && (!opt[1] || opt[1] == ',')) {
		printf("Options:\n");
		for (; options->option; options++)
			if (options->option_pattern)
				printf("\t%s\n", options->option_pattern);
		exit(0);
	}

	for (; options->option; options++) {
		len = strlen(options->option);
		if (!strncmp(opt, options->option, len) && (!opt[len] || opt[len] == ',')) {
			flag = options->flag;
			mask = options->mask ? options->mask : options->flag;
			if ((mask & *maskedp) && (*flagsp & mask) != flag) {
				fprintf(stderr, "%s: option conflicts with previously specified option: %.*s\n", argv0, (int)len, opt);
				exit(1);
			}
			*flagsp |= flag;
			*maskedp |= mask;
			return &opt[len];
		}
	}

	fprintf(stderr, "%s: unrecognised option for selected language, use ? to list available options: %s\n", argv0, opt);
	exit(1);
}

void
process_options(char **optss, size_t n_optss, struct option *options, uint32_t *flagsp, uint32_t *maskedp)
{
	uint32_t _masked;
	char *opts;
	size_t i;

	if (!maskedp)
		maskedp = &_masked;
	*flagsp = 0;
	*maskedp = 0;

	for (i = 0; i < n_optss; i++) {
		opts = optss[i];
		for (;;) {
			while (*opts == ',')
				opts = &opts[1];
			if (!*opts)
				break;
			else
				opts = process_option(opts, options, flagsp, maskedp);
		}
	}
}
