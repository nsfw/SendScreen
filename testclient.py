###############################################################################
# Helpers
###############################################################################

db = {}
def ccoreDump(msg):
    # dump a CommuniCore message to stdout and remember the last value we saw
    # in an in memory database
    global db
    print "path: %s %s %s from: %s" % ( msg.path, type(msg.data), msg.data, msg.source )
    db[msg.path]=msg.data

###############################################################################
# CCore
###############################################################################

# Listen for OSC packets

import CCore
# tcore = CCore.CCore(pubsub="osc-udp://127.0.0.1:9192")
tcore = CCore.CCore(pubsub="osc-udp")
tcore.subscribe("*", ccoreDump)	# catch all 

# run forever
import time
# print "Listening for CCore messages... ^C to quit!\n\n"
while True:
    time.sleep(5.0)
    print "."
