package br.ufg.inf.mud.command;

import java.io.IOException;

import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;

public class SayCommand implements Command {

    public boolean execute(MudServer server, Player player, Room room,
            String command) {
        int index = command.indexOf(' ');

        command = index < 0 ? command : command.substring(index + 1);

        for (Player p : player.getRoom().getPlayers()) {
            if (p == player || p.equals(player)) {
                continue;
            }
            try {
                p.println(player + " diz '" + command + "'");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        try {
            player.println("Voce diz '" + command + "'");
        } catch (IOException e) {
            e.printStackTrace();
        }

        return true;
    }
}
