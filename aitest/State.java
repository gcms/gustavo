import java.awt.Point;

public class State {
	private Problem problem;
	private int[][] data;
	private Point empty;
    
    public State(Problem problem) {
        this.problem = problem;
        
        data = new int[getDimension()][getDimension()];
        
        RandomBag bag = new RandomBag();
        
        for (int i = 0; i < getDimension() * getDimension(); i++) {
            bag.add(new Integer(i));
        }
        
        for (int i = 0; i < getDimension(); i++) {
            for (int j = 0; j < getDimension(); j++) {
                Integer o = (Integer) bag.remove();
                if (o.intValue() == 0) {
                    empty = new Point(i, j);
                 } else {
                    data[i][j] = o.intValue();
                 }
            }
        }        
    }
    	
	public State(State initialState,  Point start, Point end) {
		if (!initialState.getEmpty().equals(end)
                || initialState.getEmpty().equals(start)) {
			throw new IllegalStateException();
		}
		
        problem = initialState.getProblem();
		data = new int[getDimension()][getDimension()]; 
		for (int i = 0; i < getDimension(); i++) {
			for (int j = 0; j < getDimension(); j++) {
				data[i][j] = initialState.getValue(i, j);
			}
		}
        
        empty = end;
        data[end.x][end.y] = data[start.x][start.y];
	}
	
	public Problem getProblem() {
        return problem;
    }

    public Point getEmpty() {
		return empty;
	}
	
	public int getDimension() {
		return getProblem().getDimension();
	}
	
	public int getValue(Point p) {
		return getValue(p.x, p.y);
	}
	
	public int getValue(int x, int y) {
		if (x > getDimension() || y > getDimension()) {
			throw new IndexOutOfBoundsException();
		}
		return data[x][y];
	}
    
    public State[] getNextStates() {
        return null;
    }
}
