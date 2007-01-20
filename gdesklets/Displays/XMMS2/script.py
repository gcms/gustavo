# settings
title_len = 23
title_space = 10
playtime_display = "current"
server = ""
server_not_found = "retry"
retry_time = 10

scroll_interval = 500


artist = ""
album = ""

# used for title scrolling
title = ""
title_idx = 0

def launch():
	global server
	print "now launching"
	iexec = get_control('IExec:8es4mvgrwsac7mp9lexx82qs9-2')
	if server.strip():
		iexec.command = "xmms2-launcher -i %s" % server.strip()
	else:
		iexec.command = "xmms2-launcher"

def start():
	Dsp.artist.value = "Trying to connect..."
	Dsp.cover_img.uri = ""
	Dsp.cover_img.visible = False

	global server
	if server.strip():
		play.set_server = server.strip()

	play.start = True

	if not play.start:
		global server_not_found
		if server_not_found == "launch":
			launch()
			play.start = True
			if not play.start:
				add_timer(int(retry_time) * 1000, start)
		elif server_not_found == "retry":
			add_timer(int(retry_time) * 1000, start)

def start_changed(started):
	Dsp.artist.value = str(play.start)
	if started:
		Dsp.artist.value = "Connected."
		Dsp.title.value = ""

		Dsp.bar.visible = True
		Dsp.mygauge.fill = 0
		Dsp.mygauge.visible = True
		Dsp.time.visible = True
	else:
		Dsp.artist.value = "Not connected"
		Dsp.title.value = ""

		Dsp.bar.visible = False
		Dsp.mygauge.visible = False
		Dsp.time.visible = False
		start()


def current_changed(current):
	global artist
	global title
	global album

	artist = play.get_artist or "Unknown"
	title = play.get_title or "Unknown"
	album = play.get_album or "Unknown"

	Dsp.artist.value = artist
	Dsp.title.value = title

	# scrolling
	global title_idx
	title_idx = 0

def albumart_changed(albumart):
	if albumart:
		Dsp.cover_img.uri = albumart
		Dsp.cover_img.visible = True
	else:
		Dsp.cover_img.uri = ""
		Dsp.cover_img.visible = False
		
def get_time(playtime):
	global playtime_display

	if playtime_display == "remaining":
		secs = (play.get_duration - playtime) / 1000
		return "-%0.2d:%0.2d" % (secs / 60, secs % 60)
	else:
		secs = playtime / 1000
		return "%0.2d:%0.2d" % (secs / 60, secs % 60)


def playtime_changed(playtime):
	secs = playtime / 1000
	#Dsp.time.value = "%0.2d:%0.2d" % (secs / 60, secs % 60)
	Dsp.time.value = get_time(playtime)
	Dsp.mygauge.fill = float(playtime * 100) / float(play.get_duration)

play.bind("get_current", current_changed)
play.bind("get_playtime", playtime_changed)
play.bind("start", start_changed)
play.bind("get_albumart", albumart_changed)

start()

def tick():
	scroll_title()

def scroll_title():
	global title_len
	global title_space

	# eek, hack
	title_len = int(title_len)
	title_space = int(title_space)

	global title
	if len(title) > title_len:
		#title_len = title width
		global title
		global title_idx

		scroll = title + ' ' * max(title_space, title_len - len(title))
		str = scroll[title_idx:title_idx + title_len]
		if len(str) < title_len:
			str = str + scroll[:title_len - (len(scroll) - title_idx)]
		Dsp.title.value = str

		title_idx = (title_idx + 1) % len(scroll)

	global scroll_interval
	add_timer(scroll_interval, tick)

tick()	
