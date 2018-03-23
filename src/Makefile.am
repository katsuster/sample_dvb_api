
bin_PROGRAMS = sample_v4l2

sample_v4l2_SOURCES = main.c dump.c

sample_v4l2_CPPFLAGS = $(omxil_mf_common_cppflags) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src
sample_v4l2_CFLAGS   = $(omxil_mf_common_cflags)
sample_v4l2_CXXFLAGS = $(omxil_mf_common_cxxflags)
sample_v4l2_LDFLAGS  = $(omxil_mf_common_ldflags) \
	-L$(top_srcdir)/src \
	-lpthread \
	-ldl \
	-lstdc++
