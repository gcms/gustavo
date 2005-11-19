package br.ufg.inf.graph;
public class Main {

    public static void main(String[] args) {
        String[] names = { "A", "B", "C", "D", "E", "F" };
        byte[][] ajacencias = { { 0, 1, 1, 0, 1, 0 }, { 1, 0, 0, 0, 1, 1 },
                { 1, 0, 0, 0, 1, 1 }, { 0, 0, 0, 0, 0, 1 },
                { 1, 1, 1, 0, 0, 1 }, { 0, 1, 1, 1, 1, 0 } };

        ColorGraph g = new ColorGraph(names, ajacencias);

        System.out.println("ESTADO INICIAL");
        System.out.println(g);

        g.color(new Color[] { Color.GREEN, Color.RED, Color.BLUE });

        
        System.out.println("ESTADO FINAL");
        System.out.println(g);
    }
}
