package gustavocms.translator;

import java.io.DataInput;
import java.io.IOException;

public class Utils {

    public static long charIndex(String str) {
        if (str.length() != 1)
            throw new IllegalArgumentException();

        return charIndex(str.charAt(0));
    }

    public static long charIndex(Character ch) {
        return charIndex((int) ch.charValue());
    }

    /*
        Returns a number that will be used to build the index for words.
        This considers only letters, and the special symbol "'" (single quote)

        It means that other symbols are not significative, and will not be considered
        while building the index of the word.
     */
    public static long charIndex(int ch) {
        System.out.printf("charIndex(%d)\n", ch);

        if (ch >= 'a' && ch <= 'z')
            return ch - 'a' + 2;
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 2;
        if (ch == '\'')
            return 1;

        return 0;
    }

    /*
        Builds the index of a word, based on its 3 first characters.
        Code written in C, bellow for better understanding.
     */
//        switch (word.length()) {
//            default:
//                idx += charIndex(word.charAt(2))
//            case 2:

    //                idx += charIndex(word.charAt(1)) * 28
//            case 1:
//                idx += charIndex(word.charAt(0)) * 28 * 28
//            case 0:
//                break
//        }
    public static long wordIndex(String word) {
        long multiplier = 28 * 28;

        int maxSize = Math.min(word.length(), 3);

        long result = 0;
        for (int i = 0; i < maxSize; i++) {
            result += charIndex(word.charAt(i)) * multiplier;
            multiplier /= 28;
        }

        return result;
    }

    /*
    Read bytes in inverted order...
    For example....
    reading 2 bytes from the following stream...
        0x12 0x44 0x24 0x01
    leads to... 0x4412
     */
    public static long readValue(DataInput input, int n) throws IOException {
        long result = 0;

        for (int i = 0; i < n; ++i) {
            long b = input.readUnsignedByte();
            result |= b << (8 * i);
        }

        return result;
    }
}
