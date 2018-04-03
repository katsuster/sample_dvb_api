#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static struct dtv_property props[] = {
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
	fprintf(stderr, "usage:\n  %s adapter ch\n", argv[0]);
}

int main(int argc, char *argv[])
{
	char fe_file[1024], demux_file[1024];
	int arg_adapt, arg_ch;
	int fd_fe = -1, fd_demux = -1;
	struct dvb_frontend_info inf_fe;
	unsigned int st_fe;
	struct dtv_properties dtv_prop = {
		.num = ARRAY_SIZE(props),
		.props = props,
	};
	struct dmx_pes_filter_params fil_demux;
	int ret, i;

	if (argc < 3) {
		usage(argc, argv);
		return -1;
	}

	arg_adapt = strtol(argv[1], NULL, 0);
	if (arg_adapt < 0) {
		fprintf(stderr, "Invalid adapter %d.", arg_adapt);
		usage(argc, argv);
		return -1;
	}
	arg_ch = strtol(argv[2], NULL, 0);
	if (arg_ch < 13 || 52 < arg_ch) {
		fprintf(stderr, "Invalid channel %d, 13 ... 52 is available.",
			arg_ch);
		usage(argc, argv);
		return -1;
	}

	snprintf(fe_file, sizeof(fe_file), "/dev/dvb/adapter%d/frontend0",
		arg_adapt);
	fd_fe = open(fe_file, O_RDWR);
	if (fd_fe == -1) {
		perror("open(fe)");
		fprintf(stderr, "Cannot open '%s'.", fe_file);
		goto err_out;
	}

	snprintf(demux_file, sizeof(demux_file), "/dev/dvb/adapter%d/demux0",
		arg_adapt);
	fd_demux = open(demux_file, O_RDWR);
	if (fd_demux == -1) {
		perror("open(demux)");
		fprintf(stderr, "Cannot open '%s'.", demux_file);
		goto err_out;
	}

	ret = ioctl(fd_fe, FE_GET_INFO, &inf_fe);
	if (ret == -1) {
		perror("ioctl(fe, get info)");
		goto err_out;
	}
	dump_dvb_frontend_info(&inf_fe);
	printf("\n");

	ret = ioctl(fd_fe, FE_READ_STATUS, &st_fe);
	if (ret == -1) {
		perror("ioctl(fe, read status)");
		goto err_out;
	}
	dump_frontend_status(st_fe);
	printf("\n");

	for (i = 0; i < ARRAY_SIZE(props); i++) {
		if (props[i].cmd == DTV_FREQUENCY) {
			props[i].u.data = 473142857 + 6000000 * (arg_ch - 13);
		}
	}
	ret = ioctl(fd_fe, FE_SET_PROPERTY, &dtv_prop);
	if (ret == -1) {
		perror("ioctl(fe, set prop)");
		goto err_out;
	}

	fil_demux.pid = 0x2000;
	fil_demux.input = DMX_IN_FRONTEND;
	fil_demux.output = DMX_OUT_TS_TAP;
	fil_demux.pes_type = DMX_PES_VIDEO;
	fil_demux.flags = DMX_IMMEDIATE_START;

	ret = ioctl(fd_demux, DMX_SET_PES_FILTER, &fil_demux);
	if (ret == -1) {
		perror("ioctl(demux, set filter)");
		goto err_out;
	}

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

		ret = ioctl(fd_fe, FE_GET_INFO, &inf_fe);
		if (ret == -1) {
			perror("ioctl(fe, get info2)");
			goto err_out;
		}
		dump_dvb_frontend_info(&inf_fe);
		printf("\n");

		ret = ioctl(fd_fe, FE_READ_STATUS, &st_fe);
		if (ret == -1) {
			perror("ioctl(fe, read status2)");
			goto err_out;
		}
		dump_frontend_status(st_fe);
		printf("\n");

		dump_dtv_stat(fd_fe, cmds_dat, ARRAY_SIZE(cmds_dat),
			dump_func_data);
		dump_dtv_stat(fd_fe, cmds_stat, ARRAY_SIZE(cmds_stat),
			dump_func_stat);
	}

	//success
	ret = 0;

err_out:
	if (fd_demux != -1)
		close(fd_demux);
	if (fd_fe != -1)
		close(fd_fe);

	return ret;
}
