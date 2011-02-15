###############################################################################
# Helpers
###############################################################################

db = {}
def ccoreDump(msg):
    # dump a CommuniCore message to stdout and remember the last value we saw
    # in an in memory database
    print "path: %s %s %s from: %s" % ( msg.path, type(msg.data), msg.data, msg.source )

###############################################################################
# CCore
###############################################################################

# Listen for OSC packets

import CCore
tcore = CCore.CCore(pubsub="osc-udp")
tcore.subscribe("*", ccoreDump)	# catch all 

