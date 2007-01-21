# settings
title_len = 23
title_space = 10
playtime_display = "current"	# "current" or "remaining"
server = ""
server_not_found = "retry"		# "retry" or "launch"
retry_time = 10

scroll_interval = 500


# used for text scrolling
artist = ""
artist_idx = 0
title = ""
title_idx = 0

def launch():
	global server

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
	if started:
		Dsp.artist.value = "Connected."
		Dsp.title.value = ""

		Dsp.bar.visible = True
		Dsp.mygauge.fill = 0
		Dsp.mygauge.visible = True
		Dsp.time.value = ""
		Dsp.time.visible = True
	else:
		Dsp.artist.value = "Disconnected."
		Dsp.title.value = ""

		Dsp.bar.visible = False
		Dsp.mygauge.visible = False
		Dsp.time.visible = False
		start()


def current_changed(current):
	global artist
	global title

	artist = play.get_artist
	title = play.get_title

	Dsp.artist.value = artist or "Unknown"
	Dsp.title.value = title or "Unknown"

	# scrolling
	global title_idx
	title_idx = 0
	global artist_idx
	artist_idx = 0

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
	Dsp.time.value = get_time(playtime)
	Dsp.mygauge.fill = float(playtime * 100) / float(play.get_duration)

play.bind("get_current", current_changed)
play.bind("get_playtime", playtime_changed)
play.bind("start", start_changed)
play.bind("get_albumart", albumart_changed)

start()

def tick():
	scroll_title()

	global scroll_interval
	add_timer(int(scroll_interval), tick)

def scroll_text(text, idx, max_len, space):
	scroll = text + ' ' * max(space, max_len - len(title))
	result = scroll[idx:idx + max_len]
	if len(result) < max_len:
		result = result + scroll[:max_len - (len(scroll) - idx)]

	return ((idx + 1) % len(scroll), result)


def scroll_title():
	global title
	global title_idx
	global title_len
	global title_space


	if title and len(title) > title_len:
		title_idx, Dsp.title.value = scroll_text(title, title_idx,
				int(title_len), int(title_space))

	global artist
	global artist_idx
	if artist and len(artist) > title_len:
		artist_idx, Dsp.artist.value = scroll_text(artist, artist_idx,
				int(title_len), int(title_space))

tick()	
