# $Id: 305_ice_comp_1_2.py 5065 2015-04-13 12:14:02Z nanang $
#
from inc_cfg import *

# Different number of ICE components
test_param = TestParam(
		"Callee=use ICE, caller=use ICE",
		[
			InstanceParam("callee", "--null-audio --use-ice --max-calls=1 --ice-no-rtcp"),
			InstanceParam("caller", "--null-audio --use-ice --max-calls=1")
		]
		)
