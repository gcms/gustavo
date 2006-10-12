package br.ufg.inf.sd2006;

import java.io.IOException;

public interface ServerConnection {
    public Request getRequest() throws IOException;

    public void sendReply(Reply reply) throws IOException;
}
