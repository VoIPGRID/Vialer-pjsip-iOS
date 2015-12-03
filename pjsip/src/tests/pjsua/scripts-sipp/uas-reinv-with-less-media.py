# $Id: uas-reinv-with-less-media.py 5067 2015-04-13 12:28:02Z nanang $
#
import inc_const as const

PJSUA = ["--null-audio --extra-audio --max-calls=1 $SIPP_URI"]

# Send hold after remote holds (double hold)
PJSUA_EXPECTS = [[0, const.MEDIA_HOLD, ""],
		 [0, "ACK sip:", "H"]
		]
