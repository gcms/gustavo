import urllib
import urllib2

from xml.dom import minidom

import os


AUDIOSCROBBLER = "http://ws.audioscrobbler.com/1.0/album/%s/%s/info.xml"

def getURL(artist, album):
	return AUDIOSCROBBLER % (urllib.quote(artist), urllib.quote(album))

def getCoverDataFromURL(url):
	print url
	fp = urllib2.urlopen(url)
	
	doc = minidom.parse(fp)
	return handleAlbum(doc)


def downloadImage(path, link):
	print "%s to %s" % (link, str(path))
	fp = open(path, 'w')
	fp.write(urllib2.urlopen(link).read())


def getCoverData(artist, album):
	print "getCoverData()"
	dirname = os.path.join(os.getenv('HOME'), '.covers',
			artist.upper(), album.upper())
	print os.path.abspath(dirname)

	result = {}
	need_fetch = True
	# check if the dir have it
	for k in ['small', 'medium', 'large']:
		filename = os.path.join(dirname, k)
		result[k] = urllib.pathname2url(filename)
		if os.path.isfile(filename):
			need_fetch = False

	if not need_fetch:
		return result

	links = getCoverDataFromURL(getURL(artist, album))
	print links

	try:
		os.makedirs(dirname)
	except Exception, inst:
		print inst

	for k in ['small', 'medium', 'large']:
		downloadImage(os.path.join(dirname, k), links[k])

	return result

def handleAlbum(album):
   coverart = album.getElementsByTagName("coverart")[0]
   return handleCover(coverart)

def handleCover(coverart):
   result = {}
   for cover in coverart.childNodes:
	   result[cover.nodeName] = getText(cover)
   return result

def getText(node):
	rc = ""
	for child in node.childNodes:
		if child.nodeType == child.TEXT_NODE:
			rc = rc + child.data
	return rc
