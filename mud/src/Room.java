import java.util.Properties;

public class Room {
    private Properties properties;

    private Exit north;

    private Exit south;

    private Exit east;

    private Exit west;

    private Exit up;

    private Exit down;

    public Room() {
        properties = new Properties();
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
}
