
import junit.framework.TestCase;

public class GraphicTest extends TestCase {
    private Graphic g;

    public void setUp() {
        g = new Graphic("HTTP", 20);
    }

    public void testLoadFileStringString() {
        try {
            g.loadFile("http_get_local.dat", "GET");
            g.loadFile("http_post_local.dat", "POST");
            assertTrue(true);
        } catch (IllegalArgumentException e) {
            assertTrue(false);
        } catch (Exception e) {
            assertTrue(true);
        }
    }

    public void tearDown() {
        g = null;
    }
}