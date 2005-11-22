package br.ufg.inf.graph;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

/**
 * Classe utilizada para representar um grafo simples n�o direcionado. Um grafo
 * � representado como um conjunto de v�rtices. As arestas existentes entre
 * estes s�o representadas pelos pr�prios v�rtices
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class Graph {
    /** Lista de v�rtices que comp�e o grafo. */
    private List<Vertex> vertices;

    /**
     * Cria um novo grafo sem v�rtices.
     */
    public Graph() {
        vertices = new ArrayList<Vertex>();
    }

    /**
     * Cria um novo grafo formado pelos v�rtices especificados.
     * 
     * @param vertices v�rtices usados para compor o grafo
     */
    public Graph(Collection<Vertex> vertices) {
        this();
        for (Vertex v : vertices) {
            addVertex(v);
        }
    }

    /**
     * Retorna os v�rtices que comp�e este grafo.
     * 
     * @return lista de v�rtices
     */
    protected List<Vertex> getVertices() {
        return vertices;
    }
 
    /**
     * Adiciona um v�rtice ao grafo.
     * 
     * @param v v�rtice a ser adicionado
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
     * Retorna representa��o do grafo em cadeia de caracteres.
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
