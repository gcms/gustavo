import junit.framework.TestCase;

public class RandomBagTest extends TestCase {
    public void test1() {
        RandomBag bag = new RandomBag();
        
        bag.add(new Integer(10));
        
        assertEquals(bag.get(), new Integer(10));
        
        bag.add(new Integer(20));
        
        System.out.println(bag.get());
        System.out.println(bag.get());
        System.out.println(bag.get());
        System.out.println(bag.get());     

        Object o = bag.get();
        
        assertTrue(o.equals(new Integer(10)) || o.equals(new Integer(20)));

        if (bag.remove().equals(new Integer(20))) {
            assertEquals(bag.remove(), new Integer(10));
        } else {
            assertEquals(bag.remove(), new Integer(20));
        }
        
        assertTrue(bag.size() == 0);
        
    }
}
