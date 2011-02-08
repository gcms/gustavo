package br.ufg.inf.utils

import org.w3c.dom.Node
import com.sun.org.apache.xml.internal.serialize.OutputFormat
import com.sun.org.apache.xml.internal.serialize.XMLSerializer

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:41:44
 * To change this template use File | Settings | File Templates.
 */
class XMLUtils {
    private static XMLUtils instance

    public static XMLUtils getInstance() {
        instance ?: (instance = new XMLUtils())
    }

    private OutputFormat outputFormat

    private XMLUtils() {
        outputFormat = new OutputFormat()
        outputFormat.omitXMLDeclaration = true
        outputFormat.indent = 4
        outputFormat.indenting = true
        outputFormat.preserveSpace = false
    }

    public String nodeToHTML(Node node) {
        StringWriter writer = new StringWriter()

        XMLSerializer serializer = new XMLSerializer()
        serializer.setOutputFormat(outputFormat)
        serializer.setOutputCharStream(writer)
        serializer.asDOMSerializer()
        serializer.serialize(node)

        writer.toString()
    }
}
