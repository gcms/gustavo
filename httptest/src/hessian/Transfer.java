package hessian;

import java.util.Map;

public interface Transfer {
	public String getString(long l);

	public boolean putString(String str, long len);

	public Integer[] getIntegers(long arraySize);

	public int[] getInts(long arraySize);

	public Map getMap(long l);

	public boolean putMap(Map m, long len);
}