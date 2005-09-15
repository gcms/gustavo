package base;

public interface RequestFactory {
    /*
     * Creates a new RequestBase based on param args. @param param specify the
     * kind of RequestBase to be created @return a new RequestBase object or
     * null, if none is available
     */
    public Request getRequest(String param);

    /*
     * Creates a new RequestBase. @return a new RequestBase object, or null if
     * none is available
     */
    public Request getRequest();

    public Request getRequest(String param, long size);
}