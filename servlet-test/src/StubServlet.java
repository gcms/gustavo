
import http.utils.multipartrequest.MultipartRequest;
import http.utils.multipartrequest.ServletMultipartRequest;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

/**
 * This servlet is intented to be used for testing purposes. It can return a
 * specified amount of data, representing the results of the processing.
 * 
 * A client capable of detect the total time delayed between the request and
 * response of this servlet may be able to test the responsiveness of it's
 * network.
 * 
 * @author Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 */
public class StubServlet extends HttpServlet {

    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws IOException {
        String sizeString = req.getParameter("nbytes");

        long size = 0;
        try {
            size = Long.valueOf(sizeString).longValue();
        } catch (NumberFormatException e) {
            size = 0;
        }

        OutputStream out = resp.getOutputStream();

        while (size-- > 0) {
            out.write((int) size);
        }
    }

    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws IOException {
        String sizeString = req.getParameter("nbytes");

        long size = 0;
        try {
            size = Long.valueOf(sizeString).longValue();
        } catch (NumberFormatException e) {
            size = -1;
        }

        MultipartRequest parser = new ServletMultipartRequest(req,
                MultipartRequest.MAX_READ_BYTES,
                MultipartRequest.IGNORE_FILES_IF_MAX_BYES_EXCEEDED, null);

        InputStream in = parser.getFileContents("data");

        long readBytes = 0;
        while (in.read() != -1)
            readBytes++;

        if (readBytes < size) {
            resp.sendError(HttpServletResponse.SC_BAD_REQUEST);
            return;
        }
    }
}
