package br.ufg.inf.graph;

public interface ColorGraphLogger {
    public void prepare(ColorGraph graph);
    public void okColor(Vertex v, Color color);
    public void failColor(Vertex v, Color color, Vertex fail);
    public void backtrack(Vertex v);
}
