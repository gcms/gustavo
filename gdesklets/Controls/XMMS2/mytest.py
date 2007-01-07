import xmmsclient
import sys
import signal

def pt_callback(self) :
	msec = self.get_uint()
	print "\r%02d:%02d" % (msec / 60000, (msec / 1000) % 60), 
	sys.stdout.flush()
	self.restart()

def playback_current_id_cb(result):
	id = result.get_uint();
	print "current_id = %d" % id
	sys.stdout.flush()

	global xc
	xc.medialib_get_info(id, media_get_info_cb)

def media_get_info_cb(result):
	prop = result.get_propdict()

	for k, v in prop:
		print "[%s]\t%s = %s" % (k, v, prop.get(v))

	print ""


def sigint_callback(signal, frame) :
	xc.exit_loop()

signal.signal(signal.SIGINT, sigint_callback)

xc = xmmsclient.XMMS ()
xc.connect()
	
#res = xc.signal_playback_playtime(pt_callback)
res = xc.broadcast_playback_current_id(playback_current_id_cb)

try :
	xc.loop()
except :
	pass

res.disconnect_signal()

# force cleanup
res = None
xc = None
