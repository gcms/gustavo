package br.ufg.inf.crawler;
import java.net.URL;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

public class Resource {
    private String title;

    private Set<String> keywords = new HashSet<String>();

    private Set<URL> links = new HashSet<URL>();

    private Set<String> words = new HashSet<String>();

    public String getTitle() {
	return title;
    }

    public void setTitle(String title) {
	this.title = title;
    }

    public void setKeywordsString(String keywordsString) {
	keywords.clear();
	if (keywordsString == null)
	    return;

	Scanner scanner = new Scanner(keywordsString);

	scanner.useDelimiter(",");

	while (scanner.hasNext()) {
	    keywords.add(scanner.next());
	}
    }

    public Set<String> getKeywords() {
	return keywords;
    }

    public void addLink(URL link) {
	links.add(link);
    }

    public void setBodyString(String string) {
	string = Utils.normalize(string);

//	System.out.println(string);

	Scanner scanner = new Scanner(string);

	scanner.useDelimiter("[\\W\\s]");

	while (scanner.hasNext()) {
	    String word = scanner.next();

	    if (word.length() <= 0)
		continue;

	    words.add(word);

	    // System.out.print("\"" + word + "\"");
	    // System.out.print("\t");
	    //
	    // for (int i = 0; i < word.length(); i++) {
	    // System.out.printf("%x ", (int) word.charAt(i));
	    // }
	    // System.out.println();
	}
    }

    public Set<String> getWords() {
	return words;
    }

    public Set<URL> getLinks() {
	return links;
    }
}
