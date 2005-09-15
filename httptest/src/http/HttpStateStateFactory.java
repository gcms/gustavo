package http;

import org.apache.commons.httpclient.HttpState;

import base.State;
import base.StateFactory;

public class HttpStateStateFactory implements StateFactory {
    public State getState() {
        return new HttpStateState(new HttpState());
    }
}