package br.ufg.inf.mud.base;


public class NoExit implements Exit {

    public Room getRoom(Room r) {
        return r;
    }

    public String leaveMessage() {
        return null;
    }

    public String joinMessage() {
        return null;
    }

    public String ownMessage() {
        return "Voce nao pode ir nesta direcao";
    }

}
