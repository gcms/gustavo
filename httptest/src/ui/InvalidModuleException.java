package ui;

public class InvalidModuleException extends RuntimeException {
	
	public InvalidModuleException(Throwable e) {
		super(e);
	}
	
	public InvalidModuleException(String message) {
		super("Invalid Module: " + message);
	}
}
