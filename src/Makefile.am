
bin_PROGRAMS = sample_dvb

sample_dvb_SOURCES = main.c dump.c

sample_dvb_CPPFLAGS = $(omxil_mf_common_cppflags) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src
sample_dvb_CFLAGS   = $(omxil_mf_common_cflags)
sample_dvb_CXXFLAGS = $(omxil_mf_common_cxxflags)
sample_dvb_LDFLAGS  = $(omxil_mf_common_ldflags) \
	-L$(top_srcdir)/src \
	-lpthread \
	-ldl \
	-lstdc++
