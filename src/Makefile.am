
bin_PROGRAMS = sample_dvb

sample_dvb_SOURCES = main.c dump.c

sample_dvb_CPPFLAGS = $(sample_dvb_common_cppflags) \
	-I$(top_srcdir)/src
sample_dvb_CFLAGS   = $(sample_dvb_common_cflags)
sample_dvb_CXXFLAGS = $(sample_dvb_common_cxxflags)
sample_dvb_LDFLAGS  = $(sample_dvb_common_ldflags) \
	-L$(top_srcdir)/src
