package br.ufg.inf.mud.command;
import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;

public interface Command {
    public boolean execute(MudServer server, Player player, Room room,
            String command);
}
