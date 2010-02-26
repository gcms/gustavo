package gustavocms.translator

/**
 * Created by IntelliJ IDEA.
 * User: fibo
 * Date: Feb 10, 2010
 * Time: 10:15:39 AM
 * To change this template use File | Settings | File Templates.
 */
public class Item {
    String word
    String definition
    String attributes

    String toString() {
        "${word} - ${definition}"
    }
}
