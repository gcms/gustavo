package http;

import org.apache.commons.httpclient.HttpState;

import base.State;

public class HttpStateState implements State {
    private HttpState state;

    public HttpState getState() {
        return state;
    }

    public HttpStateState(HttpState state) {
        this.state = state;
    }
}