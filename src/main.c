#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <linux/dvb/dmx.h>
#include <linux/dvb/frontend.h>
#include <linux/videodev2.h>

#include "dump.h"
#include "util.h"

enum TRANS {
	TRANS_BS,
	TRANS_CS110,
	TRANS_CS,
};

struct tune_info {
	int adapt;
	int system;
	int fd_fe;
	int fd_demux;
	struct dtv_property *props;
	size_t size_props;

	/* Common */
	int ch;
	__u64 freq;

	/* ISDB-S */
	int trans;
	int tsid;
};

int snprintf_add(char *str, size_t size, const char *format, ...)
{
	char *buf = NULL;
	size_t s;
	int ret = 0;
	va_list ap;

	s = size - strlen(str);
	buf = calloc(s, sizeof(char));
	if (buf == NULL)
		goto err_out;

	va_start(ap, format);
	ret = vsnprintf(buf, s, format, ap);
	va_end(ap);
	strncat(str, buf, size);

err_out:
	free(buf);

	return ret;
}

static struct dtv_property props_s[] = {
	{ .cmd = DTV_CLEAR },
	{ .cmd = DTV_DELIVERY_SYSTEM,   .u.data = SYS_ISDBS, },
	{ .cmd = DTV_FREQUENCY,         .u.data = 1049480000, },
	{ .cmd = DTV_STREAM_ID,         .u.data = 0, },
	{ .cmd = DTV_TUNE },
};

static struct dtv_property props_t[] = {
	{ .cmd = DTV_CLEAR },
	{ .cmd = DTV_DELIVERY_SYSTEM,   .u.data = SYS_ISDBT, },
	{ .cmd = DTV_FREQUENCY,         .u.data = 473142857, },
	/*{ .cmd = DTV_BANDWIDTH_HZ,      .u.data = 6000000, },
	{ .cmd = DTV_INVERSION,         .u.data = INVERSION_AUTO, },
	{ .cmd = DTV_GUARD_INTERVAL,    .u.data = GUARD_INTERVAL_AUTO, },
	{ .cmd = DTV_TRANSMISSION_MODE, .u.data = TRANSMISSION_MODE_AUTO, },

	{ .cmd = DTV_ISDBT_LAYER_ENABLED,      .u.data = -1, },
	{ .cmd = DTV_ISDBT_PARTIAL_RECEPTION,  .u.data = 1, },
	{ .cmd = DTV_ISDBT_SOUND_BROADCASTING, .u.data = 0, },

	{ .cmd = DTV_ISDBT_LAYERA_FEC,               .u.data = FEC_AUTO, },
	{ .cmd = DTV_ISDBT_LAYERA_MODULATION,        .u.data = QPSK, },
	{ .cmd = DTV_ISDBT_LAYERA_SEGMENT_COUNT,     .u.data = 1, },
	{ .cmd = DTV_ISDBT_LAYERA_TIME_INTERLEAVING, .u.data = 8, },

	{ .cmd = DTV_ISDBT_LAYERB_FEC,               .u.data = FEC_AUTO, },
	{ .cmd = DTV_ISDBT_LAYERB_MODULATION,        .u.data = QAM_AUTO, },
	{ .cmd = DTV_ISDBT_LAYERB_SEGMENT_COUNT,     .u.data = 12, },
	{ .cmd = DTV_ISDBT_LAYERB_TIME_INTERLEAVING, .u.data = 2, },*/
	{ .cmd = DTV_TUNE },
};

void usage(int argc, char *argv[])
{
	fprintf(stderr, "usage:\n"
		"  ISDB-T\n"
		"    %s adapter T ch\n"
		"  ISDB-S\n"
		"    %s adapter S [BS|CS110] ch TS-ID\n",
		argv[0], argv[0]);
}

static int open_frontend(struct tune_info *t)
{
	char fe_file[1024], demux_file[1024];

	snprintf(fe_file, sizeof(fe_file),
		"/dev/dvb/adapter%d/frontend0", t->adapt);
	t->fd_fe = open(fe_file, O_RDWR);
	if (t->fd_fe == -1) {
		perror("open(fe)");
		fprintf(stderr, "Cannot open '%s'.\n", fe_file);
		goto err_out;
	}

	snprintf(demux_file, sizeof(demux_file),
		"/dev/dvb/adapter%d/demux0", t->adapt);
	t->fd_demux = open(demux_file, O_RDWR);
	if (t->fd_demux == -1) {
		perror("open(demux)");
		fprintf(stderr, "Cannot open '%s'.\n", demux_file);
		goto err_out;
	}

	return 0;

err_out:
	if (t->fd_demux != -1)
		close(t->fd_demux);
	if (t->fd_fe != -1)
		close(t->fd_fe);

	return -ENODEV;
}

static int close_frontend(struct tune_info *t)
{
	close(t->fd_demux);
	close(t->fd_fe);

	return 0;
}

static int show_frontend(struct tune_info *t)
{
	struct dvb_frontend_info inf_fe;
	unsigned int st_fe;
	int ret;

	while (1) {
		__u32 cmds_dat[] = {
			DTV_FREQUENCY,
		};
		__u32 cmds_stat[] = {
			DTV_STAT_SIGNAL_STRENGTH,
			DTV_STAT_CNR,
			DTV_STAT_PRE_ERROR_BIT_COUNT,
			DTV_STAT_PRE_TOTAL_BIT_COUNT,
			DTV_STAT_POST_ERROR_BIT_COUNT,
			DTV_STAT_POST_TOTAL_BIT_COUNT,
		};

		sleep(1);

		ret = ioctl(t->fd_fe, FE_GET_INFO, &inf_fe);
		if (ret == -1) {
			perror("ioctl(fe, get info)");
			goto err_out;
		}
		dump_dvb_frontend_info(&inf_fe);
		printf("\n");

		ret = ioctl(t->fd_fe, FE_READ_STATUS, &st_fe);
		if (ret == -1) {
			perror("ioctl(fe, read status)");
			goto err_out;
		}
		dump_frontend_status(st_fe);
		printf("\n");

		dump_dtv_stat(t->fd_fe, cmds_dat,
			ARRAY_SIZE(cmds_dat), dump_func_data);
		dump_dtv_stat(t->fd_fe, cmds_stat,
			ARRAY_SIZE(cmds_stat), dump_func_stat);
	}

	//success
	ret = 0;

err_out:
	return ret;
}

static int tune_isdb_s(int argc, char *argv[], struct tune_info *t)
{
	const char *arg_trans;
	int ret, i;

	if (argc < 6) {
		usage(argc, argv);
		return -1;
	}

	arg_trans = argv[3];
	if (strncasecmp(arg_trans, "BS", 2) == 0) {
		t->trans = TRANS_BS;
	} else if (strncasecmp(arg_trans, "CS110", 2) == 0) {
		t->trans = TRANS_CS110;
	} else {
		fprintf(stderr, "Not surppoted '%s'.\n",
			arg_trans);
		return -1;
	}

	t->ch = strtol(argv[4], NULL, 0);
	if (t->trans == TRANS_BS) {
		if (t->ch < 1 || 23 < t->ch) {
			fprintf(stderr, "Invalid channel %d, "
				"BS 1 ... BS 23 is available.\n",
				t->ch);
			return -1;
		}
		t->freq = 1049480000 + 38360000 * (t->ch - 1);
	} else if (t->trans == TRANS_CS110) {
		if (t->ch < 1 || 24 < t->ch) {
			fprintf(stderr, "Invalid channel %d, "
				"ND 1 ... ND 24 is available.\n",
				t->ch);
			return -1;
		}
		t->freq = 1593000000 + 20000000 * (t->ch - 1);
	}

	t->tsid = strtol(argv[5], NULL, 0);
	if (t->tsid < 0) {
		fprintf(stderr, "Invalid TS-ID %d(0x%04x).\n",
			t->tsid, t->tsid);
		return 1;
	}

	for (i = 0; i < t->size_props; i++)
		if (t->props[i].cmd == DTV_STREAM_ID)
			t->props[i].u.data = t->tsid;

	return 0;
}

static int tune_isdb_t(int argc, char *argv[], struct tune_info *t)
{
	int ret, i;

	if (argc < 4) {
		usage(argc, argv);
		return -1;
	}

	t->ch = strtol(argv[3], NULL, 0);
	if (t->ch < 13 || 52 < t->ch) {
		fprintf(stderr, "Invalid channel %d, "
			"13 ... 52 is available.\n", t->ch);
		usage(argc, argv);
		return -1;
	}
	t->freq = (__u64)473142857 + 6000000 * (t->ch - 13);

	//success
	ret = 0;

err_out:
	return ret;
}

static int tune_common(struct tune_info *t)
{
	struct dtv_properties dtv_prop = {
		.num = t->size_props,
		.props = t->props,
	};
	struct dmx_pes_filter_params fil_demux;
	int ret, i;

	for (i = 0; i < t->size_props; i++)
		if (t->props[i].cmd == DTV_FREQUENCY)
			t->props[i].u.data = t->freq;

	ret = ioctl(t->fd_fe, FE_SET_PROPERTY, &dtv_prop);
	if (ret == -1) {
		perror("ioctl(fe, set prop)");
		goto err_out;
	}

	fil_demux.pid = 0x2000;
	fil_demux.input = DMX_IN_FRONTEND;
	fil_demux.output = DMX_OUT_TS_TAP;
	fil_demux.pes_type = DMX_PES_VIDEO;
	fil_demux.flags = DMX_IMMEDIATE_START;

	ret = ioctl(t->fd_demux, DMX_SET_PES_FILTER, &fil_demux);
	if (ret == -1) {
		perror("ioctl(demux, set filter)");
		goto err_out;
	}

	//success
	ret = 0;

err_out:
	return ret;
}

int main(int argc, char *argv[])
{
	struct tune_info tinfo;
	struct tune_info *t = &tinfo;
	const char *arg_system;
	int ret, i;

	memset(t, 0, sizeof(*t));
	t->fd_fe = -1;
	t->fd_demux = -1;

	if (argc < 3) {
		usage(argc, argv);
		return -1;
	}

	t->adapt = strtol(argv[1], NULL, 0);
	if (t->adapt < 0) {
		fprintf(stderr, "Invalid adapter %d.\n", t->adapt);
		usage(argc, argv);
		return 1;
	}

	ret = open_frontend(t);
	if (ret) {
		fprintf(stderr, "Cannot open frontend devices.\n");
		return 2;
	}

	arg_system = argv[2];
	if (strncasecmp(arg_system, "T", 1) == 0) {
		t->system = SYS_ISDBT;
		t->props = props_t;
		t->size_props = ARRAY_SIZE(props_t);

		ret = tune_isdb_t(argc, argv, t);
	} else if (strncasecmp(arg_system, "S", 1) == 0) {
		t->system = SYS_ISDBS;
		t->props = props_s;
		t->size_props = ARRAY_SIZE(props_s);

		ret = tune_isdb_s(argc, argv, t);
	} else {
		fprintf(stderr, "Invalid system '%s'\n",
			arg_system);
		ret = 3;
		goto err_out;
	}
	if (ret) {
		ret = 4;
		goto err_out;
	}

	ret = tune_common(t);
	if (ret) {
		ret = 5;
		goto err_out;
	}

	ret = show_frontend(t);
	if (ret) {
		ret = 6;
		goto err_out;
	}

	//success
	ret = 0;

err_out:
	close_frontend(t);

	return ret;
}
