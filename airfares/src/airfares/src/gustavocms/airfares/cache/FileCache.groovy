package gustavocms.airfares.cache

import javax.xml.bind.annotation.adapters.HexBinaryAdapter
import java.security.MessageDigest

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 10/09/13
 * Time: 08:39
 * To change this template use File | Settings | File Templates.
 */
abstract class FileCache implements Cache {
    long expireTime = 1000 * 60 * 60 * 12
    File dir

    public FileCache(String dirname) {
        dir = new File(dirname)
        if (!dir.exists())
            dir.mkdirs()

        new Thread() {
            public void run() {
                cleanup()
            }
        }.start()
    }

    @Override
    Object get(String key) {
        File file = getFile(key)
        getFromFile(file)
    }

    private boolean hasExpired(File file) {
        if (file.lastModified() < System.currentTimeMillis() - expireTime) {
            file.delete()
            return true
        }

        false
    }

    protected Object getFromFile(File file) {
        synchronized (file.absolutePath) {
            return file.exists() && !hasExpired(file) ? unmarshall(file) : null
        }
    }

    abstract void marshall(File file, Object object)
    abstract Object unmarshall(File file)

    protected static String readFile(File file) {
        synchronized (file.absolutePath) {
            return file.text
        }
    }

    protected static void writeFile(File file, String content) {
        synchronized (file.absolutePath) {
            file.text = content
        }
    }

    protected static boolean removeFile(File file) {
        synchronized (file.absolutePath) {
            return file.delete()
        }
    }

    protected File getFile(String key) {
        new File(dir, getFileName(key))
    }

    private static String getFileName(String key) {
        getMD5(key)
//        key.replaceAll("\\W+", '').substring(0, 25)
    }

    public static String getMD5(String fileName) throws Exception{
        MessageDigest md = MessageDigest.getInstance("MD5")
        (new HexBinaryAdapter()).marshal(md.digest(fileName.getBytes('UTF-8'))) + fileName.length()
    }

    @Override
    void put(String key, Object map) {
        marshall(getFile(key), map)
    }

    @Override
    void flush() {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    void close() {

    }

    private void cleanup() {
        dir.eachFile { File file ->
            hasExpired(file)
        }
    }

    boolean remove(String key) {
        removeFile(getFile(key))
    }
}
