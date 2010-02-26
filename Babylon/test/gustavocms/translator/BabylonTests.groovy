package gustavocms.translator

/**
 * Created by IntelliJ IDEA.
 * User: fibo
 * Date: Feb 10, 2010
 * Time: 9:12:26 AM
 * To change this template use File | Settings | File Templates.
 */
public class BabylonTests extends GroovyTestCase {
    public void testeRepeticao() {
        File index = new File('res/english.dic')
        File definition = new File('res/EngtoPor.dic')
        Babylon babylon = new Babylon(index, definition)

        println babylon.translate('plane')
        println babylon.translate('ship')
        println babylon.translate('starship')
        println babylon.translate('WORD')

        println babylon.translate('John')
        println babylon.translate('Germany')
    }

    public void testeBasico() {
        File index = new File('res/english.dic')
        File definition = new File('res/EngtoPor.dic')
        Babylon babylon = new Babylon(index, definition)

        List translations = babylon.translate('purpose')

        Item item1 = translations.first()
        assertEquals '47020000000341', item1.attributes
        assertEquals 'purpose', item1.word
        println translations
    }

    public void testWordIndex() {
        File index = new File('res/english.dic')
        File definition = new File('res/EngtoPor.dic')
        Babylon babylon = new Babylon(index, definition)

        assertEquals 19283, babylon.wordIndex('word')
    }
}
