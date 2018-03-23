#ifndef SAMPLE_V4L2_DUMP_H__
#define SAMPLE_V4L2_DUMP_H__

#include <linux/dvb/frontend.h>

void dump_enum_fe_caps(enum fe_caps v);
void dump_dvb_frontend_info(const struct dvb_frontend_info *p);
void dump_frontend_status(unsigned int v);
void dump_dtv_property(int fd, __u32 cmd);

#endif /* SAMPLE_V4L2_DUMP_H__ */
