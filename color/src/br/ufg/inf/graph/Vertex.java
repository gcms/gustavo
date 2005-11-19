package br.ufg.inf.graph;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Classe utilizada para representar um v�rtice em um grafo simples n�o
 * direcionado. Um v�rtice � representado por um conjunto de atributos, como
 * nome, n�mero, cor, entre outros. Al�m disso um v�rtice tamb�m possui um
 * conjunto de v�rtices adjacentes.
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class Vertex {
    /** Mapa usado para armazenar os atributos do v�rtice. */
    private Map<String, Object> properties;

    /** Conjunto de v�rtices adjacentes. */
    private Set<Vertex> neighboors;

    /**
     * Cria um novo v�rtice sem atributos ou vizinhos.
     */
    public Vertex() {
        this(new HashMap<String, Object>());
    }

    /**
     * Cria um novo v�rtice com os atributos especificados.
     * 
     * @param properties atributos deste v�rtice
     */
    public Vertex(Map<String, Object> properties) {
        this.properties = properties;
        neighboors = new HashSet<Vertex>();
    }

    /**
     * Cria um novo v�rtice com um conjunto de v�rtices adjacentes.
     * 
     * @param neighboors conjunto de v�rtices adjacentes
     */
    public Vertex(Collection<Vertex> neighboors) {
        this();
        for (Vertex v : neighboors) {
            addVertex(v);
        }
    }

    /**
     * Cria um novo v�rtice com um conjunto de v�rtices adjacentes.
     * 
     * @param neighboors conjunto de v�rtices adjacentes
     */
    public Vertex(Vertex[] neighboors) {
        this();
        for (Vertex v : neighboors) {
            addVertex(v);
        }
    }

    /**
     * Define um atributo para este v�rtice.
     * 
     * @param property nome do atributo
     * @param value atributo
     */
    public void setProperty(String property, Object value) {
        properties.put(property, value);
    }

    /**
     * Obt�m um atributo deste v�rtice.
     * 
     * @param property nome do atributo
     * @return valor do atributo
     */
    public Object getProperty(String property) {
        return properties.get(property);
    }

    /**
     * Adiciona um v�rtice a lista de v�rtices adjacentes a este, criando uma
     * aresta entre estes.
     * 
     * @param v v�rtice a ser adicionado
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
     * Retorna os v�rtices adjacentes a este v�rtice.
     * 
     * @return v�rtices adjacentes
     */
    public Set<Vertex> neighboors() {
        return neighboors;
    }

    /**
     * Retorna uma representa��o para o v�rtice como cadeia de caracteres.
     * 
     * @return string representando o v�rtice
     */
    public String toString() {
        return properties.toString();
    }
}
