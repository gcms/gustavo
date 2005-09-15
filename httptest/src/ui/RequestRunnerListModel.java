package ui;

import java.net.URI;

import http.HttpConnectionConnectionFactory;
import http.HttpStateStateFactory;
import http.RequestBase;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.ListModel;
import javax.swing.event.ListDataListener;
import javax.swing.event.ListDataEvent;

import base.ErrorHandler;
import base.RequestLogger;
import base.RequestRunner;

public class RequestRunnerListModel extends RequestRunner implements ListModel,
        SimpleList {
    protected List listeners;

    public RequestRunnerListModel(URI uri, RequestLogger logger,
            ErrorHandler handler) {
        super(new HttpConnectionConnectionFactory(uri),
                new HttpStateStateFactory(), logger, handler);
        listeners = new ArrayList();
    }

    public RequestRunnerListModel(RequestLogger logger, ErrorHandler handler) {
        this(null, logger, handler);
    }

    public RequestRunnerListModel() {
        this(null, null);
    }

    private RequestRunnerListModel(RequestRunner runner) {
        super(runner);
        listeners = new ArrayList();
    }

    public Object clone() {
        return new RequestRunnerListModel((RequestRunner) super.clone());
    }

    public void addListDataListener(ListDataListener l) {
        listeners.add(l);
    }

    public void removeListDataListener(ListDataListener l) {
        listeners.remove(l);
    }

    private void contentsChanged(ListDataEvent e) {
        for (Iterator it = listeners.iterator(); it.hasNext(); /* */) {
            ListDataListener listener = (ListDataListener) it.next();
            listener.contentsChanged(e);
        }
    }

    private void intervalAdded(ListDataEvent e) {
        for (Iterator it = listeners.iterator(); it.hasNext(); /* */) {
            ListDataListener listener = (ListDataListener) it.next();
            listener.contentsChanged(e);
        }
    }

    private void intervalRemoved(ListDataEvent e) {
        for (Iterator it = listeners.iterator(); it.hasNext(); /* */) {
            ListDataListener listener = (ListDataListener) it.next();
            listener.intervalRemoved(e);
        }
    }

    public void addRequest(RequestBase req) {
        super.addRequest(req);
        intervalAdded(new ListDataEvent(this, ListDataEvent.INTERVAL_ADDED,
                super.numRequests() - 1, super.numRequests()));
    }

    public void addRequest(int index, RequestBase req) {
        super.addRequest(index, req);
        intervalAdded(new ListDataEvent(this, ListDataEvent.INTERVAL_ADDED,
                index, index + 1));
    }

    public Object removeRequestByIndex(int index) {
        intervalRemoved(new ListDataEvent(this, ListDataEvent.INTERVAL_REMOVED,
                index, index + 1));
        return super.removeRequest(index);
    }

    public boolean removeRequest(RequestBase req) {
        int index = requests.indexOf(req);
        intervalRemoved(new ListDataEvent(this, ListDataEvent.INTERVAL_REMOVED,
                index, index + 1));
        return super.removeRequest(req);
    }

    public Object remove(int index) {
        return removeRequestByIndex(index);
    }

    public boolean remove(Object o) {
        return removeRequest((RequestBase) o);
    }

    public int size() {
        return numRequests();
    }

    public Object get(int index) {
        return getRequest(index);
    }

    public int getSize() {
        return requests.size();
    }

    public Object getElementAt(int arg0) {
        return getRequest(arg0);
    }

    public void add(Object o) {
        addRequest((RequestBase) o);
    }

    public void add(int index, Object o) {
        addRequest(index, (RequestBase) o);
    }
}