package br.ufg.inf.graph;

/**
 * Interface definida para permitir a execução de ações antes de cada etapa na
 * execução do algoritmo de coloração. Esta interface pode ser utilizada para
 * visualizar a execução do algoritmo através da impressão de dados na saída
 * padrão.
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public interface ColorGraphLogger {
    /**
     * Método chamado logo antes de cada tentativa de colorir de um vértice.
     * 
     * @param graph grafo do qual o vértice faz parte
     * @param v vétice a ser colorido
     */
    public void prepare(ColorGraph graph, Vertex v);

    /**
     * Método chamado quando uma cor pode ser utilizada para colorir o vértice.
     * 
     * @param v vértice sendo colorido
     * @param color cor utilizada
     */
    public void okColor(Vertex v, Color color);

    /**
     * Método chamado quando uma cor não pode ser utilizada para colorir o
     * vértice.
     * 
     * @param v vértice sendo colorido
     * @param color cor utilizada
     * @param fail vértice adjacente com a mesma cor
     */

    public void failColor(Vertex v, Color color, Vertex fail);

    /**
     * Método chamado quando ocorre um backtracking, ou seja, nenhuma das cores
     * pode ser utilizada para colorir o vértice.
     * 
     * @param v vértice no qual ocorreu o backtrack
     */
    public void backtrack(Vertex v);
}