#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>

#include "dump.h"
#include "util.h"

void dump_enum_fe_caps(enum fe_caps v)
{
	char buf[4096] = "";

	if (v & FE_IS_STUPID)
		snprintf_add(buf, sizeof(buf), "%s, ", "STUPID");
	if (v & FE_CAN_INVERSION_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "INVERSION_AUTO");
	if (v & FE_CAN_FEC_1_2)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_1_2");
	if (v & FE_CAN_FEC_2_3)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_2_3");
	if (v & FE_CAN_FEC_3_4)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_3_4");
	if (v & FE_CAN_FEC_4_5)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_4_5");
	if (v & FE_CAN_FEC_5_6)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_5_6");
	if (v & FE_CAN_FEC_6_7)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_6_7");
	if (v & FE_CAN_FEC_7_8)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_7_8");
	if (v & FE_CAN_FEC_8_9)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_8_9");
	if (v & FE_CAN_FEC_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "FEC_AUTO");
	if (v & FE_CAN_QPSK)
		snprintf_add(buf, sizeof(buf), "%s, ", "QPSK");
	if (v & FE_CAN_QAM_16)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_16");
	if (v & FE_CAN_QAM_32)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_32");
	if (v & FE_CAN_QAM_64)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_64");
	if (v & FE_CAN_QAM_128)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_128");
	if (v & FE_CAN_QAM_256)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_256");
	if (v & FE_CAN_QAM_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "QAM_AUTO");
	if (v & FE_CAN_TRANSMISSION_MODE_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "TRANS_AUTO");
	if (v & FE_CAN_BANDWIDTH_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "BANDWIDTH_AUTO");
	if (v & FE_CAN_GUARD_INTERVAL_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "GUARD_AUTO");
	if (v & FE_CAN_HIERARCHY_AUTO)
		snprintf_add(buf, sizeof(buf), "%s, ", "HIERARCHY_AUTO");
	if (v & FE_CAN_8VSB)
		snprintf_add(buf, sizeof(buf), "%s, ", "8VSB");
	if (v & FE_CAN_16VSB)
		snprintf_add(buf, sizeof(buf), "%s, ", "16VSB");
	if (v & FE_HAS_EXTENDED_CAPS)
		snprintf_add(buf, sizeof(buf), "%s, ", "EXTENDED");
	if (v & FE_CAN_MULTISTREAM)
		snprintf_add(buf, sizeof(buf), "%s, ", "MULTISTREAM");
	if (v & FE_CAN_TURBO_FEC)
		snprintf_add(buf, sizeof(buf), "%s, ", "TURBO_FEC");
	if (v & FE_CAN_2G_MODULATION)
		snprintf_add(buf, sizeof(buf), "%s, ", "2G_MOD");
	if (v & FE_NEEDS_BENDING)
		snprintf_add(buf, sizeof(buf), "%s, ", "BENDING");
	if (v & FE_CAN_RECOVER)
		snprintf_add(buf, sizeof(buf), "%s, ", "RECOVER");
	if (v & FE_CAN_MUTE_TS)
		snprintf_add(buf, sizeof(buf), "%s, ", "MUTE_TS");

	printf("    %s\n", buf);
}

void dump_dvb_frontend_info(const struct dvb_frontend_info *p)
{
	printf("dvb_frontend_info:\n"
		"  name                 : %s\n"
		"  frequency            : %d - %d\n"
		"  frequency_stepsize   : %d\n"
		"  frequency_tolerance  : %d\n"
		"  symbol_rate          : %d - %d\n"
		"  symbol_rate_tolerance: %d\n"
		"  notifier_delay       : %d\n"
		"  caps                 : 0x%x\n"
		"",
		p->name,
		p->frequency_min, p->frequency_max,
		p->frequency_stepsize, p->frequency_tolerance,
		p->symbol_rate_min, p->symbol_rate_max,
		p->symbol_rate_tolerance,
		p->notifier_delay,
		p->caps);

	dump_enum_fe_caps(p->caps);
}

void dump_frontend_status(unsigned int v)
{
	char buf[4096] = "";

	printf("frontend status:\n  status: 0x%x\n", v);

	//if (v & FE_NONE)
	//	snprintf_add(buf, sizeof(buf), "%s, ", "NONE");
	if (v & FE_HAS_SIGNAL)
		snprintf_add(buf, sizeof(buf), "%s, ", "HAS_SIGNAL");
	if (v & FE_HAS_CARRIER)
		snprintf_add(buf, sizeof(buf), "%s, ", "HAS_CARRIER");
	if (v & FE_HAS_VITERBI)
		snprintf_add(buf, sizeof(buf), "%s, ", "HAS_VITERBI");
	if (v & FE_HAS_SYNC)
		snprintf_add(buf, sizeof(buf), "%s, ", "HAS_SYNC");
	if (v & FE_HAS_LOCK)
		snprintf_add(buf, sizeof(buf), "%s, ", "HAS_LOCK");
	if (v & FE_TIMEDOUT)
		snprintf_add(buf, sizeof(buf), "%s, ", "TIMEDOUT");
	if (v & FE_REINIT)
		snprintf_add(buf, sizeof(buf), "%s, ", "REINIT");

	printf("    %s\n", buf);
}

const char *dump_dtv_property_cmd(__u32 cmd)
{
	const char *name = "????";

	switch (cmd) {
	case DTV_UNDEFINED:
		name = "UNDEFINED";
		break;
	case DTV_TUNE:
		name = "TUNE";
		break;
	case DTV_CLEAR:
		name = "CLEAR";
		break;
	case DTV_FREQUENCY:
		name = "FREQUENCY";
		break;
	case DTV_MODULATION:
		name = "MODULATION";
		break;
	case DTV_BANDWIDTH_HZ:
		name = "BANDWIDTH_HZ";
		break;
	case DTV_INVERSION:
		name = "INVERSION";
		break;
	case DTV_DISEQC_MASTER:
		name = "DISEQC_MASTER";
		break;
	case DTV_SYMBOL_RATE:
		name = "SYMBOL_RATE";
		break;
	case DTV_INNER_FEC:
		name = "INNER_FEC";
		break;
	case DTV_VOLTAGE:
		name = "VOLTAGE";
		break;
	case DTV_TONE:
		name = "TONE";
		break;
	case DTV_PILOT:
		name = "PILOT";
		break;
	case DTV_ROLLOFF:
		name = "ROLLOFF";
		break;
	case DTV_DISEQC_SLAVE_REPLY:
		name = "DISEQC_SLAVE_REPLY";
		break;
	case DTV_STAT_SIGNAL_STRENGTH:
		name = "SIGNAL_STRENGTH";
		break;
	case DTV_STAT_CNR:
		name = "CNR";
		break;
	case DTV_STAT_PRE_ERROR_BIT_COUNT:
		name = "PRE_ERROR_BIT_COUNT";
		break;
	case DTV_STAT_PRE_TOTAL_BIT_COUNT:
		name = "PRE_TOTAL_BIT_COUNT";
		break;
	case DTV_STAT_POST_ERROR_BIT_COUNT:
		name = "POST_ERROR_BIT_COUNT";
		break;
	case DTV_STAT_POST_TOTAL_BIT_COUNT:
		name = "POST_TOTAL_BIT_COUNT";
		break;
	case DTV_STAT_ERROR_BLOCK_COUNT:
		name = "ERROR_BLOCK_COUNT";
		break;
	case DTV_STAT_TOTAL_BLOCK_COUNT:
		name = "TOTAL_BLOCK_COUNT";
		break;
	default:
		name = "unknown";
		break;
	}

	return name;
}

void dump_func_data(const struct dtv_property *prop)
{
	printf("  %20s(%2d): data:%d(0x%x)\n",
		dump_dtv_property_cmd(prop->cmd), prop->cmd,
		prop->u.data, prop->u.data);
}

void dump_func_stat(const struct dtv_property *prop)
{
	printf("  %20s(%2d): len:%d, "
		"scale:%d, data:%lld(0x%llx)\n",
		dump_dtv_property_cmd(prop->cmd),
		prop->cmd, prop->u.st.len,
		prop->u.st.stat[0].scale,
		prop->u.st.stat[0].uvalue,
		prop->u.st.stat[0].uvalue);
}

void dump_dtv_stat(int fd, __u32 *cmds, int len, dump_func_t fn)
{
	struct dtv_property *props;
	struct dtv_properties dtv_prop;
	int ret, i;

	if (len <= 0)
		return;

	props = calloc(len, sizeof(struct dtv_property));
	if (props == NULL)
		return;

	for (i = 0; i < len; i++) {
		props[i].cmd = cmds[i];
	}

	dtv_prop.num = len;
	dtv_prop.props = props;
	ret = ioctl(fd, FE_GET_PROPERTY, &dtv_prop);
	if (ret == -1) {
		perror("ioctl(fe, get prop)");
	}

	for (i = 0; i < len; i++) {
		if (fn)
			fn(&props[i]);
	}

	free(props);
}
