package br.ufg.inf.mud.command;

import java.io.IOException;

import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;

public class LookCommand implements Command {

    public boolean execute(MudServer server, Player player, Room room,
            String command) {
        System.out.println(new Exception().getStackTrace()[0]);

        try {
            player.println("");
            player.println("");
            player.println("--------------");
            player.println(player.getRoom().getDescription());
            player.println("");
            player.println(player.getRoom().getPlayers(player));
            player.println("");
            player.println(player.getRoom().getExits());
            player.println("--------------");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }
}
