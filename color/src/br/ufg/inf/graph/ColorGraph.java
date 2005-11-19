package br.ufg.inf.graph;

import java.util.Iterator;

/**
 * Representa��o para um grafo que pode ser colorido. Um grafo colorido � um
 * grafo cujos v�rtices possuem uma cor. Isto � obtido atrav�s da adi��o do
 * atributo "color" nos atributos dos v�rtices. Um grafo � coloridoa de forma
 * que dois v�rtices adjacentes n�o possuem a mesma cor.
 * 
 * Para colorir um grafo com as cores verde, vermelho e azul, o seguinte trecho
 * pode ser utilizado.
 * 
 * <pre>
 * String[] names = { &quot;A&quot;, &quot;B&quot;, &quot;C&quot;, &quot;D&quot;, &quot;E&quot;, &quot;F&quot; };
 * byte[][] ajacencias = { { 0, 1, 1, 0, 1, 0 }, { 1, 0, 0, 0, 1, 1 },
 * 		{ 1, 0, 0, 0, 1, 1 }, { 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 0, 1 },
 * 		{ 0, 1, 1, 1, 1, 0 } };
 * 
 * ColorGraph g = new ColorGraph(names, ajacencias);
 * 
 * g.color(new Color[] { Color.GREEN, Color.RED, Color.BLUE });
 * 
 * </pre>
 * 
 * @author Gustavo C. M. Sousa - <gustavomota@inf.ufg.br>
 */
public class ColorGraph extends Graph {

	/**
	 * Cria um novo grafo a partir de uma lista de nomes, e uma matriz de
	 * adjacencia. Uma matriz de adjacencia cont�m um n�mero diferente de 0 na
	 * intersec��o entre coluna e linha, quando h� uma aresta entre dois
	 * v�rtices. Por exemplo, a matriz abaixo
	 * 
	 * <pre>
	 *          A  B  C
	 *       A  0  1  0
	 *       B  0  0  1
	 *       C  0  0  0  
	 * </pre>
	 * 
	 * representa um grafo com as arestas { (A, B), (B, C) }.
	 * 
	 * @param names
	 *            nome dos v�rtices
	 * @param edges
	 *            matriz de adjacencias
	 */
	public ColorGraph(String[] names, byte[][] edges) {
		super();

		int size = Math.min(edges.length, edges[0].length);
		size = Math.min(names.length, size);

		Vertex[] vertices = new Vertex[size];

		for (int i = 0; i < size; i++) {
			vertices[i] = new Vertex();
			setName(vertices[i], names[i]);
			addVertex(vertices[i]);
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (edges[i][j] != 0) {
					vertices[i].addVertex(vertices[j]);
				}
			}
		}
	}

	/**
	 * Obtem o nome de um v�rtice.
	 * 
	 * @param v
	 *            v�rtice cujo nome deseja-se obter
	 * @return nome do v�rtice v
	 */
	public static String getName(Vertex v) {
		return (String) v.getProperty("name");
	}

	/**
	 * Define o nome de um v�rtice.
	 * 
	 * @param v
	 *            v�rtice cujo nome ser� definido
	 * @param name
	 *            nome do v�rtice v
	 */
	public static void setName(Vertex v, String name) {
		v.setProperty("name", name);
	}

	/**
	 * Obtem a cor de um v�rtice.
	 * 
	 * @param v
	 *            v�rtice cuja cor deseja-se obter
	 * @return cor do v�rtice v
	 */
	public static Color getColor(Vertex v) {
		Object o = v.getProperty("color");
		if (o != null && o instanceof Color) {
			return (Color) o;
		}

		return null;
	}

	/**
	 * Define a cor de um v�rtice.
	 * 
	 * @param v
	 *            v�rtice cuja cor ser� definida
	 * @param c
	 *            cor do v�rtice
	 */
	public static void setColor(Vertex v, Color c) {
		v.setProperty("color", c);
	}

	/**
	 * Testa se um v�rtice est� colorido.
	 * 
	 * @param v
	 *            v�rtice que ser� testado
	 * @return true se o v�rtice v est� colorido, caso contr�rio false
	 */
	private static boolean isColored(Vertex v) {
		return getColor(v) != null;
	}

	/**
	 * Testa se um v�rtice pode ser colorido por uma determinada cor. Um v�rtice
	 * pode ser colorido por uma cor, se nenhum de seus v�rtices adjacentes est�
	 * colorido com esta cor.
	 * 
	 * @param c
	 *            cor que ser� testada se pode ser usada para colorir o v�rtice
	 * @param v
	 *            v�rtice a ser testado se pode ser colorido pela cor
	 * @return true se o v�rtice v pode ser colorido com a cor c, sen�o false
	 */
	public static boolean canColor(Color c, Vertex v) {
		for (Iterator<Vertex> it = v.neighboors().iterator(); it.hasNext(); /* */) {
			Vertex n = it.next();
			if (c.equals(getColor(n))) {

				System.out.println("\tFALHA c/ " + getName(n));

				return false;
			}
		}

		System.out.println();

		return true;
	}

	/**
	 * Tenta colorir o grafo com as cores especificadas.
	 * 
	 * @param colors
	 *            cores utilizadas para colorir o grafo
	 * @return true se o grafo foi colorido com sucesso, sen�o false
	 */
	public boolean color(Color[] colors) {
		/*
		 * inicia a colora��o a partir do v�rtice na posi��o 0 na lista de
		 * v�rtices
		 */
		return color(colors, 0);
	}

	private boolean color(Color[] colors, int k) {
		/*
		 * se o ind�ce do v�rtice est� fora dos limites da lista de v�rtices, ou
		 * se o v�rtice j� est� colorido, ele n�o precisa mais ser colorido, e a
		 * fun��o retorna true.
		 */
		if (k >= getVertices().size() || k < 0
				|| isColored(getVertices().get(k))) {
			return true;
		}

		Vertex v = getVertices().get(k); // v�rtice na posi��o k

		/*
		 * tenta colorir o v�rtice k com cada uma das cores. - caso seja
		 * poss�vel colorir, isto � feito, e tenta-se colorir o v�rtice na
		 * posi��o k+1 e adiante atrav�s de uma chamada recursiva. - caso
		 * contr�rio, a pr�xima cor � testada
		 */
		for (int i = 0; i < colors.length; i++) {
			synchronized (this) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
					break;
				}
			}
			System.out.print(getName(v) + " = " + colors[i]);
			if (canColor(colors[i], v)) {
				setColor(v, colors[i]);
				if (color(colors, k + 1)) {
					return true;
				}
			}
		}

		/*
		 * Caso n�o seja poss�vel colorir o v�rtice com nenhuma cor, de forma
		 * que o grafo seja colorido corretamente, o grafo � definido como sem
		 * cor, e a fun��o retorna false.
		 */

		setColor(v, null);
		System.out.println(getName(v) + " BACKTRACKING");
		return false;
	}

	/**
	 * Retorna uma representa��o do grafo em cadeia de caracteres.
	 * 
	 * @return string contendo representa��o para o grafo
	 */
	public String toString() {
		StringBuilder builder = new StringBuilder();
		for (Iterator<Vertex> it = getVertices().iterator(); it.hasNext(); /* */) {
			Vertex v = it.next();
			builder.append(v);
			builder.append("\t");

			for (Iterator<Vertex> in = v.neighboors().iterator(); in.hasNext(); /* */) {
				builder.append(getName(in.next()));
				builder.append(", ");
			}
			builder.append("\n");
		}

		return builder.toString();
	}
}
