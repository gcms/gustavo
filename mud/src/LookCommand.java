import java.io.IOException;

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
