package br.ufg.inf.graph;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

/**
 * Classe utilizada para representar um grafo simples não direcionado. Um grafo
 * é representado como um conjunto de vértices. As arestas existentes entre
 * estes são representadas pelos próprios vértices
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class Graph {
    /** Lista de vértices que compõe o grafo. */
    private List<Vertex> vertices;

    /**
     * Cria um novo grafo sem vértices.
     */
    public Graph() {
        vertices = new ArrayList<Vertex>();
    }

    /**
     * Cria um novo grafo formado pelos vértices especificados.
     * 
     * @param vertices vértices usados para compor o grafo
     */
    public Graph(Collection<Vertex> vertices) {
        this();
        for (Vertex v : vertices) {
            addVertex(v);
        }
    }

    /**
     * Retorna os vértices que compõe este grafo.
     * 
     * @return lista de vértices
     */
    protected List<Vertex> getVertices() {
        return vertices;
    }

    //
    //
    // public Graph(String[] names, byte[][] edges) {
    // this();
    // int size = Math.min(edges.length, edges[0].length);
    // size = Math.min(names.length, size);
    //
    // Vertex[] vertices = new Vertex[size];
    //
    // for (int i = 0; i < size; i++) {
    // vertices[i] = new Vertex();
    // setName(vertices[i], names[i]);
    // addVertex(vertices[i]);
    // }
    //
    // for (int i = 0; i < size; i++) {
    // for (int j = 0; j < size; j++) {
    // if (edges[i][j] != 0) {
    // vertices[i].addVertex(vertices[j]);
    // }
    // }
    // }
    // }

    /**
     * Adiciona um vértice ao grafo.
     * 
     * @param v vértice a ser adicionado
     */
    public void addVertex(Vertex v) {
        if (!vertices.contains(v)) {
            vertices.add(v);
            for (Iterator<Vertex> iv = v.neighboors().iterator(); iv.hasNext(); /* */) {
                addVertex(iv.next());
            }
        }
    }

    /**
     * Retorna representação do grafo em cadeia de caracteres.
     * 
     * @return string representando o grafo
     */
    public String toString() {
        StringBuilder builder = new StringBuilder();
        for (Iterator<Vertex> iv = vertices.iterator(); iv.hasNext(); /* */) {
            builder.append(iv.next().toString());
            builder.append("\n");
        }

        return builder.toString();
    }

}
