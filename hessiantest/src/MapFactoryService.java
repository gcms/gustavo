
import java.util.HashMap;
import java.util.Map;

public class MapFactoryService implements MapFactory {
	public Map createMap() {
		return new HashMap();
	}

}