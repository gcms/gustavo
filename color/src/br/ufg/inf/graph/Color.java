package br.ufg.inf.graph;
/**
 * Pequena classe wrapper, utilizada para armazenar a cor utilizada por um
 * v�rtice em um ColorGraph. A fun��o desta classe � criar uma camada de
 * abstra��o entre as cores utilizadas pelo algoritmo de colora��o de grafos e
 * as cores do sistema.
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class Color {
    /** Cor do sistema. */
    private java.awt.Color ic;

    /** String com o nome da cor. */
    private String name;

    /**
     * Cria uma nova cor a partir de sua cor do sistema, e seu nome.
     * 
     * @param ic um objeto java.awt.Color
     * @param name nome da cor
     */
    private Color(java.awt.Color ic, String name) {
        this.ic = ic;
        this.name = name;
    }

    /**
     * Representa��o em texto da cor.
     * 
     * @return nome da cor
     */
    public String toString() {
        return name;
    }

    /**
     * Obt�m a cor interna, que pode ser utilizada pelo sistema.
     * 
     * @return cor interna
     */
    public java.awt.Color getColor() {
        return ic;
    }

    /**
     * Testa se duas cores s�o iguais.
     * 
     * @param o outra cor a ser checada pela igualdade
     * @return true se o � igual a esta cor
     */
    public boolean equals(Object o) {
        if (o instanceof Color) {
            Color c = (Color) o;

            return getColor().equals(c.getColor())
                    && toString().equals(c.toString());
        }

        return false;
    }

    /** Representa��o para a cor RED. */
    public static final Color RED = new Color(java.awt.Color.RED, "RED");

    /** Representa��o para a cor GREEN. */
    public static final Color GREEN = new Color(java.awt.Color.GREEN, "GREEN");

    /** Representa��o para a cor BLUE. */
    public static final Color BLUE = new Color(java.awt.Color.BLUE, "BLUE");
    
    /** Representa��o para a cor YELLOW. */
    public static final Color YELLOW = new Color(java.awt.Color.YELLOW, "YELLOW");
    
    /** Representa��o para a cor PINK. */
    public static final Color PINK = new Color(java.awt.Color.PINK, "PINK");

}
