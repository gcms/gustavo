public class SorterService implements Sorter {
	public String[] sort(String[] array) {
		for (int i = array.length; i >= 0; i--) {
			for (int j = 0; j < i - 1; j++) {
				if (array[j].compareTo(array[j + 1]) > 0) {
					String tmp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = tmp;
				}
			}
		}
		return array;
	}
}