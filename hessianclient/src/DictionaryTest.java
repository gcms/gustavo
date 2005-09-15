import java.net.MalformedURLException;

import com.caucho.hessian.client.HessianProxyFactory;

public class DictionaryTest {
	public static void main(String[] args) throws MalformedURLException,
			ClassNotFoundException {

		HessianProxyFactory factory = new HessianProxyFactory();
		factory.setOverloadEnabled(true);

		Dictionary dic = (Dictionary) factory.create(Class
				.forName("Dictionary"),
				"http://localhost:8080/hessiantest/dictionary");

		String[] words = dic.words();
		System.out.println(words.length);
		for (int i = 0; i < words.length; i++) {
			System.out.println(words[i]);
		}

	}

}
