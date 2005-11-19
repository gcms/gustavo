import java.io.IOException;

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
                p.println(p + " diz '" + command + "'");
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
