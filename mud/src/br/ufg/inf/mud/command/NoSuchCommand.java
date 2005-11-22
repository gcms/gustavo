package br.ufg.inf.mud.command;

import java.io.IOException;

import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;

public class NoSuchCommand implements Command {

    public boolean execute(MudServer server, Player player, Room room, String command) {
        try {
            player.println("Comando nao existente.");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }
}
