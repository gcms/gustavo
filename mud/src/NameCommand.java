
public class NameCommand implements Command {

	public boolean execute(MudServer server, Player player, Room room, String command) {
		int index = command.indexOf(' ');
		
		if (index < 0) {
			return false;
		}
		
		player.setName(command.substring(index + 1));
		
		return true;
	}
	
}
