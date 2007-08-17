import java.net.URL;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.collections15.list.SetUniqueList;

public class WordIndex {
    private Map<String, List<URL>> invertedFile;

    public WordIndex() {
	invertedFile = new HashMap<String, List<URL>>();
    }

    public void addWord(String word, URL resId) {
	word = Utils.normalize(word);
	getResources(word).add(resId);
    }

    private List<URL> getResources(String word) {
	word = Utils.normalize(word);
	List<URL> resources = invertedFile.get(word);

	if (resources == null) {
	    resources = newResources();
	    invertedFile.put(word, resources);
	}

	return resources;
    }

    private List<URL> newResources() {
	return SetUniqueList.decorate(new LinkedList<URL>());
    }

    public List<URL> getResources(String... words) {
	List<URL> result = null;
	for (String word : words) {
	    if (result == null) {
		result = getResources(word);
		continue;
	    }

	    result.retainAll(getResources(word));
	}

	return result;
    }

    public void index(URL url, Resource resource) {
	for (String word : resource.getWords()) {
	    addWord(word, url);
	}
    }
}
