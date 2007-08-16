package base;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


public abstract class Runner implements Runnable, Stoppable, NotifySelectable {
    private List listeners;

    public Runner(List list) {
        listeners = list;
    }

    public Runner() {
        this(new ArrayList());
    }

    public void addNotifyListener(NotifyListener l) {
        listeners.add(l);
    }

    public void removeNotifyListener(NotifyListener l) {
        listeners.remove(l);
    }

    public NotifyListener[] getNotifyListeners() {
        return (NotifyListener[]) listeners.toArray();
    }

    protected void notifyListeners(NotifyEvent e) {
        for (Iterator it = listeners.iterator(); it.hasNext(); /* */) {
            NotifyListener listener = (NotifyListener) it.next();
            listener.notificationPerformed(e);
        }
    }
}