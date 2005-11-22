package br.ufg.inf.mud.command;

import java.io.IOException;

import br.ufg.inf.mud.base.Exit;
import br.ufg.inf.mud.base.MudServer;
import br.ufg.inf.mud.base.Player;
import br.ufg.inf.mud.base.Room;


public class ExitCommand implements Command {

    public boolean execute(MudServer server, Player player, Room room,
            String command) {
        Room r = player.getRoom();

        if (r == null)
            return false;
        Exit e = null;

        if (command.equals("n")) {
            e = r.getNorth();
        } else if (command.equals("s")) {
            e = r.getSouth();
        } else if (command.equals("e")) {
            e = r.getEast();
        } else if (command.equals("w")) {
            e = r.getWest();
        }

        if (e == null) {
            return false;
        }

        r.removePlayer(player);
        try {
            if (e.leaveMessage() != null)
                r.println(player.toString() + " " + e.leaveMessage());
        } catch (IOException e1) {
            e1.printStackTrace();
        }

        Room newRoom = e.getRoom(r);
        try {
            if (e.joinMessage() != null)
                newRoom.println(player.toString() + " " + e.joinMessage());
        } catch (IOException e1) {
            e1.printStackTrace();
        }
//        newRoom.addPlayer(player);
        player.setRoom(newRoom);

        try {
            if (e.ownMessage() != null)
                player.println(e.ownMessage());
        } catch (IOException e1) {
            e1.printStackTrace();
        }

        if (newRoom != r)
            server.dispatchCommand(player, new LookCommand(), command);

        return true;

    }

}
