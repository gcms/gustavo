package br.ufg.inf.sd2006;

import java.io.IOException;

public interface ClientConnection {
    public Reply sendRequest(Request request) throws IOException;
}
