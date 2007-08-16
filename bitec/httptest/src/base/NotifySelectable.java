package base;

public interface NotifySelectable {
    public void addNotifyListener(NotifyListener listener);

    public void removeNotifyListener(NotifyListener listener);

    public NotifyListener[] getNotifyListeners();
}