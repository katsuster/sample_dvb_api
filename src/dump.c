#include <stdio.h>

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

	if (v & FE_NONE)
		snprintf_add(buf, sizeof(buf), "%s, ", "NONE");
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

void dump_dtv_property_cmd(__u32 cmd)
{
	const char *name = "????";


}

void dump_dtv_property(int fd, __u32 cmd)
{
	struct dtv_property prop;
	struct dtv_properties dtv_prop = {
		.num = 1,
		.props = &prop,
	};
	int ret;

	prop.cmd = cmd;

	ret = ioctl(fd, FE_GET_PROPERTY, &dtv_prop);
	if (ret == -1) {
		perror("ioctl(fe, get prop)");
	}

	//if (dump == NULL) {
	{
		printf("dtv_property:%d\n  data: 0x%x\n",
			cmd, prop.u.data);
	}
	//dump();
}
