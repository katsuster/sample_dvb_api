#ifndef SAMPLE_V4L2_DUMP_H__
#define SAMPLE_V4L2_DUMP_H__

#include <linux/dvb/frontend.h>

typedef void (*dump_func_t)(const struct dtv_property *);

void dump_func_data(const struct dtv_property *prop);
void dump_func_stat(const struct dtv_property *prop);

void dump_enum_fe_caps(enum fe_caps v);
void dump_dvb_frontend_info(const struct dvb_frontend_info *p);
void dump_frontend_status(unsigned int v);
void dump_dtv_stat(int fd, __u32 *cmds, int len, dump_func_t fn);

#endif /* SAMPLE_V4L2_DUMP_H__ */
