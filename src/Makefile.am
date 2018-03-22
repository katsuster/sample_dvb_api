
bin_PROGRAMS = sample_v4l

sample_v4l_SOURCES = main.c

sample_v4l_CPPFLAGS = $(omxil_mf_common_cppflags) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src
sample_v4l_CFLAGS   = $(omxil_mf_common_cflags)
sample_v4l_CXXFLAGS = $(omxil_mf_common_cxxflags)
sample_v4l_LDFLAGS  = $(omxil_mf_common_ldflags) \
	-L$(top_srcdir)/src \
	-lpthread \
	-ldl \
	-lstdc++
