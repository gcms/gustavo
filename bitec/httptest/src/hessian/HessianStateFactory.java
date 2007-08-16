package hessian;

import base.State;
import base.StateFactory;

public class HessianStateFactory implements StateFactory {

	public State getState() {
		return new HessianState();
	}

}
