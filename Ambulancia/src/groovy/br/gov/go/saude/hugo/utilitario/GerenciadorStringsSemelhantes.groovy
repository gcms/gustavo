package br.gov.go.saude.hugo.utilitario

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 12:55:18
 * To change this template use File | Settings | File Templates.
 */
public class GerenciadorStringsSemelhantes {
    private ComparadorStringsSemelhantes verificador
    private List itens = []
    private Map mapaDeItens = [:]

    public GerenciadorStringsSemelhantes(ComparadorStringsSemelhantes verificador) {
        this.verificador = verificador
    }

    public String obtenhaChave(String texto) {
        String chave = verificador.obtenhaChave(texto)

        mapaDeItens[chave] ?: encontreItemSemelhante(chave) ?: incluaItem(chave)
    }

    private String encontreItemSemelhante(String chave) {
        mapaDeItens[chave] = itens.find { verificador.ehSemelhante(chave, it) }
    }

    private String incluaItem(String chave) {
        itens.add(chave)
        mapaDeItens[chave] = chave
        chave
    }

    public void reset() {
        itens.clear()
        mapaDeItens.clear()
    }
}

public interface ComparadorStringsSemelhantes {
    String obtenhaChave(String texto)
    boolean ehSemelhante(String source, String target)
}

public class ComparadorDistanciaStrings implements ComparadorStringsSemelhantes{
    int distanciaMaxima

    public ComparadorDistanciaStrings(int distanciaMaxima) {
        this.distanciaMaxima = distanciaMaxima
    }

    public String obtenhaChave(String texto) {
        texto
    }

    public boolean ehSemelhante(String source, String target) {
        LevenshteinDistance.calcule(source, target) <= distanciaMaxima
    }
}

public class ComparadorNomesSemelhantes extends ComparadorDistanciaStrings {
    private static final int DISTANCIA_MAXIMA = 3

    private ComparadorNomesSemelhantes() {
        super(DISTANCIA_MAXIMA)
    }

    public String obtenhaChave(String nome) {
        nome.toUpperCase().
                replaceAll('\\s+', ' ').
                replace(' DA ', ' ').
                replace(' DAS ', ' ').
                replace(' DO ', ' ').
                replace(' DOS ', ' ').
                replace(' DE ', ' ')
    }

    public boolean ehSemelhante(String source, String target) {
        source = source.replaceAll('\\s', '')
        target = target.replaceAll('\\s', '')
        LevenshteinDistance.calcule(source, target) <= distanciaMaxima
    }

    private static ComparadorStringsSemelhantes instancia
    public static ComparadorStringsSemelhantes getInstancia() {
        instancia = instancia ?: new ComparadorNomesSemelhantes()
    }
}
