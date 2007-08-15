from libdesklets.controls import Control, Interface, Permission
import thread, threading
import cover

class IAlbumCovers(Interface):
	artist		= Permission.WRITE
	album		= Permission.WRITE

	ready		= Permission.READWRITE

	small		= Permission.READ
	medium		= Permission.READ
	large		= Permission.READ


class AudioScrobblerCovers(Control, IAlbumCovers):
	def __init__(self):
		self._artist = None
		self._album = None
		self._ready = True
		self._small = ""
		self._medium = ""
		self._large = ""

		self._running_thread = None

		Control.__init__(self)


	
	def __set_artist(self, value):	self._artist = value
	def __set_album(self, value):	self._album = value

	def __set_ready(self, value):
		self._ready = False

		self._running_thread = thread.start_new_thread(self._get_info, ())

	def __get_ready(self):
		return self._ready

	def __get_small(self):			return self._small
	def __get_medium(self):			return self._medium
	def __get_large(self):			return self._large

	def _get_info(self):
		print "_get_info()"
		result = {}
		try:
			global result
			result = cover.getCoverData(self._artist, self._album)
		except Exception, inst:
			print inst

		if self._running_thread != thread.get_ident():
			return

		lock = thread.allocate_lock()
		lock.acquire()

		print "result =", result
		self._small = result.get('small')
		self._medium = result.get('medium')
		self._large = result.get('large')
		print "medium = %s" % str(self._medium)

		self._ready = True
		self._update("ready")

		lock.release()


	artist = property(fset = __set_artist)
	album  = property(fset = __set_album)

	ready  = property(fget = __get_ready, fset = __set_ready)

	small  = property(fget = __get_small)
	medium = property(fget = __get_medium)
	large  = property(fget = __get_large)

def get_class(): return AudioScrobblerCovers

