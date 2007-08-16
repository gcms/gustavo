import java.util.HashMap;
import java.util.Map;

import com.caucho.burlap.server.BurlapServlet;

public class BurlapTransferService extends BurlapServlet implements Transfer {
    private StringBuffer buffer = new StringBuffer();

    public String getString(int length) {
        if (length < 1) {
            length = 1;
        }
        if (buffer.length() < length) {
            for (int i = buffer.length(); buffer.length() < length; i++) {
                buffer.append((char) ('a' + Math.random() * ('z' - 'a')));
            }
            return buffer.toString();
        } else {
            return buffer.substring(0, length - 1);
        }
    }

    public Integer[] getIntegers(int arraySize) {
        return new Integer[arraySize];
    }
    
    public int[] getInts(int arraySize) {
        return new int[arraySize];
    }

    public Map getMap(int averageSize) {
        Map map = new HashMap();
        for (int i = 0; i < averageSize; /* */) {
            String str = getString(averageSize / 10);
            map.put(str, new Integer(i));
            i += str.length() * 2 + 4;
        }
        return map;
    }

    /* (non-Javadoc)
     * @see Transfer#putString(java.lang.String, int)
     */
    public boolean putString(String str, int len) {
        // TODO Auto-generated method stub
        return false;
    }

    /* (non-Javadoc)
     * @see Transfer#putMap(java.util.Map, int)
     */
    public boolean putMap(Map m, int len) {
        // TODO Auto-generated method stub
        return false;
    }

}