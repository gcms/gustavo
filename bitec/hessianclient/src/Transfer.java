import java.util.Map;

public interface Transfer {
	public String getString(int length);
	public boolean putString(String str, int len);

	public Integer[] getIntegers(int arraySize);
	public int[] getInts(int arraySize);

	public Map getMap(int averageSize);
	public boolean putMap(Map m, int len);
}