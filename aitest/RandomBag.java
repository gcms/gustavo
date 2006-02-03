import java.util.ArrayList;
import java.util.List;

public class RandomBag {
    private List items;
    
    private int lastIndex = -1;
    
    public RandomBag() {
        items = new ArrayList();
    }
    
    public void add(Object o) {
    
        items.add(o);
    }
    
    public Object get() {
       lastIndex = (int) (Math.random() * size());
        
        return items.get(lastIndex);
    }
    
    public Object remove() {
        if (lastIndex == -1) {
            get();
        }
        
        Object o = items.remove(lastIndex);
        lastIndex = -1;
        return o;
    }
    
    public int size() {
        return items.size();
    }
}
