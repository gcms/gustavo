import java.net.URL;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class URLPool {
    private Map<URL, Date> resources = new HashMap<URL, Date>();

    public void addLinks(Set<URL> links) {
	for (URL link : links) {
	    addLink(link);
	}
    }

    public void addLink(URL link) {
	resources.put(link, null);
    }

    public void update(URL url) {
	resources.put(url, new Date());
    }

    public URL nextURL() {
	Date now = new Date();

	for (Map.Entry<URL, Date> e : resources.entrySet()) {
	    if (e.getValue() == null
		    || now.getTime() - e.getValue().getTime() > 24 * 60 * 60 * 1000) {
//		System.out.println(e.getKey() + " | now is " + now + " and time is "
//			+ e.getValue());
		return e.getKey();
	    }
	}

	return null;
    }
}
