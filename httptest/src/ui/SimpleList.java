package ui;

public interface SimpleList {
    public void add(Object o);

    public Object get(int index);

    public void add(int index, Object o);

    public Object remove(int index);

    public boolean remove(Object o);

    public int size();
}