// this version of PJSIP will no longer do symmetrical SDP negotiation, because with reinvites it will use its own numbering for codecs again
// and Asterisk will not adopt to those changes.
#define PJMEDIA_SDP_NEG_ANSWER_SYMMETRIC_PT 0
#define PJ_CONFIG_IPHONE 1
#define PJMEDIA_HAS_OPUS_CODEC 1
#define PJMEDIA_HAS_VIDEO 1
#include <VialerPJSIP/pj/config_site_sample.h>
