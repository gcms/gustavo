package br.ufg.inf.graph;

import java.util.Iterator;

/**
 * Representação para um grafo que pode ser colorido. Um grafo colorido é um
 * grafo cujos vértices possuem uma cor. Isto é obtido através da adição do
 * atributo "color" nos atributos dos vértices. Um grafo é coloridoa de forma
 * que dois vértices adjacentes não possuem a mesma cor.
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
	 * adjacencia. Uma matriz de adjacencia contêm um número diferente de 0 na
	 * intersecção entre coluna e linha, quando há uma aresta entre dois
	 * vértices. Por exemplo, a matriz abaixo
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
	 *            nome dos vértices
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
	 * Obtem o nome de um vértice.
	 * 
	 * @param v
	 *            vértice cujo nome deseja-se obter
	 * @return nome do vértice v
	 */
	public static String getName(Vertex v) {
		return (String) v.getProperty("name");
	}

	/**
	 * Define o nome de um vértice.
	 * 
	 * @param v
	 *            vértice cujo nome será definido
	 * @param name
	 *            nome do vértice v
	 */
	public static void setName(Vertex v, String name) {
		v.setProperty("name", name);
	}

	/**
	 * Obtem a cor de um vértice.
	 * 
	 * @param v
	 *            vértice cuja cor deseja-se obter
	 * @return cor do vértice v
	 */
	public static Color getColor(Vertex v) {
		Object o = v.getProperty("color");
		if (o != null && o instanceof Color) {
			return (Color) o;
		}

		return null;
	}

	/**
	 * Define a cor de um vértice.
	 * 
	 * @param v
	 *            vértice cuja cor será definida
	 * @param c
	 *            cor do vértice
	 */
	public static void setColor(Vertex v, Color c) {
		v.setProperty("color", c);
	}

	/**
	 * Testa se um vértice está colorido.
	 * 
	 * @param v
	 *            vértice que será testado
	 * @return true se o vértice v está colorido, caso contrário false
	 */
	private static boolean isColored(Vertex v) {
		return getColor(v) != null;
	}

	/**
	 * Testa se um vértice pode ser colorido por uma determinada cor. Um vértice
	 * pode ser colorido por uma cor, se nenhum de seus vértices adjacentes está
	 * colorido com esta cor.
	 * 
	 * @param c
	 *            cor que será testada se pode ser usada para colorir o vértice
	 * @param v
	 *            vértice a ser testado se pode ser colorido pela cor
	 * @return true se o vértice v pode ser colorido com a cor c, senão false
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
	 * @return true se o grafo foi colorido com sucesso, senão false
	 */
	public boolean color(Color[] colors) {
		/*
		 * inicia a coloração a partir do vértice na posição 0 na lista de
		 * vértices
		 */
		return color(colors, 0);
	}

	private boolean color(Color[] colors, int k) {
		/*
		 * se o indíce do vértice está fora dos limites da lista de vértices, ou
		 * se o vértice já está colorido, ele não precisa mais ser colorido, e a
		 * função retorna true.
		 */
		if (k >= getVertices().size() || k < 0
				|| isColored(getVertices().get(k))) {
			return true;
		}

		Vertex v = getVertices().get(k); // vértice na posição k

		/*
		 * tenta colorir o vértice k com cada uma das cores. - caso seja
		 * possível colorir, isto é feito, e tenta-se colorir o vértice na
		 * posição k+1 e adiante através de uma chamada recursiva. - caso
		 * contrário, a próxima cor é testada
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
		 * Caso não seja possível colorir o vértice com nenhuma cor, de forma
		 * que o grafo seja colorido corretamente, o grafo é definido como sem
		 * cor, e a função retorna false.
		 */

		setColor(v, null);
		System.out.println(getName(v) + " BACKTRACKING");
		return false;
	}

	/**
	 * Retorna uma representação do grafo em cadeia de caracteres.
	 * 
	 * @return string contendo representação para o grafo
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
