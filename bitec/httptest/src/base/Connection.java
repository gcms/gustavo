/*
 * Created on 14/09/2005
 * 
 * TODO To change the template for this generated file go to Window -
 * Preferences - Java - Code Style - Code Templates
 */
package base;

import java.io.IOException;

public interface Connection {

    public boolean isOpen();

    public void open() throws IOException;

    public void close();
}