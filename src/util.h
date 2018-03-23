#ifndef SAMPLE_V4L2_UTIL_H__
#define SAMPLE_V4L2_UTIL_H__

#define ARRAY_SIZE(x)    (sizeof(x) / sizeof((x)[0]))

int snprintf_add(char *str, size_t size, const char *format, ...)
	__attribute__ ((format (printf, 3, 4)));

#endif /* SAMPLE_V4L2_UTIL_H__ */
