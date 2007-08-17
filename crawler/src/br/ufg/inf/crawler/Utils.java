package br.ufg.inf.crawler;
import java.text.Normalizer;
import java.text.Normalizer.Form;

public class Utils {
    public static String normalize(String string) {
	string = Normalizer.normalize(string, Form.NFKD);
	string = string.replaceAll("[\u0301\u0302\u0303\u0327]", "");

	string = string.toLowerCase();

	return string;
    }
}
