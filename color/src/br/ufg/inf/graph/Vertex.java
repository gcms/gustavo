package br.ufg.inf.graph;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Classe utilizada para representar um vértice em um grafo simples não
 * direcionado. Um vértice é representado por um conjunto de atributos, como
 * nome, número, cor, entre outros. Além disso um vértice também possui um
 * conjunto de vértices adjacentes.
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class Vertex {
    /** Mapa usado para armazenar os atributos do vértice. */
    private Map<String, Object> properties;

    /** Conjunto de vértices adjacentes. */
    private Set<Vertex> neighboors;

    /**
     * Cria um novo vértice sem atributos ou vizinhos.
     */
    public Vertex() {
        this(new HashMap<String, Object>());
    }

    /**
     * Cria um novo vértice com os atributos especificados.
     * 
     * @param properties atributos deste vértice
     */
    public Vertex(Map<String, Object> properties) {
        this.properties = properties;
        neighboors = new HashSet<Vertex>();
    }

    /**
     * Cria um novo vértice com um conjunto de vértices adjacentes.
     * 
     * @param neighboors conjunto de vértices adjacentes
     */
    public Vertex(Collection<Vertex> neighboors) {
        this();
        for (Vertex v : neighboors) {
            addVertex(v);
        }
    }

    /**
     * Cria um novo vértice com um conjunto de vértices adjacentes.
     * 
     * @param neighboors conjunto de vértices adjacentes
     */
    public Vertex(Vertex[] neighboors) {
        this();
        for (Vertex v : neighboors) {
            addVertex(v);
        }
    }

    /**
     * Define um atributo para este vértice.
     * 
     * @param property nome do atributo
     * @param value atributo
     */
    public void setProperty(String property, Object value) {
        properties.put(property, value);
    }

    /**
     * Obtém um atributo deste vértice.
     * 
     * @param property nome do atributo
     * @return valor do atributo
     */
    public Object getProperty(String property) {
        return properties.get(property);
    }

    /**
     * Adiciona um vértice a lista de vértices adjacentes a este, criando uma
     * aresta entre estes.
     * 
     * @param v vértice a ser adicionado
     */
    public void addVertex(Vertex v) {
        if (v == this) {
            return;
        }

        if (!neighboors.contains(v)) {
            neighboors.add(v);
        }

        if (!v.neighboors.contains(this)) {
            v.neighboors.add(this);
        }
    }

    /**
     * Retorna os vértices adjacentes a este vértice.
     * 
     * @return vértices adjacentes
     */
    public Set<Vertex> neighboors() {
        return neighboors;
    }

    /**
     * Retorna uma representação para o vértice como cadeia de caracteres.
     * 
     * @return string representando o vértice
     */
    public String toString() {
        return properties.toString();
    }
}
