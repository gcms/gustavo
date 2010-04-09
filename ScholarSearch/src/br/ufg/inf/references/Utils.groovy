package br.ufg.inf.references
import org.w3c.dom.Node
import org.apache.xml.serialize.XMLSerializer
import org.apache.xml.serialize.OutputFormat

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:41:44
 * To change this template use File | Settings | File Templates.
 */
class Utils {
    private static Utils instance

    public static Utils getInstance() {
        instance ?: (instance = new Utils())
    }

    private OutputFormat outputFormat

    private Utils() {
        outputFormat = new OutputFormat()
        outputFormat.omitXMLDeclaration = true
        outputFormat.indent = 4
        outputFormat.indenting = true
        outputFormat.preserveSpace = false
    }
    
    public String nodeToHTML(Node node) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream()

        XMLSerializer serializer = new XMLSerializer()
        serializer.setOutputFormat(outputFormat)
        serializer.setOutputByteStream(stream)
        serializer.asDOMSerializer()
        serializer.serialize(node)

        stream.toString()
    }
}
