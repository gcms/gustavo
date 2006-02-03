
public class Problem {
    private int dimension;
    
    public int getDimension() {
        return dimension;
    }
    
    public boolean isSolution(State state) {
        if (state.getProblem() != this) {
            throw new IllegalArgumentException();
        }
        
        int value = 1;
        
        for (int i = 0; i < getDimension(); i++) {
            for (int j = 0; j < getDimension(); j++) {
                if (state.getValue(j, i) != value++ && j != getDimension() - 1) {
                    return false;
                }
            }
        }
        return true;
    }
}
