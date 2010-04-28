package br.ufg.inf.utils

import javax.xml.parsers.DocumentBuilder
import org.lobobrowser.html.UserAgentContext
import org.lobobrowser.html.parser.DocumentBuilderImpl
import org.lobobrowser.html.test.SimpleUserAgentContext
import org.w3c.dom.Document
import org.xml.sax.InputSource
import org.lobobrowser.html.parser.InputSourceImpl

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 28/04/2010
 * Time: 10:21:56
 * To change this template use File | Settings | File Templates.
 */
class HTMLParser {
    UserAgentContext context
    DocumentBuilder dbi

    public HTMLParser() {
        context = new ScriptSetupSimpleUserAgentContext() //new SimpleUserAgentContext()
        dbi = new DocumentBuilderImpl(context)
    }

    public Document parse(InputSource is) {
        dbi.parse(is)

    }

    public Document parse(Reader reader, String uri) {
        parse(new InputSourceImpl(reader, uri))
    }

    public Document parse(Reader reader) {
        parse(new InputSourceImpl(reader))
    }
}

class ScriptSetupSimpleUserAgentContext extends SimpleUserAgentContext {
    boolean scriptEnabled = false

    public boolean isScriptingEnabled() {
        scriptEnabled
    }
}