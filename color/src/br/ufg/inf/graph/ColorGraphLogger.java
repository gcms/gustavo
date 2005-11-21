package br.ufg.inf.graph;

/**
 * Interface definida para permitir a execu��o de a��es antes de cada etapa na
 * execu��o do algoritmo de colora��o. Esta interface pode ser utilizada para
 * visualizar a execu��o do algoritmo atrav�s da impress�o de dados na sa�da
 * padr�o.
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public interface ColorGraphLogger {
    /**
     * M�todo chamado logo antes de cada tentativa de colorir de um v�rtice.
     * 
     * @param graph grafo do qual o v�rtice faz parte
     * @param v v�tice a ser colorido
     */
    public void prepare(ColorGraph graph, Vertex v);

    /**
     * M�todo chamado quando uma cor pode ser utilizada para colorir o v�rtice.
     * 
     * @param v v�rtice sendo colorido
     * @param color cor utilizada
     */
    public void okColor(Vertex v, Color color);

    /**
     * M�todo chamado quando uma cor n�o pode ser utilizada para colorir o
     * v�rtice.
     * 
     * @param v v�rtice sendo colorido
     * @param color cor utilizada
     * @param fail v�rtice adjacente com a mesma cor
     */

    public void failColor(Vertex v, Color color, Vertex fail);

    /**
     * M�todo chamado quando ocorre um backtracking, ou seja, nenhuma das cores
     * pode ser utilizada para colorir o v�rtice.
     * 
     * @param v v�rtice no qual ocorreu o backtrack
     */
    public void backtrack(Vertex v);
}