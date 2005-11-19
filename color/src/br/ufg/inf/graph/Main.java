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

        ColorGraphLogger logger = new ColorGraphLogger() {
            public void prepare(ColorGraph graph) {
            }

            public void okColor(Vertex v, Color color) {
                System.out.println(ColorGraph.getName(v) + " = " + color);
            }

            public void failColor(Vertex v, Color color, Vertex fail) {
                System.out.println(ColorGraph.getName(v) + " = " + color
                        + " FALHA c/ " + ColorGraph.getName(fail));
            }

            public void backtrack(Vertex v) {
                System.out.println(ColorGraph.getName(v) + " BACKTRACKING");
            }
        };

        g.setLogger(logger);
        g.color(new Color[] { Color.GREEN, Color.RED, Color.BLUE });

        System.out.println();
        System.out.println("ESTADO FINAL");
        System.out.println(g);
    }
}
