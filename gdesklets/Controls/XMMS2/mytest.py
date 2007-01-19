import xmmsclient
import sys
import signal

media = None

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
	global media
	media = xc.medialib_get_info(id, media_get_info_cb)
	print "media = %s" % repr(media)

def media_get_info_cb(result):
	if result != media:
		print "result = %s" % repr(result)
		print "media  = %s" % repr(media)
		print "!="
		return
	prop = result.get_propdict()

	for k, v in prop:
		print "[%s]\t%s = %s" % (k, v, unicode(prop.get(v)))

	print ""

def plugin_list_cb(result):
	for i in result.get_list():
		print i.get('shortname'), i.get('description')

def xform_media_browse_cb(result):
	print result.get_type()
	print result.get_dict()

def sigint_callback(signal, frame) :
	xc.exit_loop()

print xmmsclient.userconfdir_get()
signal.signal(signal.SIGINT, sigint_callback)


xc = xmmsclient.XMMS ()

while True:
	try:
		print 'trying to connect..'
		xc.connect()
		break
	except IOError:
		pass

	
#res = xc.signal_playback_playtime(pt_callback)
res = xc.broadcast_playback_current_id(playback_current_id_cb)
xc.playback_current_id(playback_current_id_cb)

xc.plugin_list(xmmsclient.PLUGIN_TYPE_ALL, plugin_list_cb)
xc.xform_media_browse("file:///fat16kb/music/", xform_media_browse_cb)

try :
	xc.loop()
except :
	pass

res.disconnect_signal()

# force cleanup
res = None
xc = None
