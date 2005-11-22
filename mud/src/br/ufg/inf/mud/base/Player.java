package br.ufg.inf.mud.base;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;


public class Player {
    private OutputStream out;

    private Room currentRoom;
    
    private String name;

    public Player(OutputStream out) {
        this(out, null);
    }

    public Player(OutputStream out, Room room) {
        this.out = out;
        setRoom(room);
    }

    public Room getRoom() {
        return currentRoom;
    }

    public void setRoom(Room room) {
        currentRoom = room;
        currentRoom.addPlayer(this);
    }

    public void println(String str) throws IOException {
        DataOutputStream dout = new DataOutputStream(out);

        dout.writeBytes(str);
        dout.writeBytes("\r\n");
    }

    public void joinRoom(Room newRoom) {
        currentRoom.removePlayer(this);
        newRoom.addPlayer(this);
        setRoom(newRoom);
    }

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public String toString() {
		String name = getName();
		return name == null ? super.toString() : name;
	}
}
