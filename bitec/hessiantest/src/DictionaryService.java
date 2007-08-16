import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class DictionaryService implements Dictionary {
    private static String prefix = System.getProperty("dicprefix") + "/";

    private static String filename = "wordseed.txt";

    private static String translator = "libbab.exe";

    private Map dictionary;

    public DictionaryService() throws IOException {

        dictionary = new HashMap();

        InputStream in = new FileInputStream(prefix + filename);
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));

        String line;

        while (dictionary.size() < 500 && (line = reader.readLine()) != null) {
            String[] data = line.split("[^\\w]+");
            for (int i = 0; i < data.length; i++) {
                dictionary.put(data[i], translateFromProcess(data[i]));
            }
        }
    }

    private static String translateFromProcess(String word) throws IOException {
        String[] cmdArray = new String[] { prefix + translator, word };
        Process p = Runtime.getRuntime().exec(cmdArray);
        return readInputStream(p.getInputStream());
    }

    private static String readInputStream(InputStream in) throws IOException {
        StringBuffer buffer = new StringBuffer(in.available());
        int ch;
        while ((ch = in.read()) != -1) {
            buffer.append((char) ch);
        }
        return buffer.toString();
    }

    public String[] words() {
        return (String[]) dictionary.keySet().toArray(new String[] {});
    }

    public String translate(String word) {
        String result = (String) dictionary.get(word);

        try {
            result = translateFromProcess(word);
            if (result.matches("^\\s*$")) {
                result = null;
            }
        } catch (IOException e) {
            result = null;
        }

        return result;
    }

    public String[] words(int num) {
        num = num > dictionary.size() ? dictionary.size() : num;
        String[] result = new String[num];
        int i = 0;
        for (Iterator it = dictionary.keySet().iterator(); i < num
                && it.hasNext(); i++) {
            result[i] = (String) it.next();
        }

        return result;
    }
}
