package br.ufg.inf.mud.command;
import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;

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
