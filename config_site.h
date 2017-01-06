// this version of PJSIP will no longer do symmetrical SDP negotiation, because with reinvites it will use its own numbering for codecs again
// and Asterisk will not adopt to those changes.
#define PJMEDIA_SDP_NEG_ANSWER_SYMMETRIC_PT 1
#define PJ_CONFIG_IPHONE 1
#define PJMEDIA_HAS_OPUS_CODEC 1
#define PJMEDIA_HAS_VIDEO 1
#define PJ_IPHONE_OS_HAS_MULTITASKING_SUPPORT 0
#define PJ_ACTIVESOCK_TCP_IPHONE_OS_BG 0
#include <pj/config_site_sample.h>
