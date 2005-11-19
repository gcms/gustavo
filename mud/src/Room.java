import java.io.IOException;
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;

public class Room {
    private Properties properties;

    private Set<Player> players;

    private Exit north;

    private Exit south;

    private Exit east;

    private Exit west;

    private Exit up;

    private Exit down;

    public Room() {
        properties = new Properties();
        players = new HashSet<Player>();
    }

    public void setProperty(String attr, String value) {
        properties.setProperty(attr, value);
    }

    public String getProperty(String attr) {
        return properties.getProperty(attr);
    }

    public Exit getDown() {
        return down;
    }

    public void setDown(Exit down) {
        this.down = down;
    }

    public Exit getEast() {
        return east;
    }

    public void setEast(Exit east) {
        this.east = east;
    }

    public Exit getNorth() {
        return north;
    }

    public void setNorth(Exit north) {
        this.north = north;
    }

    public Exit getSouth() {
        return south;
    }

    public void setSouth(Exit south) {
        this.south = south;
    }

    public Exit getUp() {
        return up;
    }

    public void setUp(Exit up) {
        this.up = up;
    }

    public Exit getWest() {
        return west;
    }

    public void setWest(Exit west) {
        this.west = west;
    }

    public String getDescription() {
        return getProperty("description");
    }

    public void setDescription(String description) {
        setProperty("description", description);
    }

    public void addPlayer(Player player) {
        players.add(player);
    }

    public void removePlayer(Player player) {
        players.remove(player);
    }

    public void println(String str) throws IOException {
        for (Player p : players) {
            p.println(str);
        }
    }

    public Set<Player> getPlayers() {
        return players;
    }

    public String getPlayers(Player player) {
        StringBuilder builder = new StringBuilder();
        for (Player p : players) {
            if (p == player || p.equals(player)) {
                continue;
            }
            builder.append(p.toString());
            builder.append(" esta aqui");
            builder.append("\r\n");
        }

        return builder.toString();
    }

    public String getExits() {
        StringBuilder builder = new StringBuilder("Exits: [");
        if (!(getWest() instanceof NoExit)) {
            builder.append("w ");
        }
        if (!(getEast() instanceof NoExit)) {
            builder.append("e ");
        }
        if (!(getNorth() instanceof NoExit)) {
            builder.append("n ");
        }
        if (!(getSouth() instanceof NoExit)) {
            builder.append("s ");
        }

        builder.append("]");

        return builder.toString();
    }
}
