
import java.util.HashMap;
import java.util.Map;

public class HessianTransferService implements Transfer {
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
        int j = 10;
        for (int i = 0; i < averageSize; i += j + 4) {
            String str = getString(++j);
            map.put(str, new Integer(i));
        }
        return map;
    }

    public boolean putString(String str, int len) {
        return true;
    }

    public boolean putMap(Map m, int len) {
        return true;
    }

}