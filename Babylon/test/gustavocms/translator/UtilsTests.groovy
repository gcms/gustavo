package gustavocms.translator

/**
 * Created by IntelliJ IDEA.
 * User: fibo
 * Date: Feb 10, 2010
 * Time: 10:23:21 AM
 * To change this template use File | Settings | File Templates.
 */
public class UtilsTests extends GroovyTestCase {
    void testeCharIndex() {
        assertEquals 19, Utils.charIndex('r')
        assertEquals 22, Utils.charIndex('u')
        assertEquals 17, Utils.charIndex('p')
        assertEquals 1, Utils.charIndex("'")
        assertEquals 0, Utils.charIndex('*')
        assertEquals 24, Utils.charIndex('w')
    }

    void testeWordIndex() {
        assertEquals 19283, Utils.wordIndex('word')
        assertEquals 13963, Utils.wordIndex('purpose')
        assertEquals 13795, Utils.wordIndex('portrait')
        assertEquals 1568, Utils.wordIndex('a__')
        assertEquals 16722, Utils.wordIndex('the')

        assertEquals(0, Utils.wordIndex(''))
        assertEquals 2 * 28 * 28, Utils.wordIndex('a')
        assertEquals((2 * 28 * 28) + (3 * 28), Utils.wordIndex('ab'))
        assertEquals((2 * 28 * 28) + (3 * 28) + 4, Utils.wordIndex('abc'))
        assertEquals((2 * 28 * 28) + (3 * 28) + 4, Utils.wordIndex('abcd'))

        assertEquals Utils.wordIndex('purpose'), Utils.wordIndex('pur')
        assertEquals Utils.wordIndex('a__'), Utils.wordIndex('a')
        assertEquals Utils.wordIndex('a**'), Utils.wordIndex('a')
    }

    void testeReadValue() {
        RandomAccessFile file = new RandomAccessFile("res/binario.bin", "r")
        /* binario.bin contém...
            0x01
            0x00 0x02
            0x00 0x00 0x00 0x04
            0x12 0x34 0x56 0x78
         */

        assertEquals 1, Utils.readValue(file, 1)
        assertEquals 0x0200, Utils.readValue(file, 2)
        assertEquals 0x04000000, Utils.readValue(file, 4)
        assertEquals 0x78563412, Utils.readValue(file, 4)
    }
}
