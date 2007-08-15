from libdesklets.controls import Control, Interface, Permission
import urllib
import os, thread, threading

import xmmsclient


class IMusicPlayback(Interface):
	get_current	= Permission.READ
	get_artist	= Permission.READ
	get_album	= Permission.READ
	get_title	= Permission.READ
	get_tracknr	= Permission.READ
	get_duration = Permission.READ
	get_date = Permission.READ
	get_genre = Permission.READ
	get_albumart = Permission.READ

	set_server	= Permission.WRITE

	get_playtime = Permission.READ

	start = Permission.READWRITE

class IMusicControl(Interface):
	play	= Permission.WRITE
	stop	= Permission.WRITE
	pause	= Permission.WRITE
	next	= Permission.WRITE
	prev	= Permission.WRITE
	seek	= Permission.WRITE

	set_server = Permission.WRITE
	start = Permission.READWRITE


class IMusic(IMusicPlayback, IMusicControl):
	pass


class XMMS2(Control, IMusic):
	CLIENT_NAME = 'gDesklets-XMMS2'
	def __init__(self):
		self._current		= -1
		self._server		= None
		self._artist		= ""
		self._album			= ""
		self._title			= ""
		self._tracknr		= None
		self._duration		= None
		self._date			= None
		self._genre			= None
		self._playtime		= 0
		self._start			= False

		self._albumart		= None

		# Stores the result of the currently queried media info and playtime
		self._media_info_res = None


		self._confdir		= os.path.join(xmmsclient.userconfdir_get(),
				'clients', XMMS2.CLIENT_NAME)

		Control.__init__(self)
		#self._add_timer(500, self.__start)

	
	def __set_server(self, value):	self._server = value
	def __get_current(self):		return self._current
	def __get_artist(self):			return self._artist
	def __get_album(self):			return self._album
	def __get_title(self):			return self._title
	def __get_tracknr(self):		return self._tracknr
	def __get_duration(self):		return self._duration
	def __get_date(self):			return self._date
	def __get_genre(self):			return self._genre
	def __get_albumart(self):		return self._albumart
	def __get_playtime(self):		return self._playtime
	def __get_start(self):			return self._start
	def __set_start(self, value):
		if value:
			self._start or self.__start()
		else:
			self._start and self.__stop()
		

	def __start(self):
		print "__start()"

		try:
			os.makedirs(self._confdir)
			os.makedirs(os.path.join(self._confdir, 'albumart'))
		except OSError, inst:
			print inst

		def _medialib_get_info_cb(result):
			# if it's not the result of the current song, discard it
			if result != self._media_info_res:
				return
			prop = result.get_propdict()
			self._artist = prop.get('artist')
			self._title = prop.get('title')
			self._album = prop.get('album')
			self._tracknr = prop.get('tracknr')
			self._duration = prop.get('duration')
			self._date = prop.get('date')
			self._genre = prop.get('genre')
			self._update("get_current")

			pic_hash = prop.get('picture_front')
			if pic_hash:
				pic = os.path.join(self._confdir, 'albumart', pic_hash)
				print pic
				if os.path.isfile(pic):
					self._albumart = urllib.pathname2url(pic)
					self._update("get_albumart")
				else:
					def _pic_bindata_cb(result):
						open(pic, 'w').write(result.get_bin())
						self._albumart = urllib.pathname2url(pic)
						self._update("get_albumart")

					self._xc.bindata_retrieve(pic_hash, _pic_bindata_cb)
			else:
				self._albumart = None
				self._update("get_albumart")


			return

		def _playback_playtime_cb(result):
			self._playtime = result.get_uint()
			self._update("get_playtime")
			result.restart()
			return

		def _playback_current_id_cb(result):
			self._current = result.get_uint()
			
			self._media_info_res = self._xc.medialib_get_info(self._current,
					_medialib_get_info_cb)

			return

	
		self._xc = xmmsclient.XMMS(XMMS2.CLIENT_NAME)
		try:
			self._xc.connect(self._server, self.__stop)
		except IOError:
			self._start = False
			return


		self._xc.playback_current_id(_playback_current_id_cb)
		self._xc.broadcast_playback_current_id(_playback_current_id_cb)
		self._xc.signal_playback_playtime(_playback_playtime_cb)

#		self._updated = 0
#		self._refresh = 0
#		self._status = 1
#		self._update("status")
#		gtk.gdk.threads_enter(self._loop, ())
		thread.start_new_thread(self._loop, ())
		self._start = True
		self._update("start")
#		self._add_timer(1000, self.__wait)

	def __stop(self):
		print "__stop()"
		self._xc.exit_loop()
		self._xc = None
		self._start = False
		self._update("start")

	
#	def __wait(self):
#		if self._updated == 0:
#			return 1
#		elif self._updated == 1:
#			self._refresh = 1
#			self._update("refresh")
#			self._status = 0
#			self._update("status")
#			self._add_timer(self._interval, self.__start)
#		return 0

	def _loop(self):
		try:
			self._xc.loop()
		except Exception:
			self._start = False
			self._update("start")



#	def __read_xml(self):
#		self._handler.item = []
#		self._parser.setContentHandler(self._handler)
#
#		try:
#			self._parser.parse(self._url)
#		except:
#			print "NewsGrab\nBad XML data, but will continue...\n"
#			pass
#
#		self._updated = 1


#	def __launch(self, value):
#		p = re.compile('&')
#		url = p.sub('\&', value, 0)
#		threading.Thread(target = os.system, args = ("nice -n 0 %s %s" % (self._browser, url),)).start()
		
		
#	def __get_icon(self):
#		for i in self._logos:
#			m = re.search(i, self._url)
#			if m:
#				return i
#			
#		return "none"


	# Interface
#	get_icon	= property(fget = __get_icon,
#							doc = "Set provider icon")
#	set_url		= property(fset = __set_url,
#							doc = "Set the URL of where to grab the RSS from")
#	set_browser	= property(fset = __set_browser,
#							doc = "Set the default browser")
#	launch		= property(fset = __launch,
#							doc = "Launch the URL within a browser")
#	interval	= property(fset = __set_interval,
#							doc = "Set time between reading news")
#	get_items	= property(__get_items,
#							doc = "Get the news items array")
#	refresh		= property(__get_refresh,
#							doc = "Display refresh trigger")
#	status		= property(__get_status,
#							doc = "Status")
	get_current	= property(fget = __get_current,
			doc = "Get current song id in medialib")
	get_artist	= property(fget = __get_artist,
			doc = "Get the artist of the current playing song")
	get_album	= property(fget = __get_album,
			doc = "Get the album of the current playing song")
	get_title	= property(fget = __get_title,
			doc = "Get the title of the current playing song")
	get_tracknr	= property(fget = __get_tracknr,
			doc = "Get the track number of the current playing song")
	get_duration = property(fget = __get_duration,
			doc = "Get the duration of the current playing song")
	get_date	= property(fget = __get_date,
			doc = "Get the date of the current playing song")
	get_genre	= property(fget = __get_genre,
			doc = "Get the genre of the current playing song")
	get_albumart= property(fget = __get_albumart,
			doc = "Get the uri of the albumart of the current playing song")

	set_server	= property(fset = __set_server,
			doc = "Sets the XMMS2 server to connect")

	get_playtime = property(fget = __get_playtime,
			doc = "Current playing time")

	start 		= property(fget = __get_start, fset = __set_start,
			doc = "Checks if the server started, and start/stop it")

	def __dummy(self, value):
		pass

	play		= property(fset = __dummy)
	stop		= property(fset = __dummy)
	pause		= property(fset = __dummy)
	next		= property(fset = __dummy)
	prev		= property(fset = __dummy)
	seek		= property(fset = __dummy)



def get_class(): return XMMS2

