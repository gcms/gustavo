package br.ufg.inf.crawler;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;

import au.id.jericho.lib.html.CharacterReference;
import au.id.jericho.lib.html.Element;
import au.id.jericho.lib.html.HTMLElementName;
import au.id.jericho.lib.html.Segment;
import au.id.jericho.lib.html.Source;
import au.id.jericho.lib.html.StartTag;

public class Parser {

    @SuppressWarnings("unchecked")
    public static Resource getResource(URL url) throws IOException {
	Source src = new Source(url);

	src.fullSequentialParse();

	Resource resource = new Resource();
	resource.setTitle(getTitle(src));

	resource.setKeywordsString(getMetaValue(src, "keywords"));

	List<Element> linkElements = src.findAllElements(HTMLElementName.A);
	for (Element linkElement : linkElements) {
	    String href = linkElement.getAttributeValue("href");
	    if (href == null)
		continue;

	    try {
		resource.addLink(new URL(url, href));
	    } catch (MalformedURLException e) {
		continue;
	    }
	}

	Element bodyElement = src.findNextElement(0, HTMLElementName.BODY);
	Segment contentSegment = (bodyElement == null) ? src : bodyElement
		.getContent();
	// System.out.println(contentSegment.getTextExtractor()
	// .setIncludeAttributes(true).toString());

	resource.setBodyString(contentSegment.getTextExtractor()
		.setIncludeAttributes(true).toString());

	return resource;
    }

    private static String getTitle(Source source) {
	Element titleElement = source.findNextElement(0, HTMLElementName.TITLE);
	if (titleElement == null)
	    return null;
	// TITLE element never contains other tags so just decode it collapsing
	// whitespace:
	return CharacterReference.decodeCollapseWhiteSpace(titleElement
		.getContent());
    }

    private static String getMetaValue(Source source, String key) {
	for (int pos = 0; pos < source.length();) {
	    StartTag startTag = source
		    .findNextStartTag(pos, "name", key, false);
	    if (startTag == null)
		return null;
	    if (startTag.getName() == HTMLElementName.META)
		return startTag.getAttributeValue("content"); // Attribute
	    // values are
	    // automatically
	    // decoded
	    pos = startTag.getEnd();
	}
	return null;
    }

    public static void getResource(String string) throws MalformedURLException,
	    IOException {
	getResource(new URL(string));
    }
}
