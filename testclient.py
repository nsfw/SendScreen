###############################################################################
# Helpers
###############################################################################

def ccoreDump(msg):
    # dump a CommuniCore message to stdout and remember the last value we saw
    # in an in memory database
    print "path: %s %s %s from: %s" % ( msg.path, type(msg.data), msg.data, msg.source )

###############################################################################
# CCore
###############################################################################

# Listen for OSC packets

import CCore
# MC listen
tcore = CCore.CCore(pubsub="osc-udp")
tcore.subscribe("*", ccoreDump)	# catch all 

# Unicast list
ttcore = CCore.CCore(pubsub="osc-udp://198.178.187.11:9999")
ttcore.subscribe("*", ccoreDump)

# Unicast list
# define OUT_HOST "198.178.187.122"
# define OUT_PORT 9999
uucore = CCore.CCore(pubsub="osc-udp://198.178.187.122:9999")
uucore.subscribe("*", ccoreDump)

